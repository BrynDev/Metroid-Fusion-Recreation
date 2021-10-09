#include "pch.h"
#include "Moto.h"
#include "Sprite.h"
#include "Avatar.h"
#include "Parasite.h"
#include "Level.h"
#include "Missile.h"
#include <fstream>
#include <assert.h>
#include "SoundEffect.h"

Moto::Moto(const Point2f& pos, const bool isFlipped, Parasite* pParasite)
	:Enemy(25,8, 0, pos, isFlipped, 215, true, pParasite)
	, m_State{EnemyState::idle}
	, m_Acceleration{ 320 }
	, m_MaxSpeed{290}
	, m_WalkSpeed{60}
	, m_RunSpeed{0}
	, m_BrakeDeceleration{-500}
	, m_IsTurnOK{false}
	, m_IsDetectOK{true}
{
	std::ifstream spriteInfo{ "Resources/Enemies/Moto/SpriteInfo.txt" };
	assert(spriteInfo);
	InitSprites(spriteInfo);
}

void Moto::Draw() const
{
	if (m_IsDead)
	{
		m_pParasite->Draw();
		return;
	}

	if (m_IsFlashing)
	{
		m_pSprites.find(int(m_State))->second->Draw(m_Shape, m_IsFlipped, false, Color4f(1.f, 0.5f, 0.5f, 1.f));
	}
	else
	{
		m_pSprites.find(int(m_State))->second->Draw(m_Shape, m_IsFlipped);
	}
	
}

void Moto::Update(float elapsedSec, GameInfo& gameInfo)
{
	if (m_IsDead)
	{
		m_pParasite->Update(elapsedSec, gameInfo.pAvatar);
		return;
	}
	else if (m_IsDying)
	{
		m_State = EnemyState::dying;
	}

	const Point2f avatarCenter{ gameInfo.pAvatar->GetCenter() };
	m_pSprites.find(int(m_State))->second->Update(elapsedSec);
	

	DetectAvatar(avatarCenter);
	
	if ((m_TargetFound) && (m_State == EnemyState::idle || m_State == EnemyState::walking) && (m_IsDetectOK))
	{
		m_State = EnemyState::taunting;
	}
	else if ((m_State == EnemyState::charging) && (HasPassedAvatar(avatarCenter)))
	{
		m_State = EnemyState::braking;
	}
	

	switch (m_State)
	{
	case EnemyState::idle:
		HandleIdleBehaviour();
		break;
	case EnemyState::walking:
		UpdatePos(elapsedSec);
		HandleWalkTimer(elapsedSec);
		break;
	case EnemyState::taunting:
		HandleTauntingBehaviour();
		break;
	case EnemyState::charging:
		HandleChargingBehaviour(elapsedSec);
		
		break;
	case EnemyState::braking:
		HandleBrakingBehaviour(elapsedSec);
		break;
	case EnemyState::dying:
		if (m_pSprites[int(m_State)]->HasFinished())
		{
			HandleDeath();
		}
		break;
	}

	if (m_IsFlashing)
	{
		HandleFlashingTimer(elapsedSec);
	}
	HandleCollision(gameInfo.pLevel);
	SetShapeSize();
}

void Moto::ResetUniqueDataMembers()
{
	m_IdleCounter = 0;
	m_AccuWalkTime = 0;
	m_TauntCounter = 0;
	m_State = EnemyState::idle;
}

void Moto::HandleWalkTimer(float elapsedSec)
{
	const float maxWalkTime{0.6f};
	m_AccuWalkTime += elapsedSec;
	if (m_AccuWalkTime >= maxWalkTime)
	{
		m_AccuWalkTime = 0;
		m_State = EnemyState::idle;
		m_Velocity.x = 0;
	}
}

void Moto::HandleCollision(const Level* pLevel)
{
	utils::HitInfo hitInfo{};
	if (pLevel->CheckHorEnemyCollision(m_Shape, hitInfo))
	{
		
		if (!m_IsFlipped && m_Velocity.x >= 0)
		{
			m_Shape.left = hitInfo.intersectPoint.x - m_Shape.width - 15;
		}
		else if (m_IsFlipped && m_Velocity.x < 0)
		{
			m_Shape.left = hitInfo.intersectPoint.x + 7;
		}
		m_IsTurnOK = true;
		m_IsDetectOK = false;
		m_State = EnemyState::idle;
		m_Velocity.x = 0;
	}
	if (pLevel->CheckVertEnemyCollision(m_Shape, hitInfo))
	{
		m_Shape.bottom = hitInfo.intersectPoint.y + 1;
	}

}

void Moto::SetShapeSize()
{
	const int scale{ 2 };
	m_Shape.width = m_pSprites[int(m_State)]->GetFrameWidth() * scale;
	m_Shape.height = m_pSprites[int(m_State)]->GetFrameHeight() * scale;
}

bool Moto::HasPassedAvatar(const Point2f& avatarCenter)
{
	const int range{120};
	if (m_IsFlipped)
	{
		return (avatarCenter.x > GetCenter().x + range);

	}
	else
	{
		return (avatarCenter.x < GetCenter().x - range);
	}
}

void Moto::HandleHit(const AvatarProjectile* pAvProj)
{
	bool weaknessHit{ false };
	if (dynamic_cast<const Missile*>(pAvProj) != nullptr)
	{
		weaknessHit = true;
	}
	else
	{
		const float projectileVelocity{ pAvProj->GetHorVelocity() };
		if ((projectileVelocity > 0 && !m_IsFlipped) || (projectileVelocity < 0 && m_IsFlipped))
		{
			weaknessHit = true;
		}
	}

	if (weaknessHit)
	{
		m_Health -= pAvProj->GetDamage();
		if (m_Health <= 0)
		{
			m_IsDying = true;
			m_pSounds.find(int(m_IsDying))->second->Play(0);
		}
		else
		{
			m_IsFlashing = true;
			m_pSounds.find(int(m_IsDying))->second->Play(0);
		}
		m_WasDamageTaken = true;
	}
}

void Moto::UpdatePos(float elapsedSec)
{
	m_Shape.left += m_Velocity.x * elapsedSec;
}

void Moto::HandleIdleBehaviour()
{
	const int maxIdleCount{ 2 };
	if (m_pSprites.find(int(m_State))->second->HasFinished())
	{
		++m_IdleCounter;
		if (m_IdleCounter >= maxIdleCount)
		{
			m_IdleCounter = 0;
			m_State = EnemyState::walking;

			if (m_IsTurnOK)
			{
				m_IsFlipped = !m_IsFlipped;
				m_IsTurnOK = false;
				m_IsDetectOK = true;
			}
			m_Velocity.x = (!m_IsFlipped * 2 - 1)*m_WalkSpeed;
		}
	}
}

void Moto::HandleTauntingBehaviour()
{
	const int maxTauntCount{ 2 };
	if (m_pSprites.find(int(m_State))->second->HasFinished())
	{
		++m_TauntCounter;
	}
	if (m_TauntCounter >= maxTauntCount)
	{
		m_TauntCounter = 0;
		m_Velocity.x = 0;
		m_State = EnemyState::charging;
	}
}

void Moto::HandleChargingBehaviour(float elapsedSec)
{
	m_Velocity.x += (!m_IsFlipped * 2 - 1) * m_Acceleration * elapsedSec;
	if (m_Velocity.x > m_MaxSpeed)
	{
		m_Velocity.x = m_MaxSpeed;
	}
	else if (m_Velocity.x < -m_MaxSpeed)
	{
		m_Velocity.x = -m_MaxSpeed;
	}
	UpdatePos(elapsedSec);
}

void Moto::HandleBrakingBehaviour(float elapsedSec)
{
	const float epsilon{ 2.f };
	m_Velocity.x += (!m_IsFlipped * 2 - 1) * m_BrakeDeceleration * elapsedSec;
	UpdatePos(elapsedSec);

	if (m_Velocity.x < epsilon && m_Velocity.x > -epsilon)
	{
		m_Velocity.x = 0;
		m_State = EnemyState::idle;
		m_IsTurnOK = true;
	}
}