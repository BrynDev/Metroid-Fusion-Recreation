#pragma once
#include "GameInfo.h"
#include "Manager.h"
#include <vector>

class Avatar;
class AvatarProjectile;
class Sprite;
class Enemy;
class Zombie;

class EnemyManager : public Manager
{
public:
	EnemyManager();
	virtual ~EnemyManager();
	EnemyManager(const EnemyManager& other) = delete;
	EnemyManager& operator=(const EnemyManager& other) = delete;
	EnemyManager(EnemyManager&& other) = delete;
	EnemyManager& operator=(EnemyManager&& rhs) = delete;

	virtual void Draw() const override;
	virtual void Update(float elapsedSec, GameInfo& gameInfo) override;
	std::pair<bool,bool> HandleHit(const AvatarProjectile* pAvProj) const;
	void ChangeEffectVolume(const int newVolume);
protected:
	std::vector<Enemy*> m_pEnemies;

	void ResetEnemies();
};

