#include "pch.h"
#include "EnemyProjectile.h"
#include "Sprite.h"
#include "Avatar.h"
#include "utils.h"
#include "Level.h"

EnemyProjectile::EnemyProjectile(const Vector2f& velocity, const int damage)
	:Projectile(velocity, damage)
{
}

void EnemyProjectile::Draw() const
{
	m_pSprites.find(int(m_HasHit))->second->Draw(m_Shape,m_IsHorFlipped);
}

void EnemyProjectile::Update(float elapsedSec, const Level* pLevel)
{
	if (!m_HasHit)
	{
		UpdatePos(elapsedSec);
		if (pLevel->CheckProjectileCollision(m_Shape, m_Velocity))
		{
			SetImpactShape();
			m_HasHit = true;
		}
	}
	else
	{
		if (m_pSprites.find(int(m_HasHit))->second->HasFinished())
		{
			ToggleActive();
			m_HasHit = false;
		}
	}
	m_pSprites.find(int(m_HasHit))->second->Update(elapsedSec);
}

void EnemyProjectile::HandleAvatarHit(Avatar* pAvatar)
{
	if (utils::IsOverlapping(m_Shape, pAvatar->GetShape()))
	{
		m_HasHit = true;
		SetImpactShape();
		pAvatar->HandleHit(m_Damage); 
	}
}

void EnemyProjectile::Activate(const Point2f& startPos, const bool isFlipped)
{
	ToggleActive();
	SetPos(Point2f{ startPos.x + 10, startPos.y });
	SetShape();
	SetActiveVelocity(isFlipped, !isFlipped, false, true);
}

void EnemyProjectile::SetImpactShape()
{
	const int scale{ 2 };
	Sprite* pCurrentSprite{ m_pSprites.find(int(!m_HasHit))->second };
	Point2f center{ m_Shape.left + pCurrentSprite->GetFrameWidth() / 2.f, m_Shape.bottom + pCurrentSprite->GetFrameHeight() / 2.f };
	m_Shape.width = m_pSprites.find(int(m_HasHit))->second->GetFrameWidth() * scale;
	m_Shape.height = m_pSprites.find(int(m_HasHit))->second->GetFrameHeight() * scale;
	Point2f newBottomLeft{ center.x - m_Shape.width / 2.f, center.y - m_Shape.height / 2.f };
	m_Shape.left = newBottomLeft.x;
	m_Shape.bottom = newBottomLeft.y;
}

void EnemyProjectile::SetShape()
{
	const int scale(2);
	m_Shape.width = m_pSprites.find(int(m_HasHit))->second->GetFrameWidth() * scale;
	m_Shape.height = m_pSprites.find(int(m_HasHit))->second->GetFrameHeight() * scale;
}