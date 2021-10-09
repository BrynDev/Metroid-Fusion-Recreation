#pragma once
#include "structs.h"

class ProjectileManager;
class EnemyManager;
class Level;
class Avatar;

struct GameInfo
{
	ProjectileManager* pProjectileManager;
	EnemyManager* pEnemyManager;
	const Level* pLevel;
	Avatar* pAvatar;
};