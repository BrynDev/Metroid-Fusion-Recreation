 #pragma once
#include <vector>
#include "utils.h"
#include "Manager.h"

class Door;
class Sprite;
class Texture;
class SoundEffect;

class DoorHandler
{
public:
	DoorHandler();
	~DoorHandler();
	DoorHandler(const DoorHandler& other) = delete;
	DoorHandler& operator=(const DoorHandler& rhs) = delete;
	DoorHandler(DoorHandler&& other) = delete;
	DoorHandler& operator=(DoorHandler&& rhs) = delete;
	

	void Draw() const;
	void Update(float elapsedSec, GameInfo& gameInfo);
	bool CheckActorCollision(const Point2f& raycastPoint1, const Point2f& raycastPoint2, utils::HitInfo& hitInfo) const;
	bool CheckProjectileCollision(const Rectf& projectileShape) const;
	bool IsAvatarInDoor(const Rectf& actorShape);
	std::pair<std::vector<Point2f>, float> GetDoorInfo() const;
	void ChangeEffectVolume(const int newVolume);
private:
	std::vector<Door*> m_pDoors;
	SoundEffect* m_pDoorOpeningSound;
	Texture* m_pDoorTunnelTexture;
	void CloseAllDoors();
};

