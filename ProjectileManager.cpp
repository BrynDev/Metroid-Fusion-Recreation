#include "pch.h"
#include "ProjectileManager.h"
#include "Utils.h"
#include "AvatarProjectile.h"
#include "Beam.h"
#include "Missile.h"
#include "Sprite.h"
#include "EnemyManager.h"
#include <algorithm>
#include "EnemyProjectile.h"
#include "HornadShot.h"
#include "Projectile.h"
#include "Avatar.h"


ProjectileManager::ProjectileManager()
{
	const int nrBeams{ 13 };
	const int nrMissiles{ 15 };
	const int nrHornadShots{ 3 };

	for (int i{}; i < nrBeams; ++i)
	{
		m_pAvatarProjectiles.push_back(new Beam{});
	}
	for (int i{}; i < nrMissiles; ++i)
	{
		m_pAvatarProjectiles.push_back(new Missile{});
	}
	for (int i{}; i < nrHornadShots; ++i)
	{
		m_pEnemyProjectiles.push_back(new HornadShot{});
	}
	m_pProjectiles.insert(m_pProjectiles.cbegin(), m_pAvatarProjectiles.cbegin(), m_pAvatarProjectiles.cend());
	m_pProjectiles.insert(m_pProjectiles.cbegin(), m_pEnemyProjectiles.cbegin(), m_pEnemyProjectiles.cend());
}

ProjectileManager::~ProjectileManager()
{
	std::for_each(m_pProjectiles.begin(), m_pProjectiles.end(), [](Projectile* pProj) {delete pProj; });
}

void ProjectileManager::ActivateBeam(const Point2f& startPos, const bool isTravellingLeft, const bool isTravellingRight, const bool isTravellingUp, const bool isTravellingDown)
{
	std::any_of(m_pAvatarProjectiles.begin(), m_pAvatarProjectiles.end(), [startPos, isTravellingLeft, isTravellingRight, isTravellingUp, isTravellingDown](AvatarProjectile* pAvProj) {
		if (dynamic_cast<Beam*>(pAvProj) != nullptr)
		{
			if (!pAvProj->IsActive())
			{
				pAvProj->Activate(startPos, isTravellingLeft, isTravellingRight, isTravellingUp, isTravellingDown);
				return true;
			}
		}
		return false;
	});
}

void ProjectileManager::ActivateMissile(const Point2f& startPos, const bool isTravellingLeft, const bool isTravellingRight, const bool isTravellingUp, const bool isTravellingDown)
{
	std::any_of(m_pAvatarProjectiles.begin(), m_pAvatarProjectiles.end(), [startPos, isTravellingLeft, isTravellingRight, isTravellingUp, isTravellingDown](AvatarProjectile* pAvProj) {
		if (dynamic_cast<Missile*>(pAvProj) != nullptr)
		{
			if (!pAvProj->IsActive())
			{
				pAvProj->Activate(startPos, isTravellingLeft, isTravellingRight, isTravellingUp, isTravellingDown);
				return true;
			}
		}
		return false;
	});
}

void ProjectileManager::ActivateHornadShot(const Point2f& startPos, const bool isFlipped)
{
	std::any_of(m_pEnemyProjectiles.begin(), m_pEnemyProjectiles.end(), [startPos, isFlipped](EnemyProjectile* pProj) {
		if (dynamic_cast<HornadShot*>(pProj) != nullptr && !pProj->IsActive())
		{
			pProj->Activate(startPos, isFlipped);
			return true;
		}		
		return false;
	});
}

void ProjectileManager::Update(float elapsedSec, GameInfo& gameInfo)
{
	std::for_each(m_pProjectiles.begin(), m_pProjectiles.end(), [elapsedSec, &gameInfo](Projectile* pProj) {
		if (pProj->IsActive())
		{
			pProj->Update(elapsedSec, gameInfo.pLevel);
		}
	});

	std::for_each(m_pAvatarProjectiles.begin(), m_pAvatarProjectiles.end(), [&gameInfo](AvatarProjectile* pProj) {
		if (pProj->IsActive() && !pProj->HasHit())
		{
			std::pair<bool, bool> hitTypeInfo{ gameInfo.pEnemyManager->HandleHit(pProj) }; //pair to determine if projectile has hit an enemy and if projectile did any damage (enemy not immune)
			if (hitTypeInfo.first)
			{
				pProj->HandleEnemyHit(hitTypeInfo.second);
			}			
		}
	});

	std::for_each(m_pEnemyProjectiles.begin(), m_pEnemyProjectiles.end(), [&gameInfo](EnemyProjectile* pProj) {
		if (pProj->IsActive() && !pProj->HasHit())
		{
			pProj->HandleAvatarHit(gameInfo.pAvatar);
		}
	});
}

void ProjectileManager::Draw() const
{
	std::for_each(m_pProjectiles.begin(), m_pProjectiles.end(), [](Projectile* pProj) {
		if (pProj->IsActive())
		{
			pProj->Draw();
		}
	});
} 

void ProjectileManager::ChangeEffectVolume(const int newVolume)
{
	std::for_each(m_pAvatarProjectiles.begin(), m_pAvatarProjectiles.end(), [&newVolume](AvatarProjectile* pProj) {pProj->ChangeEffectVolume(newVolume); });
}