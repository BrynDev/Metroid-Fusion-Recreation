#pragma once
#include "Enemy.h"

class Zombie: public Enemy
{
public:
	Zombie(const Point2f& pos, const bool isFlipped, Parasite* pParasite);
	virtual ~Zombie() = default;

	virtual void Draw() const override;
	virtual void Update(float elapsedSec, GameInfo& gameInfo) override;
private:
	enum class EnemyState
	{
		moving, dying
	};
	virtual void ResetUniqueDataMembers() override;
	EnemyState m_State;
	void UpdatePos(float elapsedSec);
	void HandleCollision(const Level* pLevel);
};

