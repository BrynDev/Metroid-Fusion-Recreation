#pragma once

#include "Vector2f.h"
#include <unordered_map>

class Level;
class Sprite;
class EnemyManager;

class Projectile
{
public:
	Projectile(const Vector2f& velocity, const int damage);
	virtual ~Projectile();
	virtual void Draw() const = 0;
	virtual void Update(float elapsedSec, const Level* pLevel) = 0;
	
	bool IsActive() const;
	bool HasHit() const;
	void ToggleActive();
	
	void UpdatePos(float elapsedSec);
	
	Rectf GetShape() const;
	int GetDamage() const;
protected:
	const Vector2f m_Velocity;
	Vector2f m_ActiveVelocity;
	Rectf m_Shape;
	std::unordered_map<int,Sprite*> m_pSprites;
	const int m_Damage;

	bool m_IsActive;
	bool m_HasHit;
	bool m_IsHorFlipped;
	bool m_IsVertFlipped;
	virtual void SetShape() = 0;
	void SetPos(const Point2f& pos);
	void SetActiveVelocity(const bool isTravellingLeft, const bool isTravellingRight, const bool isTravellingUp, const bool isTravellingDown);
	virtual void SetImpactShape() = 0;
	void InitSprites(std::ifstream& spriteInfo);
};

