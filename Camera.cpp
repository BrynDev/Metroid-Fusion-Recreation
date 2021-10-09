#include "pch.h"
#include "Camera.h"
#include "utils.h"
#include <algorithm>
#include <fstream>
#include <assert.h>


Camera::Camera(const float width, const float height)
	:m_Width{width}
	, m_Height{height}
	, m_LevelBoundaries{0.f, 0.f, 0.f, 0.f}

{
}

void Camera::SetLevelBoundaries(const Rectf& levelBoundaries, const std::pair<std::vector<Point2f>,float>& doorInfo)
{
	m_LevelBoundaries.bottom = levelBoundaries.bottom;
	m_LevelBoundaries.left = levelBoundaries.left;
	m_LevelBoundaries.width = levelBoundaries.width;
	m_LevelBoundaries.height = levelBoundaries.height;
	m_DoorPointsVect = doorInfo.first;
	m_DoorHeightAdjust = doorInfo.second / 2.f;
	SetAnchorPoints();
}

void Camera::SetAnchorPoints()
{
	SetVertAnchorPoints();
	SetHorAnchorPoints();
	
}

void Camera::SetHorAnchorPoints()
{
	std::ifstream anchorPoints{ "Resources/Camera/HorAnchorPoints.txt" };
	assert(anchorPoints);
	std::string xPos{};
	std::string yPos{};
	while (!anchorPoints.eof())
	{
		anchorPoints >> xPos;
		anchorPoints >> yPos;
		m_HorAnchorPointsVect.push_back(Point2f{ std::stof(xPos), std::stof(yPos) });
	}
}
void Camera::SetVertAnchorPoints()
{
	std::ifstream anchorPoints{ "Resources/Camera/VertAnchorPoints.txt" };
	assert(anchorPoints);
	std::string xPos{};
	std::string yPos{};
	while (!anchorPoints.eof())
	{
		anchorPoints >> xPos;
		anchorPoints >> yPos;
		m_VertAnchorPointsVect.push_back(Point2f{ std::stof(xPos), std::stof(yPos) });
	}
}

void Camera::Transform(const Rectf& target) const
{
	Point2f bottomLeft{ Track(target) };
	Clamp(bottomLeft);
	glTranslatef(-bottomLeft.x, -bottomLeft.y, 0);
	
}

Point2f Camera::Track(const Rectf& target) const
{
	Point2f trackBottomLeft{ target.left + target.width / 2.f - m_Width / 2.f, target.bottom + target.height / 2.f - m_Height / 2.f };
	std::for_each(m_DoorPointsVect.cbegin(), m_DoorPointsVect.cend(), [target,&trackBottomLeft,this](const Point2f& doorPoint) {
		if (IsAvatarNearDoor(doorPoint, target))
		{
			if ((target.left < doorPoint.x + m_Width / 2.f) && (target.left > doorPoint.x)) //right side of a door
			{
				trackBottomLeft.x = doorPoint.x;
				
			}
			else if ((target.left + target.width > doorPoint.x - m_Width / 2.f) && (target.left + target.width < doorPoint.x)) //left side of a door
			{
				trackBottomLeft.x = doorPoint.x - m_Width;
				
			}

			if ((target.bottom < doorPoint.y + m_Height / 2.f) && (target.bottom + target.height >= doorPoint.y - m_Height / 8.f)) //above a door
			{
				trackBottomLeft.y = doorPoint.y - m_Height / 2.f + m_DoorHeightAdjust;
				
			}

			/*if ((target.bottom < doorPoint.y + m_Height / 2.f) && (target.bottom >= doorPoint.y - m_Height / 2.f)) //above a door
			{
				trackBottomLeft.y = doorPoint.y - m_Height / 2.f;			
			}
			if (IsHorClampOK(doorPoint, target))
			{
				if ((target.left < doorPoint.x + 3*m_Width / 4.f) && (target.left > doorPoint.x)) //right side of a door
				{
					trackBottomLeft.x = doorPoint.x;
					return true;
				}
				else if ((target.left + target.width > doorPoint.x - m_Width / 2.f) && (target.left + target.width < doorPoint.x)) //left side of a door
				{
					trackBottomLeft.x = doorPoint.x - m_Width;
					return true;
				}
			}*/
		}
		//return wasFound;
		
	});
	return trackBottomLeft;
	
	
}
void Camera::Clamp(Point2f& bottomLeftPos) const
{
	ClampAnchorPoints(bottomLeftPos);
	ClampLevelBoundaries(bottomLeftPos);
}

void Camera::ClampAnchorPoints(Point2f& bottomLeftPos) const
{
	ClampVertAnchorPoints(bottomLeftPos);
	ClampHorAnchorPoints(bottomLeftPos);
}
void Camera::ClampVertAnchorPoints(Point2f& bottomLeftPos) const
{
	std::any_of(m_VertAnchorPointsVect.cbegin(), m_VertAnchorPointsVect.cend(), [&bottomLeftPos, this](const Point2f& anchorPoint) {
		if (((anchorPoint.x > bottomLeftPos.x) && (anchorPoint.x < bottomLeftPos.x + m_Width)) && ((anchorPoint.y > bottomLeftPos.y) && (anchorPoint.y < bottomLeftPos.y + m_Height)))
		{
			bottomLeftPos.y = anchorPoint.y - m_Height / 2.f;
			return true;
		}
		return false;
	});
}
void Camera::ClampHorAnchorPoints(Point2f& bottomLeftPos) const
{
	std::any_of(m_HorAnchorPointsVect.cbegin(), m_HorAnchorPointsVect.cend(), [&bottomLeftPos, this](const Point2f& anchorPoint) {
		if (((anchorPoint.x > bottomLeftPos.x) && (anchorPoint.x < bottomLeftPos.x + m_Width)) && ((anchorPoint.y > bottomLeftPos.y) && (anchorPoint.y < bottomLeftPos.y + m_Height)))
		{
			bottomLeftPos.x = anchorPoint.x - m_Width / 2.f;
			return true;
		}
		return false;
	});
}

void Camera::ClampLevelBoundaries(Point2f& bottomLeftPos) const
{
	if (bottomLeftPos.x < m_LevelBoundaries.left)
	{
		bottomLeftPos.x = m_LevelBoundaries.left;
	}
	else if (bottomLeftPos.x + m_Width > m_LevelBoundaries.left + m_LevelBoundaries.width)
	{
		bottomLeftPos.x = m_LevelBoundaries.left + m_LevelBoundaries.width - m_Width;
	}

	if (bottomLeftPos.y < m_LevelBoundaries.bottom)
	{
		bottomLeftPos.y = m_LevelBoundaries.bottom;
	}
	else if (bottomLeftPos.y + m_Height > m_LevelBoundaries.bottom + m_LevelBoundaries.height)
	{
		bottomLeftPos.y = m_LevelBoundaries.bottom + m_LevelBoundaries.height - m_Height;
	}
}

bool Camera::IsAvatarNearDoor(const Point2f& doorPos, const Rectf& target) const
{
	float widthCheck{ m_Width };
	float heightCheck{ m_Height*1.5f };
	if (((target.left <= doorPos.x + widthCheck) && (target.left >= doorPos.x - widthCheck)))
	{
		if (((target.bottom <= doorPos.y + heightCheck) && (target.bottom >= doorPos.y - heightCheck)))
		{
			return true;
		}
	}
	return false;
}

