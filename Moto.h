#pragma once
#include "Enemy.h"

class Level;
class AvatarProjectile;

class Moto : public Enemy
{
public:
	Moto(const Point2f& pos, const bool isFlipped, Parasite* pParasite);
	virtual ~Moto() = default;

	virtual void Draw() const override;
	virtual void Update(float elapsedSec, GameInfo& gameInfo);
	virtual void ResetUniqueDataMembers() override;
	virtual void HandleHit(const AvatarProjectile* pAvProj) override;
private:
	enum class EnemyState 
	{
		idle, walking, taunting,charging, braking, dying
	};
	EnemyState m_State;
	const float m_Acceleration;
	const float m_WalkSpeed;
	const float m_RunSpeed;
	const float m_BrakeDeceleration;
	const float m_MaxSpeed;
	int m_IdleCounter;
	float m_AccuWalkTime;
	int m_TauntCounter;
	bool m_IsTurnOK;
	bool m_IsDetectOK;

	void HandleWalkTimer(float elapsedSec);
	void HandleCollision(const Level* pLevel);
	void SetShapeSize();
	bool HasPassedAvatar(const Point2f& avatarCenter);
	void UpdatePos(float elapsedSec);
	void HandleIdleBehaviour();
	void HandleTauntingBehaviour();
	void HandleChargingBehaviour(float elapsedSec);
	void HandleBrakingBehaviour(float elapsedSec);
};

