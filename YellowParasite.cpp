#include "pch.h"
#include "YellowParasite.h"
#include "Sprite.h"
#include <fstream>
#include <assert.h>

YellowParasite::YellowParasite()
	:Parasite(10,0)
{
	std::ifstream spriteInfo{ "Resources/Parasite/YellowParasite/SpriteInfo.txt" };
	assert(spriteInfo);

	InitSprite(spriteInfo);
}
