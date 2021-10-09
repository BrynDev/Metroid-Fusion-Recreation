#include "pch.h"
#include "Level.h"
#include "Sprite.h"
#include "Zombie.h"
#include "Hornad.h"
#include <algorithm>
#include "EnemyManager.h"
#include "GameInfo.h"
#include "ProjectileManager.h"
#include "Avatar.h"
#include "YellowParasite.h"
#include "GreenParasite.h"
#include "RedParasite.h"
#include "AvatarProjectile.h"
#include "Geron.h"
#include "Moto.h"


EnemyManager::EnemyManager()
{
	m_pEnemies.push_back(new Zombie{ Point2f(4739, 29), false,new YellowParasite() });
	m_pEnemies.push_back(new Hornad{ Point2f(3392, 997), false, new YellowParasite() });
	m_pEnemies.push_back(new Geron{ Point2f(3249, 1660), new RedParasite() });
	m_pEnemies.push_back(new Zombie{ Point2f(2590, 1026), false, new YellowParasite() });
	m_pEnemies.push_back(new Hornad{ Point2f(1589, 995), false, new YellowParasite() });
	m_pEnemies.push_back(new Moto{ Point2f(1310, 633), true, new GreenParasite() });
	m_pEnemies.push_back(new Hornad{ Point2f(107, 634), false, new YellowParasite() });
	m_pEnemies.push_back(new Hornad{ Point2f(269, 634), false, new YellowParasite() });
	m_pEnemies.push_back(new Zombie{ Point2f(2502, 1346), false, new YellowParasite() });
	m_pEnemies.push_back(new Hornad{ Point2f(3140, 1669), true, new YellowParasite() });
	m_pEnemies.push_back(new Moto{ Point2f(4599, 1627), true, new YellowParasite() });
	m_pEnemies.push_back(new Moto{ Point2f(4340, 1627), true, new GreenParasite() });
	m_pEnemies.push_back(new Zombie{ Point2f(4207, 1627), false, new GreenParasite() });
	m_pEnemies.push_back(new Hornad{ Point2f(4121, 1990), false, new YellowParasite() });
	m_pEnemies.push_back(new Moto{ Point2f(5031, 2220), true, new GreenParasite() });
	m_pEnemies.push_back(new Geron{ Point2f(6059, 2222), new RedParasite() });
}	
 
EnemyManager::~EnemyManager()
{
	std::for_each(m_pEnemies.begin(), m_pEnemies.end(), [](Enemy* pEnemy) { delete pEnemy; });
	
}

void EnemyManager::Draw() const
{
	std::for_each(m_pEnemies.begin(), m_pEnemies.end(), [](Enemy* pEnemy) { pEnemy->Draw();});
}

void EnemyManager::Update(float elapsedSec, GameInfo& gameInfo)
{
	if (gameInfo.pAvatar->HasGoneThroughDoor())
	{
		ResetEnemies();
	}

	std::for_each(m_pEnemies.begin(), m_pEnemies.end(), [elapsedSec, &gameInfo](Enemy* pEnemy) {
		pEnemy->Update(elapsedSec, gameInfo);
		if (!pEnemy->IsDying())
		{
			pEnemy->HandleAvatarContact(gameInfo.pAvatar);
		}
	});
	
	
}

std::pair<bool,bool> EnemyManager::HandleHit(const AvatarProjectile* pAvProj) const
{
	const Rectf projectileShape{ pAvProj->GetShape() };
	bool hasHit{ false };
	bool hasTakenDamage{ false };
	hasHit = std::any_of(m_pEnemies.begin(), m_pEnemies.end(), [&hasTakenDamage,projectileShape,pAvProj](Enemy* pEnemy) {
		if (!pEnemy->IsDead() && !pEnemy->IsDying())
		{
			if (utils::IsOverlapping(projectileShape, pEnemy->GetShape()))
			{
				pEnemy->HandleHit(pAvProj);
				hasTakenDamage = pEnemy->WasDamageTakenFromHit();
				return true;
			}
		}
		return false;
	});
	return std::make_pair(hasHit, hasTakenDamage);
}

void EnemyManager::ResetEnemies()
{
	std::for_each(m_pEnemies.cbegin(), m_pEnemies.cend(), [](Enemy* pEnemy) {pEnemy->Reset(); });
}

void EnemyManager::ChangeEffectVolume(const int newVolume)
{
	std::for_each(m_pEnemies.cbegin(), m_pEnemies.cend(), [&newVolume](Enemy* pEnemy) {pEnemy->ChangeEffectVolume(newVolume); });
}