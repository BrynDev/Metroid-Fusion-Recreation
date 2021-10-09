#include "pch.h"
#include "RedParasite.h"
#include <assert.h>


RedParasite::RedParasite()
	:Parasite(100,10)
{
	std::ifstream spriteInfo{ "Resources/Parasite/RedParasite/SpriteInfo.txt" };
	assert(spriteInfo);

	InitSprite(spriteInfo);
}
