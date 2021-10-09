#pragma once

#include "Enemy.h"

class Hornad : public Enemy
{
public:
	Hornad(const Point2f& startPos, const bool isFlipped, Parasite* pParasite);
	virtual ~Hornad() = default;

	Hornad(const Hornad& other) = delete;
	Hornad& operator=(const Hornad& rhs) = delete;
	Hornad(Hornad&& other) = delete;
	Hornad& operator=(Hornad&& rhs) = delete;

	virtual void Draw() const override;
	virtual void Update(float elapsedSec, GameInfo& gameInfo) override;
private:
	enum class EnemyState
	{
		idle, moving, turning, shooting, dying
	};
	EnemyState m_State;
	int m_IdleAnimCounter;
	const int m_IdleAnimCountMax;
	int m_NrJumps;
	const int m_MaxNrJumps;
	bool m_IsJumping;
	bool m_IsOnGround;
	bool m_IsTurnOK;
	bool m_HasShot;
	const int m_JumpSpeed;
	const Vector2f m_Acceleration;

	virtual void ResetUniqueDataMembers() override;
	void UpdatePos(float elapsedSec);
	void HandleCollision(const Level* pLevel);
	void StartJump();
	void HandleIdleBehavior();
	void HandleMoveBehavior(float elapsedSec);
};

