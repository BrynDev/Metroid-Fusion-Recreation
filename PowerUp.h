#pragma once
#include "Avatar.h"

class Sprite;
class SoundEffect;

class PowerUp
{
public:
	PowerUp(const Point2f& pos);
	virtual ~PowerUp();
	virtual void Draw() const;
	virtual void Update(float elapsedSec, Avatar* pAvatar);

	bool WasTaken() const;
	void ChangeEffectVolume(const int newVolume);
protected:
	Sprite* m_pSprite;
	Rectf m_Shape;
	bool m_WasTaken;
	SoundEffect* m_pPickupSound;
	void InitSprite(std::ifstream& spriteInfo);
};

