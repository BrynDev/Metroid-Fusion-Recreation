#include "pch.h"
#include "Missile.h"
#include "AvatarProjectile.h"
#include <fstream>
#include <assert.h>

Missile::Missile()
	:AvatarProjectile(Vector2f(480,480), 10, "Resources/Projectiles/Missile/Shot.wav", "Resources/Projectiles/Missile/Impact.wav")
{
	std::ifstream spriteInfo{ "Resources/Projectiles/Missile/SpriteInfo.txt" };
	assert(spriteInfo);
	InitSprites(spriteInfo);
}
