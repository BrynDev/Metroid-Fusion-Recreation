#include "pch.h"
#include "Hornad.h"
#include "Sprite.h"
#include "utils.h"
#include "Level.h"
#include <algorithm>
#include "Avatar.h"
#include "ProjectileManager.h"
#include "Parasite.h"
#include <fstream>
#include <assert.h>

Hornad::Hornad(const Point2f& startPos, const bool isFlipped, Parasite* pParasite)
	:Enemy(15,7, 90, startPos, isFlipped,120, true,pParasite)
	, m_State{EnemyState::idle}
	, m_IdleAnimCounter{0}
	, m_IdleAnimCountMax{2}
	, m_IsJumping{false}
	, m_JumpSpeed{400}
	, m_Acceleration{0,-1700.f}
	, m_MaxNrJumps{3}
	, m_NrJumps{}
	, m_IsTurnOK{false}
	, m_HasShot{false}
{	
	std::ifstream spriteInfo{ "Resources/Enemies/Hornad/SpriteInfo.txt" };
	assert(spriteInfo);
	InitSprites(spriteInfo);
}

void Hornad::Draw() const
{
	if (!m_IsDead)
	{
		if (m_IsFlashing)
		{
			m_pSprites.find(int(m_State))->second->Draw(m_Shape, m_IsFlipped, false, Color4f(1, 0.5f, 0.5f, 1));
		}
		else
		{
			m_pSprites.find(int(m_State))->second->Draw(m_Shape, m_IsFlipped);
		}
	}
	else
	{
		m_pParasite->Draw();
	}
}

void Hornad::Update(float elapsedSec, GameInfo& gameInfo)
{
	if (m_IsDead)
	{
		m_pParasite->Update(elapsedSec, gameInfo.pAvatar);
		return;
	}
	if (m_IsDying)
	{
		m_State = EnemyState::dying;
	}
	int spriteIdx{ int(m_State) };
	m_pSprites.find(int(spriteIdx))->second->Update(elapsedSec);

	m_IsOnGround = gameInfo.pLevel->IsEnemyOnGround(m_Shape);
	DetectAvatar(gameInfo.pAvatar->GetCenter());
	switch (m_State)
	{
	case EnemyState::idle:
		
		if (m_TargetFound)
		{
			if (!m_HasShot)
			{
				m_State = EnemyState::shooting;
			}
			else
			{
				StartJump();
			}
		}
		else if (m_pSprites.find(int(spriteIdx))->second->HasFinished())
		{ 
			HandleIdleBehavior();
		}
		break;
	case EnemyState::moving:
		
		HandleMoveBehavior(elapsedSec);
		HandleCollision(gameInfo.pLevel);
		break;
	case EnemyState::turning:
		if (m_pSprites.find(int(spriteIdx))->second->HasFinished())
		{
			m_IsFlipped = !m_IsFlipped;
			m_State = EnemyState::idle;
			m_Velocity.x = -m_Velocity.x;
		}
		break;
	case EnemyState::shooting:
		if (m_pSprites.find(int(spriteIdx))->second->HasFinished())
		{
			gameInfo.pProjectileManager->ActivateHornadShot(GetCenter(),m_IsFlipped);
			m_HasShot = true;
			m_State = EnemyState::idle;
		}
		break;
	case EnemyState::dying:
		if (m_pSprites.find(int(spriteIdx))->second->HasFinished())
		{
			HandleDeath();
		}
	}
	if (m_IsFlashing)
	{
		HandleFlashingTimer(elapsedSec);
	}
	const int scale{ 2 };
	ScaleShape(scale, m_pSprites[spriteIdx]->GetFrameWidth(), m_pSprites[spriteIdx]->GetFrameHeight());
}

void Hornad::HandleIdleBehavior()
{
	++m_IdleAnimCounter;
	if ((m_IdleAnimCounter %= m_IdleAnimCountMax) == 0)
	{
		if (m_IsTurnOK)
		{
			m_IsTurnOK = false;
			m_State = EnemyState::turning;
		}
		else
		{
			StartJump();
		}
	}
}

void Hornad::HandleMoveBehavior(float elapsedSec)
{
	m_Velocity.y += m_Acceleration.y * elapsedSec;
	if (m_Velocity.y < 0 && m_IsOnGround) //jump has finished
	{
		m_Velocity.y = 0;
		if (!m_TargetFound)
		{
			++m_NrJumps;
			m_IsTurnOK = (m_NrJumps %= m_MaxNrJumps) == 0;
			m_State = EnemyState::idle;
			m_HasShot = false;
		}
		else
		{
			StartJump();
		}
	}
	UpdatePos(elapsedSec);
}

void Hornad::UpdatePos(float elapsedSec)
{
	m_Shape.left += m_Velocity.x * elapsedSec;
	m_Shape.bottom += m_Velocity.y * elapsedSec;
}

void Hornad::HandleCollision(const Level* pLevel)
{
	utils::HitInfo hitInfo;
	if (pLevel->CheckHorEnemyCollision(m_Shape, hitInfo))
	{
		if (!m_IsFlipped)
		{
			m_Shape.left = hitInfo.intersectPoint.x - m_Shape.width - 2;
		}
		else
		{
			m_Shape.left = hitInfo.intersectPoint.x + 2;
		}
	}

	if (pLevel->CheckVertEnemyCollision(m_Shape, hitInfo))
	{
		if (m_Velocity.y > 0)
		{
			m_Shape.bottom = hitInfo.intersectPoint.y - m_Shape.height - 2;
			
		}
		else
		{
			m_Shape.bottom = hitInfo.intersectPoint.y + 1;
		}
		m_Velocity.y = 0;
	}
}

void Hornad::StartJump()
{
	m_State = EnemyState::moving;
	m_Velocity.y = float(m_JumpSpeed);
}

void Hornad::ResetUniqueDataMembers()
{
	m_NrJumps = 0;
	m_State = EnemyState::idle;
	m_IsTurnOK = false;
	m_IsOnGround = true;
	m_IsJumping = false;
	m_IdleAnimCounter = 0;
	m_HasShot = false;
}