#pragma once

#include "Projectile.h"
#include "Sprite.h"

class SoundEffect;
class Level;

class AvatarProjectile : public Projectile
{
public:
	AvatarProjectile(const Vector2f& velocity, const int damage, const std::string& shootSoundPath, const std::string& impactSoundPath);
	virtual ~AvatarProjectile();

	virtual void Draw() const override;
	virtual void Update(float elapsedSec, const Level* pLevel) override;
	
	void HandleEnemyHit(const bool wasDamageDealt);
	void Activate(const Point2f& startPos, const bool isTravellingLeft, const bool isTravellingRight, const bool isTravellingUp, const bool isTravellingDown);
	float GetHorVelocity() const;
	void ChangeEffectVolume(const int newVolume);
protected:
	enum class TravelState
	{
		horizontal, diagonal, vertical
	};
	TravelState m_TravelState;
	enum class ImpactType
	{
		level = int(TravelState::vertical) + 1, enemy, enemyImmune
	};
	ImpactType m_ImpactType;
	std::unordered_map<int, SoundEffect*> m_pSounds;

	void SetTravelState();
	virtual void SetShape() override;
	virtual void SetImpactShape() override;
	void SetDataMembers(const Point2f& startPos, const bool isTravellingLeft, const bool isTravellingRight, const bool isTravellingUp, const bool isTravellingDown);
	void SetFlipped(const bool isTravellingLeft, const bool isTravellingDown);
};

