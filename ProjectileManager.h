#pragma once
#include <vector>
#include "Manager.h"

class Projectile;
class AvatarProjectile;
class EnemyProjectile;
class Beam;
class Sprite;



class ProjectileManager : public Manager
{
public:
	ProjectileManager();
	virtual ~ProjectileManager();
	ProjectileManager(const ProjectileManager& other) = delete;
	ProjectileManager& operator=(const ProjectileManager& rhs) = delete;
	ProjectileManager(ProjectileManager&& other) = delete;
	ProjectileManager& operator=(ProjectileManager&& rhs) = delete;

	void ActivateBeam(const Point2f& startPos, const bool isTravellingLeft, const bool isTravellingRight, const bool isTravellingUp, const bool isTravellingDown);
	void ActivateMissile(const Point2f& startPos, const bool isTravellingLeft, const bool isTravellingRight, const bool isTravellingUp, const bool isTravellingDown);
	void ActivateHornadShot(const Point2f& startPos, const bool isFlipped);
	virtual void Update(float elapsedSec, GameInfo& gameInfo) override;
	virtual void Draw() const override;
	void ChangeEffectVolume(const int newVolume);

private:
	std::vector<AvatarProjectile*> m_pAvatarProjectiles;
	std::vector<EnemyProjectile*> m_pEnemyProjectiles;
	std::vector<Projectile*> m_pProjectiles;
};

