#include "pch.h"
#include "AvatarProjectile.h"
#include "Sprite.h"
#include "Level.h"
#include "SoundEffect.h"
#include <algorithm>

AvatarProjectile::AvatarProjectile(const Vector2f& velocity, const int damage, const std::string& shootSoundPath, const std::string& impactSoundPath)
	:Projectile{velocity, damage}
{
	m_pSounds.insert(std::make_pair(0, new SoundEffect{ shootSoundPath }));
	m_pSounds.insert(std::make_pair(1, new SoundEffect{impactSoundPath}));
	m_pSounds.insert(std::make_pair(int(ImpactType::enemyImmune), new SoundEffect{ "Resources/Projectiles/Impact_EnemyImmune.wav" }));

}

AvatarProjectile::~AvatarProjectile()
{
	std::for_each(m_pSounds.begin(), m_pSounds.end(), [](std::pair<int, SoundEffect*> pair) {delete pair.second; });
}

void AvatarProjectile::SetShape()
{
	const int scale{ 2 };
	m_Shape.width = m_pSprites.find(int(m_TravelState))->second->GetFrameWidth() * scale;
	m_Shape.height = m_pSprites.find(int(m_TravelState))->second->GetFrameHeight() * scale;
}

void AvatarProjectile::SetTravelState()
{
	if (m_ActiveVelocity.y == 0)
	{
		m_TravelState = TravelState::horizontal;
	}
	else if (m_ActiveVelocity.x == 0)
	{
		m_TravelState = TravelState::vertical;
	}
	else
	{
		m_TravelState = TravelState::diagonal;
	}
}

void AvatarProjectile::SetImpactShape()
{
	const int scale{ 2 };
	Sprite* pCurrentSprite{ m_pSprites.find(int(m_TravelState))->second };
	Point2f center{ m_Shape.left + pCurrentSprite->GetFrameWidth() / 2.f, m_Shape.bottom + pCurrentSprite->GetFrameHeight() / 2.f };
	m_Shape.width = m_pSprites.find(int(m_ImpactType))->second->GetFrameWidth() * scale;
	m_Shape.height = m_pSprites.find(int(m_ImpactType))->second->GetFrameHeight() * scale;
	Point2f newBottomLeft{ center.x - m_Shape.width / 2.f, center.y - m_Shape.height / 2.f };
	m_Shape.left = newBottomLeft.x;
	m_Shape.bottom = newBottomLeft.y;
}

void AvatarProjectile::HandleEnemyHit(const bool wasDamageDealt)
{
	m_HasHit = true;
	if (!wasDamageDealt)
	{
		m_ImpactType = ImpactType::enemyImmune;
		m_pSounds.find(int(m_ImpactType))->second->Play(0);
	}
	else
	{
		m_ImpactType = ImpactType::enemy;
		//m_pSounds.find(int(m_HasHit))->second->Play(0);
	}
	SetImpactShape();
	
}

void AvatarProjectile::Draw() const
{
	if (!m_HasHit)
	{
		m_pSprites.find(int(m_TravelState))->second->Draw(m_Shape, m_IsHorFlipped, m_IsVertFlipped);
	}
	else
	{
		m_pSprites.find(int(m_ImpactType))->second->Draw(m_Shape, m_IsHorFlipped);
	}

}

void AvatarProjectile::Update(float elapsedSec, const Level* pLevel)
{
	if (!m_HasHit)
	{
		m_pSprites.find(int(m_TravelState))->second->Update(elapsedSec);

		UpdatePos(elapsedSec);

		if (pLevel->CheckProjectileCollision(m_Shape, m_Velocity))
		{
			m_ImpactType = ImpactType::level;
			SetImpactShape();		
			m_HasHit = true;
			m_pSounds.find(int(m_HasHit))->second->Play(0);
		}
	}
	else
	{
		m_pSprites.find(int(m_ImpactType))->second->Update(elapsedSec);
		if (m_pSprites[int(m_ImpactType)]->HasFinished())
		{
			ToggleActive();
			m_HasHit = false;
		}
	}
}

void AvatarProjectile::Activate(const Point2f& startPos, const bool isTravellingLeft, const bool isTravellingRight, const bool isTravellingUp, const bool isTravellingDown)
{
	SetDataMembers(startPos, isTravellingLeft, isTravellingRight, isTravellingUp, isTravellingDown);
	SetTravelState();
	SetShape();
	ToggleActive();
	m_pSounds.find(int(m_HasHit))->second->Play(0);//hashit is false
}

void AvatarProjectile::SetDataMembers(const Point2f& startPos, const bool isTravellingLeft, const bool isTravellingRight, const bool isTravellingUp, const bool isTravellingDown)
{
	SetPos(startPos);
	SetFlipped(isTravellingLeft, isTravellingDown);
	SetActiveVelocity(isTravellingLeft, isTravellingRight, isTravellingUp, isTravellingDown);
}

void AvatarProjectile::SetFlipped(const bool isTravellingLeft, const bool isTravellingDown)
{
	m_IsHorFlipped = isTravellingLeft;
	m_IsVertFlipped = isTravellingDown;
}

float AvatarProjectile::GetHorVelocity() const
{
	return m_ActiveVelocity.x;
}

void AvatarProjectile::ChangeEffectVolume(const int newVolume)
{
	std::for_each(m_pSounds.begin(), m_pSounds.end(), [&newVolume](std::pair<int, SoundEffect*> pair) {pair.second->SetVolume(newVolume); });
}