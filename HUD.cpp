#include "pch.h"
#include "HUD.h"
#include "structs.h"
#include "Texture.h"
#include "Avatar.h"


HUD::HUD(const Point2f& pos)
	:m_Pos{pos}
	, m_pEnergyTexture{ new Texture{"Resources/HUD/EnergyText.png"} }
	,m_FontPath{ "Resources/HUD/Metroid-Fusion.ttf" }
	, m_HealthFontSize{20}
	, m_AmmoFontSize{15}
	, m_FontColor{1,1,1,1}
	, m_MissileTexturePos{pos.x + 150, pos.y}
	, m_pAmmoValueText{ nullptr }
	,m_pHealthValueText{nullptr}
	, m_IsMissileDrawOK{false}
{
	int startAmmo{ 10 };
	m_pAmmoValueText = new Texture{ std::to_string(startAmmo), m_FontPath, m_AmmoFontSize, m_FontColor };
	m_HealthNumbersPos = Point2f(m_Pos.x + m_pEnergyTexture->GetWidth() / 2.f, m_Pos.y - m_HealthFontSize);

	m_pMissileTextures.push_back(new Texture("Resources/HUD/Missile_Deactivated.png"));
	m_pMissileTextures.push_back(new Texture("Resources/HUD/Missile_Activated.png"));

	m_AmmoNumbersPos = Point2f(m_MissileTexturePos.x + m_pMissileTextures[0]->GetWidth() / 2.f, m_HealthNumbersPos.y);
	
}

HUD::~HUD()
{
	delete m_pEnergyTexture;
	delete m_pHealthValueText;
	m_pHealthValueText = nullptr;
	delete m_pAmmoValueText;
	m_pAmmoValueText = nullptr;
	for (Texture* pTexture : m_pMissileTextures)
	{
		delete pTexture;
	}
}

void HUD::Draw() const
{
	m_pEnergyTexture->Draw(m_Pos);
	m_pHealthValueText->Draw(m_HealthNumbersPos);

	if (m_IsMissileDrawOK)
	{
		m_pMissileTextures[int(m_IsMissileWeaponActive)]->Draw(m_MissileTexturePos);
		m_pAmmoValueText->Draw(m_AmmoNumbersPos);
	}	
}

void HUD::SetHealthDisplay(const int health)
{
	if (m_pHealthValueText != nullptr)
	{
		delete m_pHealthValueText;
	}
	
	m_pHealthValueText = new Texture(std::to_string(health), m_FontPath, m_HealthFontSize, m_FontColor);
}
void HUD::SetAmmoDisplay(const int ammo)
{
	if (m_pAmmoValueText != nullptr)
	{
		delete m_pAmmoValueText;
	}
	
	m_pAmmoValueText = new Texture(std::to_string(ammo), m_FontPath, m_AmmoFontSize, m_FontColor);
}

void HUD::ToggleMissileWeaponActive()
{
	m_IsMissileWeaponActive = !m_IsMissileWeaponActive;
}

void HUD::SetMissileDrawOK()
{
	m_IsMissileDrawOK = true;
}