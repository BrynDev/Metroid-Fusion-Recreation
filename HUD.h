#pragma once
#include <vector>
#include "structs.h"

class Avatar;
class Texture;

class HUD
{
public:
	HUD(const Point2f& pos);
	~HUD();
	HUD(const HUD& other) = delete;
	HUD& operator=(const HUD& rhs) = delete;
	HUD(HUD&& other) = delete;
	HUD& operator=(HUD&& rhs) = delete;

	void Draw() const;
	void SetHealthDisplay(const int health);
	void SetAmmoDisplay(const int ammo);
	void ToggleMissileWeaponActive();
	void SetMissileDrawOK();
private:
	const Texture* m_pEnergyTexture;
	const Texture* m_pHealthValueText;
	const Texture* m_pAmmoValueText;
	std::vector<Texture*> m_pMissileTextures;
	const Point2f m_Pos;
	Point2f m_HealthNumbersPos;
	const Point2f m_MissileTexturePos;
	Point2f m_AmmoNumbersPos;
	const std::string m_FontPath;
	const Color4f m_FontColor;
	const int m_HealthFontSize;
	const int m_AmmoFontSize;
	bool m_IsMissileWeaponActive;
	bool m_IsMissileDrawOK;
};

