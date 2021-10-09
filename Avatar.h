#pragma once

#include "Vector2f.h"
#include <unordered_map>

class Level;
class ProjectileManager; 
class Sprite;
class HUD;
class SoundEffect;

class Avatar
{
public:
	Avatar(Point2f hudPos);
	~Avatar();
	Avatar(const Avatar& other) = delete;
	Avatar& operator=(const Avatar& rhs) = delete;
	Avatar(Avatar&& other) = delete;
	Avatar& operator=(Avatar&& rhs) = delete;

	void Update(float elapsedSec, const Level* pLevel);
	void Draw() const;

	Rectf GetShape() const;
	Point2f GetCenter() const;
	Rectf GetCollisionShape() const;
	void Shoot(ProjectileManager* pProjectileManager);
	void ToggleMissileLauncher();
	void HandleHit(const int damage);
	bool HasGoneThroughDoor();
	bool IsGameOver() const;
	void DrawHUD() const;
	void RestoreStats(const int healthRestore, const int ammoRestore);
	void HandleMissilePowerUp();
	bool IsImmune() const;
	void ChangeEffectVolume(const int newEffectVolume);
	bool IsDying() const;

private:

	HUD* m_pHud;
	enum class MovementState
	{
		idle, moving, crouching, knockback
	};

	enum class AimingState
	{
		straight, diagUp, diagDown, up, down
	};
	
	Rectf m_Shape;
	Rectf m_CollisionShape;
	const float m_HorSpeed;
	const float m_JumpSpeed;
	Vector2f m_Velocity;
	const Vector2f m_Acceleration;
	MovementState m_MovementState;
	AimingState m_AimingState;

	std::unordered_map<int, SoundEffect*> m_pSounds;

	bool m_IsOnGround;
	bool m_IsJumping;
	bool m_IsFlipped;
	bool m_IsImmune;
	bool m_IsDying;
	bool m_IsDead;
	bool m_IsGameOver;
	bool m_IsDrawOK;
	bool m_HasGoneThroughDoor;
	bool m_HasMissileWeapon;
	bool m_IsMissileWeaponActive; 
	bool m_IsFlashing;

	std::unordered_map<int,Sprite*> m_pSprites;
	std::unordered_map<int, Sprite*>::iterator m_CurrentSpriteIt;
	float m_JumpStartHeight;
	const float m_MaxJumpHeight;

	int m_NrSpritesPerSet;
	int m_JumpSpriteNr;
	int m_VertAimSpriteNr;
	int m_MissileSpriteNr;
	int m_SwitchSoundNr;
	int m_DamageSoundsNr;
	int m_MaxHealth;
	int m_CurrentHealth;
	int m_AccuImmuneFrames;
	int m_MaxMissileAmmo;
	int m_CurrentMissileAmmo;
	const int m_Scale;
	float m_AccuImmuneSecs;
	int m_DyingAnimationCounter;
	const float m_MaxFlashingSecs;
	float m_AccuFlashingSecs;
	
	//Helper functions
	void InitStandardSprites(int& keyInt);
	void InitMissileSprites(int& keyInt);
	void InitMiscSprites(int& keyInt);
	void InitSounds();
	void UpdateAvatar(float elapsedSec);
	void HandleKeyPress(float elapsedSec);
	void UpdateShapes();
	void InitCollisionShape();
	void InitSprites();
	void DetermineSprite();
	void StartKnockback();
	void UpdatePos(float elapsedSec);
	void UpdateDeath(float elapsedSec);
	void HandleImmuneTimer(float elapsedSec);
	void HandleFlashingTimer(float elapsedSec);
	void HandleTimers(float elapsedSec);
	void HandleDeath();
	Point2f DetermineWeaponPos();
};

