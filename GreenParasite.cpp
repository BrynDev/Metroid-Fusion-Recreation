#include "pch.h"
#include "GreenParasite.h"
#include "Sprite.h"
#include <fstream>
#include <assert.h>


GreenParasite::GreenParasite()
	:Parasite(0,2)
{
	std::ifstream spriteInfo{ "Resources/Parasite/GreenParasite/SpriteInfo.txt" };
	assert(spriteInfo);
	
	InitSprite(spriteInfo);
}
