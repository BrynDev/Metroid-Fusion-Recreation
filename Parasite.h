#pragma once
#include <vector>
#include "Vector2f.h"
#include <fstream>

class Sprite;
class Avatar;
class SoundEffect;

class Parasite
{
public:
	Parasite(const int healthRestore, const int ammoRestore);
	virtual ~Parasite();
	Parasite(const Parasite& other) = delete;
	Parasite& operator=(const Parasite& rhs) = delete;
	Parasite(Parasite&& other) = delete;
	Parasite& operator=(Parasite&& rhs) = delete;

	void Update(float elapsedSec, Avatar* pAvatar);
	void Draw() const;
	void Activate(const Point2f& center);
	void Reset();
	void ChangeEffectVolume(const int newVolume);
protected:
	Sprite* m_pSprite;
	const int m_HealthRestore;
	const int m_AmmoRestore;
	const int m_MaxSpeed;
	const int m_Acceleration;
	const float m_VertSpeed;
	const float m_HorSpeed;
	const int m_MaxIdleSecs;
	float m_AccuIdleSecs;
	bool m_IsActive;
	bool m_GoingRight;
	bool m_GoingLeft;
	float m_ScaleFactor;
	bool m_HasCollided;
	Rectf m_Shape;
	Vector2f m_Velocity;
	SoundEffect* m_pAbsorbSound;

	void SetShape(const Point2f& avatarCenter);
	void UpdatePos(float elapsedSec);
	void InitSprite(std::ifstream& spriteInfo);
};

