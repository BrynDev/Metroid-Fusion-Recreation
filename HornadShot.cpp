#include "pch.h"
#include "HornadShot.h"
#include "Sprite.h"
#include <fstream>
#include <assert.h>

HornadShot::HornadShot()
	:EnemyProjectile(Vector2f{830,80},10)
{
	std::ifstream spriteInfo{ "Resources/Projectiles/HornadShot/SpriteInfo.txt" };
	assert(spriteInfo);
	InitSprites(spriteInfo);
}

