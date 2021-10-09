#include "pch.h"
#include "MissilePowerUp.h"
#include <fstream>
#include <assert.h>
#include "Sprite.h"


MissilePowerUp::MissilePowerUp(const Point2f& pos)
	:PowerUp(pos)
{
	std::ifstream spriteInfo{ "Resources/PowerUp/SpriteInfo.txt" };
	assert(spriteInfo);
	InitSprite(spriteInfo);
	const int scale{ 2 };
	m_Shape.width = m_pSprite->GetFrameWidth() * scale;
	m_Shape.height = m_pSprite->GetFrameHeight() * scale;
}
