#include "pch.h"
#include "DoorHandler.h"
#include "Door.h"
#include "Sprite.h"
#include "Texture.h"
#include "utils.h"
#include <algorithm>
#include <fstream>
#include <assert.h>
#include "SoundEffect.h"


DoorHandler::DoorHandler()
	: m_pDoorTunnelTexture{ new Texture{"Resources/Door/Door_Tunnel.png"} }
	, m_pDoorOpeningSound{ new SoundEffect{"Resources/Door/Opening.wav"} }
{
	std::ifstream doorInfo{ "Resources/Door/DoorInfo.txt" };
	assert(doorInfo);
	std::string xPos{};
	std::string yPos{};
	bool isLocked{};
	while (!doorInfo.eof())
	{	
		doorInfo >> xPos;
		doorInfo >> yPos;
		doorInfo >> std::boolalpha >> isLocked;
		m_pDoors.push_back(new Door{ m_pDoorTunnelTexture, Point2f(std::stof(xPos),std::stof(yPos)),isLocked});
	}
}


DoorHandler::~DoorHandler()
{
	std::for_each(m_pDoors.begin(), m_pDoors.end(), [](Door* pDoor) {delete pDoor; });
	delete m_pDoorOpeningSound;
	delete m_pDoorTunnelTexture;
}

void DoorHandler::Draw() const
{
	std::for_each(m_pDoors.begin(), m_pDoors.end(), [](Door* pDoor) { pDoor->Draw(); });
}

void DoorHandler::Update(float elapsedSec, GameInfo& gameInfo)
{
	std::for_each(m_pDoors.begin(), m_pDoors.end(), [elapsedSec](Door* pDoor) {pDoor->Update(elapsedSec); });
}

bool DoorHandler::CheckActorCollision(const Point2f& raycastPoint1, const Point2f& raycastPoint2, utils::HitInfo& hitInfo) const
{
	return std::any_of(m_pDoors.cbegin(), m_pDoors.cend(), [raycastPoint1,raycastPoint2,&hitInfo](const Door* pDoor) {
		if (!pDoor->IsOpen())
		{
			if (utils::Raycast(pDoor->GetVertices(), raycastPoint1, raycastPoint2, hitInfo))
			{
				return true;
			}
		}
		return false;
	});
}

bool DoorHandler::CheckProjectileCollision(const Rectf& projectileShape) const
{
	return std::any_of(m_pDoors.cbegin(), m_pDoors.cend(), [projectileShape,this](Door* pDoor) {
		Rectf frontDoorShape{ pDoor->GetFrontDoorShape() };
		if (!pDoor->IsOpen())
		{
			if (utils::IsOverlapping(projectileShape, frontDoorShape))
			{
				if (!pDoor->IsLocked())
				{
					pDoor->OpenDoor();
					m_pDoorOpeningSound->Play(0);
				}
				return true;
			}
			else if (utils::IsOverlapping(projectileShape, pDoor->GetBackDoorShape()))
			{
				if (!pDoor->IsLocked())
				{
					pDoor->OpenDoor();
					m_pDoorOpeningSound->Play(0);
				}
				return true;
			}
		}
		else
		{
			Point2f pos{ pDoor->GetPos() };
			if (utils::IsOverlapping(projectileShape, Rectf(pos.x, pos.y, 0, frontDoorShape.height))) //make sure projectiles don't travel to different rooms
			{
				return true;
			}
		}
		return false;
	});
}

bool DoorHandler::IsAvatarInDoor(const Rectf& actorShape)
{
	const int lineHeight{ 40 };
	return std::any_of(m_pDoors.cbegin(), m_pDoors.cend(), [actorShape, lineHeight,this](const Door* pDoor) {
		if (pDoor->IsOpen())
		{
			Point2f middleDoorPos{ pDoor->GetPos() };
			if (utils::IsOverlapping(middleDoorPos, Point2f(middleDoorPos.x, middleDoorPos.y + lineHeight), actorShape))
			{
				CloseAllDoors();
				return true;
			}
		}
		return false;
	});
}

void DoorHandler::CloseAllDoors()
{
	std::for_each(m_pDoors.cbegin(), m_pDoors.cend(), [](Door* pDoor) {
		if (pDoor->IsOpen())
		{
			pDoor->CloseDoor();
		}
	});
}

std::pair<std::vector<Point2f>, float> DoorHandler::GetDoorInfo() const
{
	std::vector<Point2f> doorPosVect;
	std::for_each(m_pDoors.cbegin(), m_pDoors.cend(), [&doorPosVect](Door* pDoor) {
		doorPosVect.push_back(pDoor->GetPos());
		});
	
	return std::make_pair(doorPosVect, m_pDoors[0]->GetHeight());
}

void DoorHandler::ChangeEffectVolume(const int newVolume)
{
	m_pDoorOpeningSound->SetVolume(newVolume);
}