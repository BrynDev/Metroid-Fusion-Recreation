#pragma once
#include "Projectile.h"
#include "structs.h"

class Avatar;

class EnemyProjectile : public Projectile
{
public:
	EnemyProjectile(const Vector2f& velocity, const int damage);
	virtual ~EnemyProjectile() = default;

	virtual void Draw() const override;
	virtual void Update(float elapsedSec, const Level* pLevel) override;
	void Activate(const Point2f& startPos, const bool isFlipped);
	void HandleAvatarHit(Avatar* pAvatar);
protected:
	virtual void SetShape() override;
	virtual void SetImpactShape() override;
	
};

