#include "pch.h"
#include "Level.h"
#include "utils.h"
#include "SVGParser.h"
#include "Texture.h"
#include "DoorHandler.h"
#include "GameInfo.h"
#include <algorithm>

Level::Level()
	: m_BackgroundTexture{ "Resources/Level/Level_Complete4.png" }
	, m_Boundaries{ 0, 0, m_BackgroundTexture.GetWidth(), m_BackgroundTexture.GetHeight() }
	, m_pDoorHandler{new DoorHandler()}
{
	
	SVGParser::GetVerticesFromSvgFile("Resources/Level/Level_Complete4.svg", m_Vertices);
	SVGParser::GetVerticesFromSvgFile("Resources/Level/Level_EnemyCollision.svg", m_EnemyVertices);
}

Level::~Level()
{
	delete m_pDoorHandler;
}

void Level::DrawBackground() const
{
	m_BackgroundTexture.Draw();
}

void Level::DrawForeground() const
{
	m_pDoorHandler->Draw();
}

void Level::Update(float elapsedSec,GameInfo& info)
{
	m_pDoorHandler->Update(elapsedSec, info);
}

void Level::HandleAvatarCollision(Rectf& actorShape, const Rectf& collisionShape, Vector2f& actorVelocity, bool& isJumping, bool& hasGoneThroughDoor) const
{
	utils::HitInfo hitInfo;
	if (HorRaycast(collisionShape, hitInfo))
	{
   		if (actorVelocity.x <= 0)
		{
			actorShape.left = hitInfo.intersectPoint.x + 2;
			actorVelocity.x = 0.f;
		}
		else if (actorVelocity.x > 0)
		{
			actorShape.left = hitInfo.intersectPoint.x - collisionShape.width - 2;
			actorVelocity.x = 0.f;
		}
	}
	
	if (actorVelocity.y > 0)
	{
		if (HeadRaycast(collisionShape, hitInfo))
		{
			actorShape.bottom = hitInfo.intersectPoint.y - collisionShape.height - 10;
			actorVelocity.y = 0;
			isJumping = false;
		}    		
	}
	else if(actorVelocity.y <= 0)
	{
		if (FeetRaycast(collisionShape, hitInfo))
		{
			actorShape.bottom = hitInfo.intersectPoint.y;
			actorVelocity.y = 0;
		} 			
		
	}

	if (m_pDoorHandler->IsAvatarInDoor(actorShape))
	{
		const int moveDistance{ 150 };
		if (actorVelocity.x > 0)
		{
			actorShape.left += moveDistance;
		}
		else
		{
			actorShape.left -= moveDistance;
		}
		hasGoneThroughDoor = true;
	}

}


bool Level::CheckHorEnemyCollision(Rectf& shape, utils::HitInfo& hitInfo) const
{
	return (EnemyHorRaycast(shape,hitInfo));
}

bool Level::CheckVertEnemyCollision(Rectf& shape, utils::HitInfo& hitInfo) const
{
	if (EnemyFeetRaycast(shape, hitInfo) || EnemyHeadRaycast(shape, hitInfo))
	{
		return true;
	}
	else
	{
		return false;
	}
}

bool Level::IsOnGround(const Rectf& actorShape) const
{
	utils::HitInfo hitInfo;

	return FeetRaycast(actorShape, hitInfo);
	
}

bool Level::IsEnemyOnGround(const Rectf& actorShape) const
{
	utils::HitInfo hitInfo;

	return EnemyFeetRaycast(actorShape, hitInfo);

}

Rectf Level::GetBoundaries() const
{
	return m_Boundaries;
}

bool Level::HeadRaycast(const Rectf& actorShape, utils::HitInfo& hitInfo) const
{
	const float widthBuffer{ 4.f };
	Point2f actorLeft1{ actorShape.left + widthBuffer, actorShape.bottom + actorShape.height / 2.f };
	Point2f actorLeft2{ actorLeft1.x, actorShape.bottom + actorShape.height + 1 };
	Point2f actorRight1{ actorShape.left + actorShape.width - widthBuffer, actorLeft1.y };
	Point2f actorRight2{ actorRight1.x, actorLeft2.y };
	return std::any_of(m_Vertices.cbegin(), m_Vertices.cend(), [actorLeft1, actorLeft2, actorRight1, actorRight2, &hitInfo](const std::vector<Point2f>& vertices) {
		if (utils::Raycast(vertices, actorLeft1, actorLeft2, hitInfo))
		{
			return true;
		}
		if (utils::Raycast(vertices, actorRight1, actorRight2, hitInfo))
		{
			return true;
		}
		return false;
	}); 
}

bool Level::FeetRaycast(const Rectf& actorShape, utils::HitInfo& hitInfo) const
{
	const float widthBuffer{ 7.f };
	Point2f actorLeft1{ actorShape.left + widthBuffer, actorShape.bottom - 1 };
	Point2f actorLeft2{ actorLeft1.x, actorShape.bottom + actorShape.height /2.f };
	Point2f actorRight1{ actorShape.left + actorShape.width - widthBuffer, actorLeft1.y };
	Point2f actorRight2{ actorRight1.x, actorLeft2.y };
	return std::any_of(m_Vertices.cbegin(), m_Vertices.cend(), [actorLeft1, actorLeft2, actorRight1, actorRight2, widthBuffer,&hitInfo](const std::vector<Point2f>& vertices) {
		if (utils::Raycast(vertices, actorLeft1, actorLeft2, hitInfo))
		{
			return true;
		}
		if (utils::Raycast(vertices, actorRight1, actorRight2, hitInfo))
		{
			return true;
		}
		return false;
	});
}

bool Level::EnemyHeadRaycast(const Rectf& actorShape, utils::HitInfo& hitInfo) const
{
	const float widthBuffer{ 4.f };
	Point2f actorLeft1{ actorShape.left + widthBuffer, actorShape.bottom + actorShape.height / 2.f };
	Point2f actorLeft2{ actorLeft1.x, actorShape.bottom + actorShape.height + 1 };
	Point2f actorRight1{ actorShape.left + actorShape.width - widthBuffer, actorLeft1.y };
	Point2f actorRight2{ actorRight1.x, actorLeft2.y };
	return std::any_of(m_EnemyVertices.cbegin(), m_EnemyVertices.cend(), [actorLeft1, actorLeft2, actorRight1, actorRight2, &hitInfo](const std::vector<Point2f>& vertices) {
		if (utils::Raycast(vertices, actorLeft1, actorLeft2, hitInfo))
		{
			return true;
		}
		if (utils::Raycast(vertices, actorRight1, actorRight2, hitInfo))
		{
			return true;
		}
		return false;
	});
}

bool Level::EnemyFeetRaycast(const Rectf& actorShape, utils::HitInfo& hitInfo) const
{
	const float widthBuffer{ 7.f };
	Point2f actorLeft1{ actorShape.left + widthBuffer, actorShape.bottom - 2 };
	Point2f actorLeft2{ actorLeft1.x, actorShape.bottom + actorShape.height / 2.f };
	Point2f actorRight1{ actorShape.left + actorShape.width - widthBuffer, actorLeft1.y };
	Point2f actorRight2{ actorRight1.x, actorLeft2.y };
	return std::any_of(m_EnemyVertices.cbegin(), m_EnemyVertices.cend(), [actorLeft1, actorLeft2, actorRight1, actorRight2, widthBuffer, &hitInfo](const std::vector<Point2f>& vertices) {
		if (utils::Raycast(vertices, actorLeft1, actorLeft2, hitInfo))
		{
			return true;
		}
		if (utils::Raycast(vertices, actorRight1, actorRight2, hitInfo))
		{
			return true;
		}
		return false;
	});
}

bool Level::HorRaycast(const Rectf& actorShape, utils::HitInfo& hitInfo) const
{
	const float heightBuffer{ 8.f };
	Point2f actorMiddle1{ actorShape.left - 1, actorShape.bottom + actorShape.height / 2.f };
	Point2f actorMiddle2{ actorShape.left + actorShape.width + 1, actorMiddle1.y };
	Point2f actorTop1{ actorMiddle1.x, actorShape.bottom + actorShape.height - heightBuffer };
	Point2f actorTop2{ actorMiddle2.x, actorTop1.y };
	Point2f actorBottom1{ actorMiddle1.x, actorShape.bottom + heightBuffer };
	Point2f actorBottom2{ actorMiddle2.x, actorBottom1.y };

	return std::any_of(m_Vertices.cbegin(), m_Vertices.cend(), [actorMiddle1, actorMiddle2, actorTop1, actorTop2, actorBottom1, actorBottom2, heightBuffer, &hitInfo, this](const std::vector<Point2f>& vertices) {
		if (utils::Raycast(vertices, actorMiddle1, actorMiddle2, hitInfo) || m_pDoorHandler->CheckActorCollision(actorMiddle1, actorMiddle2, hitInfo))
		{
			return true;
		}
		if (utils::Raycast(vertices, actorTop1, actorTop2, hitInfo) || m_pDoorHandler->CheckActorCollision(actorTop1, actorTop2, hitInfo))
		{
			return true;
		}
		if (utils::Raycast(vertices, actorBottom1, actorBottom2, hitInfo) || m_pDoorHandler->CheckActorCollision(actorBottom1, actorBottom2, hitInfo))
		{
			return true;
		}
		return false;
	});
}

bool Level::EnemyHorRaycast(const Rectf& actorShape, utils::HitInfo& hitInfo) const
{
	const float heightBuffer{ 8.f };
	Point2f actorMiddle1{ actorShape.left - 1, actorShape.bottom + actorShape.height / 2.f };
	Point2f actorMiddle2{ actorShape.left + actorShape.width + 1, actorMiddle1.y };
	Point2f actorTop1{ actorMiddle1.x, actorShape.bottom + actorShape.height - heightBuffer };
	Point2f actorTop2{ actorMiddle2.x, actorTop1.y };
	Point2f actorBottom1{ actorMiddle1.x, actorShape.bottom + heightBuffer };
	Point2f actorBottom2{ actorMiddle2.x, actorBottom1.y };

	return std::any_of(m_EnemyVertices.cbegin(), m_EnemyVertices.cend(), [actorMiddle1, actorMiddle2, actorTop1, actorTop2, actorBottom1, actorBottom2, heightBuffer, &hitInfo, this](const std::vector<Point2f>& vertices) {
		if (utils::Raycast(vertices, actorMiddle1, actorMiddle2, hitInfo) || m_pDoorHandler->CheckActorCollision(actorMiddle1, actorMiddle2, hitInfo))
		{
			return true;
		}
		if (utils::Raycast(vertices, actorTop1, actorTop2, hitInfo) || m_pDoorHandler->CheckActorCollision(actorTop1, actorTop2, hitInfo))
		{
			return true;
		}
		if (utils::Raycast(vertices, actorBottom1, actorBottom2, hitInfo) || m_pDoorHandler->CheckActorCollision(actorBottom1, actorBottom2, hitInfo))
		{
			return true;
		}
		return false;
	});
}

bool Level::CheckProjectileCollision(const Rectf& projectileShape, const Vector2f& projectileVelocity) const
{
	utils::HitInfo hitInfo{};

	if (projectileVelocity.y != 0)
	{
		Point2f vertLine1{ projectileShape.left + projectileShape.width / 2.f, projectileShape.bottom };
		Point2f vertLine2{ vertLine1.x, projectileShape.bottom + projectileShape.height };
		for (size_t i{}; i < m_Vertices.size(); ++i)
		{
			if (utils::Raycast(m_Vertices[i], vertLine1, vertLine2, hitInfo))
			{
				return true;
			}
		}
	}
	if (projectileVelocity.x != 0)
	{
		Point2f horLine1{ projectileShape.left, projectileShape.bottom + projectileShape.height / 2.f };
		Point2f horLine2{ projectileShape.left + projectileShape.width / 2.f, horLine1.y };
		for (size_t i{}; i < m_Vertices.size(); ++i)
		{
			if (utils::Raycast(m_Vertices[i], horLine1, horLine2, hitInfo) || m_pDoorHandler->CheckProjectileCollision(projectileShape))
			{
				return true;
			}
		}
		
	}
	return false;

}

std::pair<std::vector<Point2f>, float> Level::GetDoorInfo() const
{
	return m_pDoorHandler->GetDoorInfo();
}

void Level::ChangeEffectVolume(const int newVolume)
{
	m_pDoorHandler->ChangeEffectVolume(newVolume);
}