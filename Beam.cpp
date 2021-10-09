#include "pch.h"
#include "Beam.h"
#include "utils.h"
#include "Projectile.h"
#include "EnemyManager.h"
#include "Sprite.h"
#include <fstream>
#include <assert.h>


Beam::Beam()
	:AvatarProjectile(Vector2f{600,600}, 3, "Resources/Projectiles/Beam/Shot.wav", "Resources/Projectiles/Beam/Impact.wav")
	
{
	std::ifstream spriteInfo{ "Resources/Projectiles/Beam/SpriteInfo.txt" };
	assert(spriteInfo);
	InitSprites(spriteInfo);
}


