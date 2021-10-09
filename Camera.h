#pragma once
#include <Vector>
#include "Structs.h"

//Instead of changing boundaries in new rooms, do not track avatar if near door

class Camera
{
public:
	Camera(const float width, const float height);
	void SetLevelBoundaries(const Rectf& levelBoundaries, const std::pair<std::vector<Point2f>, float>& doorInfo);
	void Transform(const Rectf& target) const;
private:
	const float m_Width;
	const float m_Height;
	float m_DoorHeightAdjust;
	Rectf m_LevelBoundaries;
	std::vector<Point2f> m_DoorPointsVect;
	std::vector<Point2f> m_HorAnchorPointsVect;
	std::vector<Point2f> m_VertAnchorPointsVect;

	Point2f Track(const Rectf& target) const;
	void Clamp(Point2f& bottomLeftPos) const;
	void ClampLevelBoundaries(Point2f& bottomLeftPos) const;
	void ClampAnchorPoints(Point2f& bottomLeftPos) const;
	void ClampVertAnchorPoints(Point2f& bottomLeftPos) const;
	void ClampHorAnchorPoints(Point2f& bottomLeftPos) const;
	bool IsAvatarNearDoor(const Point2f& doorPos, const Rectf& target) const;
	void SetAnchorPoints();
	void SetHorAnchorPoints();
	void SetVertAnchorPoints();
};

