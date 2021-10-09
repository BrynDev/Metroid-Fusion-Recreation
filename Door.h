#pragma once
#include <vector>
#include "Structs.h"

class Sprite;
class Texture;

class Door
{
public:
	Door(Texture* pDoorTunnelTexture, const Point2f& pos, bool isLocked);
	~Door();
	Door(const Door& other) = delete;
	Door& operator=(const Door& rhs) = delete;
	Door(Door&& other) = delete;
	Door& operator=(Door&& rhs) = delete;

	void Draw() const;
	void Update(float elapsedSec);
	bool IsLocked() const;
	bool IsOpen() const;
	std::vector<Point2f> GetVertices() const;
	Rectf GetFrontDoorShape() const;
	Rectf GetBackDoorShape() const;
	float GetHeight() const;
	Point2f GetPos() const;
	void OpenDoor();
	void CloseDoor();
private:
	bool m_IsLocked;
	bool m_IsOpen;
	Sprite* m_pFrontSprite;
	Texture* m_pTunnelTexture;
	const Point2f m_Pos;
	const float m_TunnelTextureHeight;
	const float m_TunnelTextureWidth;
	float m_DoorWidth;
	float m_DoorHeight;
	float m_Scale;
	std::vector<Point2f> m_Vertices;
	Rectf m_FrontDoorShape;
	Rectf m_BackDoorShape;

	void DrawMirroredDoor() const;
	void InitDoorVertices();
	
};

