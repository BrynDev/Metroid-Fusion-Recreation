#include "pch.h"
#include "PowerUp.h"
#include "Sprite.h"
#include "utils.h"
#include <fstream>
#include <assert.h>
#include "SoundEffect.h"

PowerUp::PowerUp(const Point2f& pos)
	:m_WasTaken{false}
	, m_Shape{pos.x, pos.y, 0, 0}
	, m_pPickupSound{ new SoundEffect{"Resources/PowerUp/PowerUpGet.wav"} }
{
}

PowerUp::~PowerUp()
{
	delete m_pSprite;
	delete m_pPickupSound;
}

void PowerUp::InitSprite(std::ifstream& spriteInfo)
{
	std::string filePath{};
	std::string frameSec{};
	std::string nrCols{};
	std::getline(spriteInfo, filePath, ',');
	std::getline(spriteInfo, frameSec, ',');
	std::getline(spriteInfo, nrCols);
	m_pSprite = new Sprite{ filePath, std::stof(frameSec), std::stoi(nrCols) };
}

void PowerUp::Draw() const
{
	m_pSprite->Draw(m_Shape,false);
}

void PowerUp::Update(float elapsedSec, Avatar* pAvatar)
{
	m_pSprite->Update(elapsedSec);
	if (utils::IsOverlapping(pAvatar->GetShape(), m_Shape))
	{
		pAvatar->HandleMissilePowerUp();
		m_WasTaken = true;
		m_pPickupSound->Play(0);
	}
}

bool PowerUp::WasTaken() const
{
	return m_WasTaken;
}

void PowerUp::ChangeEffectVolume(const int newVolume)
{
	m_pPickupSound->SetVolume(newVolume);
}
