#pragma once
#include "structs.h"
#include "Vector2f.h"
#include <Vector>
#include "Texture.h"
#include "utils.h"
#include "GameInfo.h"

class DoorHandler;

class Level
{
public:
	Level();
	Level(const Level& other) = delete;
	Level& operator=(const Level& rhs) = delete;
	Level(Level&& other) = delete;
	Level& operator=(Level&& rhs) = delete;
	~Level();

	void DrawBackground() const;
	void DrawForeground() const;
	void Update(float elapsedSec, GameInfo& info);
	void HandleAvatarCollision(Rectf& actorShape, const Rectf& collisionShape, Vector2f& actorVelocity, bool& isJumping, bool& hasGoneThroughDoor) const;
	bool CheckHorEnemyCollision(Rectf& shape, utils::HitInfo& hitInfo) const;
	bool CheckVertEnemyCollision(Rectf& shape, utils::HitInfo& hitInfo) const;
	bool CheckProjectileCollision(const Rectf& projectileShape, const Vector2f& projectileVelocity) const;
	bool IsOnGround(const Rectf& actorShape) const;
	bool IsEnemyOnGround(const Rectf& actorShape) const;
	Rectf GetBoundaries() const;
	std::pair<std::vector<Point2f>,float> GetDoorInfo() const;
	void ChangeEffectVolume(const int newVolume);
private:
	std::vector<std::vector<Point2f>> m_Vertices;
	std::vector<std::vector<Point2f>> m_EnemyVertices;
	const Texture m_BackgroundTexture;
	const Rectf m_Boundaries;
	
	bool HeadRaycast(const Rectf& actorShape, utils::HitInfo& hitInfo) const;
	bool FeetRaycast(const Rectf& actorShape, utils::HitInfo& hitInfo) const;
	bool EnemyHeadRaycast(const Rectf& actorShape, utils::HitInfo& hitInfo) const;
	bool EnemyFeetRaycast(const Rectf& actorShape, utils::HitInfo& hitInfo) const;
	bool HorRaycast(const Rectf& actorShape, utils::HitInfo& hitInfo) const;
	bool EnemyHorRaycast(const Rectf& actorShape, utils::HitInfo& hitInfo) const;

	DoorHandler* m_pDoorHandler;

};

