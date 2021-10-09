#include "pch.h"
#include "Door.h"
#include "Sprite.h"
#include "Texture.h"
#include <fstream>
#include <assert.h>


Door::Door(Texture* pDoorTunnelTexture, const Point2f& pos, bool isLocked)
	:m_IsOpen{false}
	, m_pTunnelTexture{pDoorTunnelTexture}
	, m_Pos{pos}
	, m_TunnelTextureWidth{pDoorTunnelTexture->GetWidth()}
	, m_TunnelTextureHeight{pDoorTunnelTexture->GetHeight()}
	, m_IsLocked{isLocked}
	, m_Scale{1}
{
	std::ifstream spriteInfo{ "Resources/Door/SpriteInfo.txt" };
	assert(spriteInfo);
	std::string filePath{};
	std::string frameSec{};
	std::string nrCols{};
	std::getline(spriteInfo, filePath, ',');
	std::getline(spriteInfo, frameSec, ',');
	std::getline(spriteInfo, nrCols);
	m_pFrontSprite = new Sprite{filePath, std::stof(frameSec), std::stoi(nrCols)} ;

	InitDoorVertices();
	
}

Door::~Door()
{
	delete m_pFrontSprite;
}

void Door::InitDoorVertices()
{
	m_DoorHeight = m_pFrontSprite->GetFrameHeight();
	m_DoorWidth = m_pFrontSprite->GetFrameWidth();
	Point2f doorFrontPos{ m_Pos.x + m_TunnelTextureWidth, m_Pos.y };

	//right facing side of the door
	m_Vertices.push_back(doorFrontPos);
	m_Vertices.push_back(Point2f(doorFrontPos.x + m_DoorWidth, doorFrontPos.y));
	m_Vertices.push_back(Point2f(doorFrontPos.x + m_DoorWidth, doorFrontPos.y + m_DoorHeight));
	m_Vertices.push_back(Point2f(doorFrontPos.x, doorFrontPos.y + m_DoorHeight));
	m_FrontDoorShape = Rectf(doorFrontPos.x, doorFrontPos.y, m_DoorWidth, m_DoorHeight);
	//left facing side of the door
	Point2f doorBackPos{ doorFrontPos.x - 2 * m_TunnelTextureWidth, doorFrontPos.y };
	m_Vertices.push_back(doorBackPos);
	m_Vertices.push_back(Point2f(doorBackPos.x - m_DoorWidth, doorBackPos.y));
	m_Vertices.push_back(Point2f(doorBackPos.x - m_DoorWidth, doorBackPos.y + m_DoorHeight));
	m_Vertices.push_back(Point2f(doorBackPos.x, doorBackPos.y + m_DoorHeight));
	m_BackDoorShape = Rectf(doorBackPos.x - m_DoorWidth, doorBackPos.y, m_DoorWidth, m_DoorHeight);
}

void Door::Draw() const
{
	m_pTunnelTexture->Draw(m_Pos);
	
	if (m_IsOpen)
	{
		if (m_Scale > 0)
		{
			glPushMatrix();
			{
				glTranslatef(m_Pos.x + m_TunnelTextureWidth, m_Pos.y, 1);
				glScalef(m_Scale, 1, 1);
				m_pFrontSprite->Draw(Point2f(0, 0), false);
			}
			glPopMatrix();
		}
	}
	else
	{
		m_pFrontSprite->Draw(Point2f(m_Pos.x + m_TunnelTextureWidth, m_Pos.y), false);
	}
	
	DrawMirroredDoor();
}

void Door::Update(float elapsedSec)
{
	if (!m_IsLocked)
	{
		m_pFrontSprite->Update(elapsedSec);
	}
	
	if (m_IsOpen && m_Scale > 0)
	{
		m_Scale -= 0.05f;
	}
}

bool Door::IsLocked() const
{
	return m_IsLocked;
}

bool Door::IsOpen() const
{
	return m_IsOpen;
}

std::vector<Point2f> Door::GetVertices() const
{
	return m_Vertices;
}

void Door::DrawMirroredDoor() const
{
	glPushMatrix();
	{
		glTranslatef(m_Pos.x, m_Pos.y, 1);
		glScalef(-1, 1, 1); 
		m_pTunnelTexture->Draw(Point2f(0, 0));
	}
	glPopMatrix();

	if (m_IsOpen )
	{
		if (m_Scale > 0)
		{
			glPushMatrix();
			{
				glTranslatef(m_Pos.x - m_TunnelTextureWidth , m_Pos.y, 1);
				glScalef(m_Scale, 1, 1);
				m_pFrontSprite->Draw(Point2f(-m_DoorWidth, 0), true);
			}
			glPopMatrix();
		}
	}
	else
	{
		m_pFrontSprite->Draw(Point2f(m_Pos.x - m_TunnelTextureWidth - m_DoorWidth, m_Pos.y), true);
	}
	
}

Rectf Door::GetFrontDoorShape() const
{
	return m_FrontDoorShape;
}

Rectf Door::GetBackDoorShape() const
{
	return m_BackDoorShape;
}

void Door::OpenDoor()
{
	m_IsOpen = true;
}

void Door::CloseDoor()
{
	m_IsOpen = false;
	m_Scale = 1;
}

Point2f Door::GetPos() const
{
	return m_Pos;
}

float Door::GetHeight() const
{
	return m_pTunnelTexture->GetHeight();
}