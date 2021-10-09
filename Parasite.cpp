#include "pch.h"
#include "Parasite.h"
#include <algorithm>
#include "Sprite.h"
#include "Avatar.h"
#include "Utils.h"
#include "SoundEffect.h"

Parasite::Parasite(const int healthRestore, const int ammoRestore)
	:m_HasCollided{false}
	, m_ScaleFactor{2}
	, m_MaxSpeed{540}
	, m_Acceleration{350}
	, m_HorSpeed{200}
	, m_VertSpeed{220}
	, m_IsActive{false}
	, m_HealthRestore{healthRestore}
	, m_AmmoRestore{ammoRestore}
	, m_MaxIdleSecs{3}
	, m_pAbsorbSound{ new SoundEffect{"Resources/Parasite/Absorb.wav"} }
{
}


Parasite::~Parasite()
{
	delete m_pSprite;
	delete m_pAbsorbSound;
}

void Parasite::InitSprite(std::ifstream& spriteInfo)
{
	std::string filePath{};
	std::string frameSec{};
	std::string nrCols{};
	std::getline(spriteInfo, filePath, ',');
	std::getline(spriteInfo, frameSec, ',');
	std::getline(spriteInfo, nrCols);
	m_pSprite = new Sprite{ filePath, std::stof(frameSec), std::stoi(nrCols) };
	
}

void Parasite::Draw() const
{
	m_pSprite->Draw(m_Shape, false);
}

void Parasite::Update(float elapsedSec, Avatar* pAvatar)
{
	if (!m_IsActive)
	{
		return;
	}

	m_pSprite->Update(elapsedSec);
	const Point2f avatarCenter{ pAvatar->GetCenter() };
	if (m_HasCollided)
	{
		SetShape(avatarCenter);
		m_ScaleFactor -= 0.02f; 
		if (m_ScaleFactor <= 0)
		{
			m_IsActive = false;
		}
	}
	else
	{
		UpdatePos(elapsedSec);
		if (utils::IsOverlapping(m_Shape, pAvatar->GetShape()))
		{
			m_HasCollided = true;
			pAvatar->RestoreStats(m_HealthRestore, m_AmmoRestore);
			m_pAbsorbSound->Play(0);
		}
		else if (m_Shape.bottom >= avatarCenter.y + 400)
		{
			m_IsActive = false;
		}
	}
}

void Parasite::Activate(const Point2f& center)
{
	m_Shape.width = m_pSprite->GetFrameWidth() * m_ScaleFactor;
	m_Shape.height = m_pSprite->GetFrameHeight() * m_ScaleFactor;
	m_Shape.left = center.x - m_Shape.width / 2.f;
	m_Shape.bottom = center.y - m_Shape.height / 2.f;
	m_IsActive = true;
	m_Velocity.y = -m_VertSpeed;
	rand() % 2 == 0 ? m_Velocity.x = -m_HorSpeed : m_Velocity.x = m_HorSpeed;
	m_Velocity.x > 0 ? m_GoingLeft = true : m_GoingRight = true;
}

void Parasite::Reset()
{
	m_IsActive = false;
	m_HasCollided = false;
	m_ScaleFactor = 2.f;
	m_AccuIdleSecs = 0;
	m_GoingLeft = false;
	m_GoingRight = false;
}

void Parasite::SetShape(const Point2f& avatarCenter)
{
	m_Shape.width = m_pSprite->GetFrameWidth() * m_ScaleFactor;
	m_Shape.height = m_pSprite->GetFrameHeight() * m_ScaleFactor;
	m_Shape.left = avatarCenter.x - m_Shape.width / 2.f;
	m_Shape.bottom = avatarCenter.y - m_Shape.height / 2.f;
	
}

void Parasite::UpdatePos(float elapsedSec)
{
	m_AccuIdleSecs += elapsedSec;
	if (m_AccuIdleSecs >= m_MaxIdleSecs)
	{
		m_Velocity.y += m_Acceleration * elapsedSec;
		if (m_GoingLeft)
		{
			m_Velocity.x -= m_Acceleration * elapsedSec;
		}
		else if (m_GoingRight)
		{
			m_Velocity.x += m_Acceleration * elapsedSec;
		}

		m_Shape.bottom += m_Velocity.y * elapsedSec;
		m_Shape.left += m_Velocity.x * elapsedSec;
	}
}

void Parasite::ChangeEffectVolume(const int newVolume)
{
	m_pAbsorbSound->SetVolume(newVolume);
}