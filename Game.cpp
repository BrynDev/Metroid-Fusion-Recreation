#include "pch.h"
#include "Game.h"
#include "utils.h"
#include "GameInfo.h"
#include "Manager.h"
#include "ProjectileManager.h"
#include "EnemyManager.h"
#include "PowerUpManager.h"
#include "Avatar.h"
#include "Level.h"
#include "Texture.h"
#include "HUD.h"
#include <algorithm>
#include "SoundStream.h"
#include "Menu.h"



Game::Game( const Window& window )
	:m_Window{ window }
	, m_Camera{window.width, window.height}
	, m_GameOverTexture{ "Resources/General/GameOver.png" }	
	, m_BackgroundMusic{ "Resources/General/MainDeck.mp3"}
	
{	 
	Initialize( );
}

Game::~Game( )
{
	Cleanup( );
}

void Game::Initialize( )
{
	m_pAvatar = new Avatar(Point2f(5, m_Window.height - 30));
	m_pProjectileManager = new ProjectileManager{} ;
	m_pEnemyManager = new EnemyManager{};
	m_pPowerUpManager = new PowerUpManager{};
	m_pLevel = new Level{};
	m_pMenu = new Menu{ Point2f(m_Window.width / 2.f, m_Window.height / 2.f) };

	m_GameInfo.pEnemyManager = m_pEnemyManager;
	m_GameInfo.pProjectileManager = m_pProjectileManager;
	m_GameInfo.pAvatar = m_pAvatar;
	m_GameInfo.pLevel = m_pLevel;

	m_Camera.SetLevelBoundaries(m_pLevel->GetBoundaries(), m_pLevel->GetDoorInfo());

	m_pManagers.push_back(m_pProjectileManager);
	m_pManagers.push_back(m_pEnemyManager);
	m_pManagers.push_back(m_pPowerUpManager);

	m_GameOverAlphaValue = 0;
	m_IsRestartOK = false;
	m_BackgroundMusic.Play(true);
}

void Game::Cleanup( )
{
	delete m_pLevel;
	delete m_pAvatar;
	delete m_pMenu;
	std::for_each(m_pManagers.begin(), m_pManagers.end(), [](Manager* pManager) { delete pManager;});
	m_pManagers.clear();
	
}

void Game::Update( float elapsedSec )
{
	
	if (m_pAvatar->IsGameOver() && !m_IsRestartOK)
	{
		if (m_GameOverAlphaValue < 256.f)
		{
			++m_GameOverAlphaValue;
		}
		else
		{
			m_IsRestartOK = true;
		}		
		return;
	}

	if (m_pMenu->IsOpen())
	{
		m_pMenu->Update(elapsedSec);
		return;
	}

	std::for_each(m_pManagers.begin(), m_pManagers.end(), [elapsedSec, this](Manager* pManager)
	{
		pManager->Update(elapsedSec, m_GameInfo);
	});

	m_pAvatar->Update(elapsedSec, m_pLevel);
	m_pLevel->Update(elapsedSec, m_GameInfo);
}

void Game::Draw( ) const
{
	ClearBackground( );
	glPushMatrix();
	{
		m_Camera.Transform(m_pAvatar->GetCollisionShape());
		m_pLevel->DrawBackground();
		m_pAvatar->Draw();
		std::for_each(m_pManagers.cbegin(), m_pManagers.cend(), [](const Manager* pManager) {pManager->Draw(); });
		m_pLevel->DrawForeground();
	}
	glPopMatrix();
	m_pAvatar->DrawHUD();
	if (m_pMenu->IsOpen())
	{
		m_pMenu->Draw();
		return;
	}

	if (m_pAvatar->IsGameOver())
	{
		if (m_IsRestartOK)
		{
			m_GameOverTexture.Draw(Rectf(0, 0, m_Window.width, m_Window.height));
		}
		else
		{
			utils::SetColor(Color4f(1, 1, 1, m_GameOverAlphaValue / 255.f));
			utils::FillRect(0, 0, m_Window.width, m_Window.height);
		}		
	}
}

void Game::ProcessKeyDownEvent( const SDL_KeyboardEvent & e )
{
}

void Game::ProcessKeyUpEvent( const SDL_KeyboardEvent& e )
{
	switch (e.keysym.sym)
	{
	case SDLK_i:
		PrintInfo();
		break;
	
	case SDLK_e:
		m_pAvatar->Shoot(m_GameInfo.pProjectileManager);
		break;
	case SDLK_r:
		m_pAvatar->ToggleMissileLauncher();
		break;
	case SDLK_h:
		if (m_IsRestartOK)
		{
			Reset();
		}
		break;
	case SDLK_ESCAPE:
		if (!m_pAvatar->IsDying())
		{
			m_pMenu->IsOpen() ? m_BackgroundMusic.Resume() : m_BackgroundMusic.Pause();
			m_pMenu->ToggleOpen();
			if (!m_pMenu->IsOpen())
			{
				ChangeVolumes();
			}
		}
		break;
	case SDLK_UP:
		if (m_pMenu->IsOpen())
		{
			m_pMenu->MoveCursorUp();
		}
		break;
	case SDLK_DOWN:
		if (m_pMenu->IsOpen())
		{
			m_pMenu->MoveCursorDown();
		}
		break;
	case SDLK_LEFT:
		if (m_pMenu->IsOpen())
		{
			m_pMenu->DecreaseItemValue();
		}
		break;
	case SDLK_RIGHT:
		if (m_pMenu->IsOpen())
		{
			m_pMenu->IncreaseItemValue();
		}
		break;
	}
}

void Game::ProcessMouseMotionEvent( const SDL_MouseMotionEvent& e )
{
}

void Game::ProcessMouseDownEvent( const SDL_MouseButtonEvent& e )
{
}

void Game::ProcessMouseUpEvent( const SDL_MouseButtonEvent& e )
{
}

void Game::ClearBackground( ) const
{
	glClearColor( 0.0f, 0.0f, 0.0f, 1.0f );
	glClear( GL_COLOR_BUFFER_BIT );
}

void Game::PrintInfo() const
{
	std::cout << "Controls: \n";
	std::cout << "A: move left \n";
	std::cout << "D: move right \n";
	std::cout << "S: crouch \n";
	std::cout << "W: aim upwards \n";
	std::cout << "UP/DOWN arrows: aim diagonally / navigate menu\n";
	std::cout << "LEFT/RIGhT arrows: navigate menu\n";
	std::cout << "SPACE: jump \n";
	std::cout << "E: shoot\n";
	std::cout << "R: Toggle missile launcher (after getting missile launcher)";
	std::cout << "H: restart (on game over screen)\n";

}

void Game::Reset()
{
	Cleanup();
	m_BackgroundMusic.Stop();
	Initialize();
}

void Game::ChangeVolumes()
{
	int newMusicVolume{ m_pMenu->GetMusicVolume() * 10};
	m_BackgroundMusic.SetVolume(newMusicVolume);
	int newEffectVolume{ m_pMenu->GetEffectVolume() * 10 };
	m_pAvatar->ChangeEffectVolume(newEffectVolume);
	m_pLevel->ChangeEffectVolume(newEffectVolume);
	m_pProjectileManager->ChangeEffectVolume(newEffectVolume);
	m_pPowerUpManager->ChangeEffectVolume(newEffectVolume);
	m_pEnemyManager->ChangeEffectVolume(newEffectVolume);
}