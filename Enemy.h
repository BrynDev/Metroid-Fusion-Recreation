#pragma once
#include "Vector2f.h"
#include "GameInfo.h"
#include "structs.h"
#include <fstream>
#include <unordered_map>

class Sprite;
class Avatar;
class AvatarProjectile;
class Parasite;
class SoundEffect;

class Enemy
{
public:
	Enemy(const int contactDamage, const int health, const float startHorSpeed, const Point2f& startPos, const bool isFlipped, const int detectionRange, const bool canRespawn,Parasite* pParasite);
	virtual ~Enemy();

	virtual void Draw() const = 0;
	virtual void Update(float elapsedSec, GameInfo& gameInfo) = 0;
	bool IsDead() const;
	bool IsDying() const;
	void HandleAvatarContact(Avatar* pAvatar) const;
	Rectf GetShape() const;
	virtual void HandleHit(const AvatarProjectile* pAvProj);
	bool WasDamageTakenFromHit();
	void Reset();
	void ChangeEffectVolume(const int newVolume);
protected:
	std::unordered_map<int,Sprite*> m_pSprites;
	std::unordered_map<int, SoundEffect*> m_pSounds;
	const int m_ContactDamage;
	const int m_MaxHealth;
	const int m_DetectionRange;
	int m_Health;
	bool m_IsDead;
	bool m_IsDying;
	bool m_IsFlipped;
	bool m_IsFlippedAtStart;
	bool m_TargetFound;
	bool m_IsFlashing;
	bool m_WasDamageTaken;
	const bool m_CanRespawn;
	float m_AccuFlashingSecs;
	const float m_MaxFlashingSecs;
	Vector2f m_Velocity;
	const Vector2f m_StartVelocity;
	Rectf m_Shape;
	Point2f m_StartPos;
	Parasite* m_pParasite;


	Point2f GetCenter() const;
	virtual void ResetUniqueDataMembers() = 0;
	void ScaleShape(const float scale, const float width, const float height);
	void DetectAvatar(const Point2f& avatarCenter);
	void HandleFlashingTimer(float elapsedSec);
	void HandleDeath();
	void InitSprites(std::ifstream& spriteInfo);
};

