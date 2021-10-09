#pragma once
#include "Enemy.h"

class Geron : public Enemy
{
public:
	Geron(const Point2f& pos, Parasite* pParasite);
	virtual ~Geron() = default;

	virtual void HandleHit(const AvatarProjectile* pAvProj) override;
	virtual void Draw() const override;
	virtual void Update(float elapsedSec, GameInfo& info) override;
	virtual void ResetUniqueDataMembers() override;
};

