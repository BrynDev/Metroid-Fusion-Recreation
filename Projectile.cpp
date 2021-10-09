#include "pch.h"
#include "Projectile.h"
#include "Level.h"
#include "Sprite.h"
#include "EnemyManager.h"
#include <algorithm>
#include <fstream>


Projectile::Projectile(const Vector2f& velocity, const int damage)
	:m_Velocity{velocity}
	, m_ActiveVelocity{}
	, m_Damage{damage}
	, m_Shape{Rectf(0, 0, 0, 0)}
	, m_IsActive{false}
{
}

Projectile::~Projectile()
{
	std::for_each(m_pSprites.begin(), m_pSprites.end(), [](std::pair<int,Sprite*> pair) {delete pair.second; });
}

void Projectile::InitSprites(std::ifstream& ifStream)
{
	std::string filePath{};
	std::string frameSec{};
	std::string nrCols{};
	int mapKey{};
	while (!ifStream.eof())
	{
		std::getline(ifStream, filePath, ',');
		std::getline(ifStream, frameSec, ',');
		std::getline(ifStream, nrCols);
		m_pSprites.insert(std::make_pair(mapKey, new Sprite{ filePath, std::stof(frameSec), std::stoi(nrCols) }));
		++mapKey;
	}
}

void Projectile::UpdatePos(float elapsedSec)
{
	m_Shape.left += m_ActiveVelocity.x * elapsedSec;
	m_Shape.bottom += m_ActiveVelocity.y * elapsedSec;
}
bool Projectile::IsActive() const
{
	return m_IsActive;
}
bool Projectile::HasHit() const
{
	return m_HasHit;
}

void Projectile::ToggleActive()
{
	m_IsActive = !m_IsActive;
}

void Projectile::SetPos(const Point2f& pos)
{
	m_Shape.left = pos.x;
	m_Shape.bottom = pos.y;
}

void Projectile::SetActiveVelocity(const bool isTravellingLeft, const bool isTravellingRight, const bool isTravellingUp, const bool isTravellingDown)
{
	if (isTravellingRight)
	{
		m_ActiveVelocity.x = m_Velocity.x;
	}
	else if (isTravellingLeft)
	{
		m_ActiveVelocity.x = -m_Velocity.x;
	}
	else
	{
		m_ActiveVelocity.x = 0;
	}

	if (isTravellingUp)
	{
		m_ActiveVelocity.y = m_Velocity.y;
	}
	else if (isTravellingDown)
	{
		m_ActiveVelocity.y = -m_Velocity.y;
	}
	else
	{
		m_ActiveVelocity.y = 0;
	}
}

Rectf Projectile::GetShape() const
{
	return m_Shape;
}

int Projectile::GetDamage() const
{
	return m_Damage;
}



