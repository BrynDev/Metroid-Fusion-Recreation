#pragma once

#include "Vector2f.h"
#include "Camera.h"
#include <vector>
#include "Texture.h"
#include "GameInfo.h"
#include "SoundStream.h"

class Manager;
class ProjectileManager;
class EnemyManager;
class PowerUpManager;
class Avatar;
class Level;
class HUD;
class Menu;

class Game
{
public:
	explicit Game( const Window& window );
	Game(const Game& other) = delete;
	Game& operator=(const Game& other) = delete;
	Game(Game&& other) = delete;
	Game& operator=(Game&& other) = delete;
	~Game();

	void Update( float elapsedSec );
	void Draw( ) const;

	// Event handling
	void ProcessKeyDownEvent( const SDL_KeyboardEvent& e );
	void ProcessKeyUpEvent( const SDL_KeyboardEvent& e );
	void ProcessMouseMotionEvent( const SDL_MouseMotionEvent& e );
	void ProcessMouseDownEvent( const SDL_MouseButtonEvent& e );
	void ProcessMouseUpEvent( const SDL_MouseButtonEvent& e );

private:
	// DATA MEMBERS
	const Window m_Window;
	Level* m_pLevel;
	Avatar* m_pAvatar;
	Camera m_Camera;
	Menu* m_pMenu;
	ProjectileManager* m_pProjectileManager;
	EnemyManager* m_pEnemyManager;
	PowerUpManager* m_pPowerUpManager;
	Texture m_GameOverTexture;
	SoundStream m_BackgroundMusic;
	std::vector<Manager*> m_pManagers;
	GameInfo m_GameInfo;
	float m_GameOverAlphaValue;
	bool m_IsRestartOK;
	// FUNCTIONS
	void Initialize( );
	void Cleanup( );
	void ClearBackground( ) const;
	void PrintInfo() const;
	void Reset();
	void ChangeVolumes();
};