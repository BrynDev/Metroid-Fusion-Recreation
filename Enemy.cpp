#include "pch.h"
#include "Enemy.h"
#include "Sprite.h"
#include "Avatar.h"
#include "Utils.h"
#include <algorithm>
#include "Parasite.h"
#include "YellowParasite.h"
#include "AvatarProjectile.h"
#include "Geron.h"
#include "SoundEffect.h"

Enemy::Enemy(const int contactDamage, const int health, const float startHorSpeed, const Point2f& startPos, const bool isFlipped, const int detectionRange, const bool canRespawn,Parasite* pParasite)
	:m_ContactDamage{ contactDamage }
	, m_MaxHealth{ health }
	, m_Health{ health }
	, m_IsDead{ false }
	, m_IsDying{ false }
	, m_IsFlipped{ isFlipped }
	, m_StartVelocity{ Vector2f{startHorSpeed * ((!isFlipped * 2) - 1),0.f} }
	, m_IsFlippedAtStart{ isFlipped }
	, m_Velocity{ startHorSpeed,0 }
	, m_StartPos{ startPos }
	, m_TargetFound{ false }
	, m_DetectionRange{ detectionRange }
	, m_IsFlashing{false}
	, m_WasDamageTaken{false}
	, m_AccuFlashingSecs{}
	, m_MaxFlashingSecs{0.25f}
	, m_CanRespawn{canRespawn}
	, m_pParasite{pParasite}
{
	m_Shape.left = startPos.x;
	m_Shape.bottom = startPos.y;
	if (m_IsFlipped)
	{
		m_Velocity.x *= -1;
	}
	m_pSounds.insert(std::make_pair(0, new SoundEffect{ "Resources/Enemies/EnemyHit.wav" }));
	m_pSounds.insert(std::make_pair(1, new SoundEffect{ "Resources/Enemies/EnemyDeath.wav" }));
}

Enemy::~Enemy()
{
	std::for_each(m_pSprites.cbegin(), m_pSprites.cend(), [](std::pair<int,Sprite*> pair) {delete pair.second; });
	std::for_each(m_pSounds.begin(), m_pSounds.end(), [](std::pair<int, SoundEffect*> pair) {delete pair.second; });
	delete m_pParasite;
}

void Enemy::InitSprites(std::ifstream& spriteInfo)
{
	std::string filePath{};
	std::string frameSec{};
	std::string nrCols{};
	int mapKey{};
	while (!spriteInfo.eof())
	{
		std::getline(spriteInfo, filePath, ',');
		std::getline(spriteInfo, frameSec, ',');
		std::getline(spriteInfo, nrCols);
		m_pSprites.insert(std::make_pair(mapKey,new Sprite{ filePath, std::stof(frameSec), std::stoi(nrCols) }));
		++mapKey;
	}
}

bool Enemy::IsDead() const
{
	return m_IsDead;
}

bool Enemy::IsDying() const
{
	return m_IsDying;
}

void Enemy::HandleAvatarContact(Avatar* pAvatar) const
{
	if (utils::IsOverlapping(pAvatar->GetShape(), m_Shape))
	{
		if (!pAvatar->IsImmune() || dynamic_cast<const Geron*>(this) != nullptr)
		{
			pAvatar->HandleHit(m_ContactDamage);
		}
		
	}
}

Rectf Enemy::GetShape() const
{
	return m_Shape;
}

void Enemy::HandleHit(const AvatarProjectile* pAvProj)
{
	m_Health -= pAvProj->GetDamage();
	if (m_Health <= 0)
	{
		m_IsDying = true;
		m_pSounds.find(int(m_IsDying))->second->Play(0);
	}
	else
	{
		m_IsFlashing = true;
		m_pSounds.find(int(m_IsDying))->second->Play(0);
	}
	m_WasDamageTaken = true;
}

bool Enemy::WasDamageTakenFromHit()
{
	if (m_WasDamageTaken)
	{
		m_WasDamageTaken = false;
		return true;
	}
	else
	{
		return false;
	}
}

void Enemy::Reset()
{
	if (!m_CanRespawn)
	{
		return;
	}
	m_IsDead = false;
	m_IsDying = false;
	m_Health = m_MaxHealth;
	m_Shape.left = m_StartPos.x;
	m_Shape.bottom = m_StartPos.y;
	m_IsFlipped = m_IsFlippedAtStart;
	m_Velocity = m_StartVelocity;
	m_IsFlashing = false;
	m_AccuFlashingSecs = 0;
	std::for_each(m_pSprites.begin(), m_pSprites.end(), [](std::pair<int,Sprite*> pair) {pair.second->Reset(); });
	m_pParasite->Reset();
	ResetUniqueDataMembers();
}

void Enemy::ScaleShape(const float scale, const float width, const float height)
{
	m_Shape.width = width * scale;
	m_Shape.height = height * scale;
}

Point2f Enemy::GetCenter() const
{
	return Point2f(m_Shape.left + m_Shape.width / 2.f, m_Shape.bottom + m_Shape.height / 2.f);
}

void Enemy::DetectAvatar(const Point2f& avatarCenter)
{
	const Point2f center{ GetCenter() };

	if (!(avatarCenter.y > center.y - m_Shape.height) && !(avatarCenter.y < center.y + m_Shape.height))//avatar is not close
	{
		return;
	}

	if (m_IsFlipped)
	{
		if (avatarCenter.x > center.x - m_DetectionRange && avatarCenter.x < center.x)
		{
			m_TargetFound = true;
			return;
		}
	}
	else if ((avatarCenter.x < center.x + m_DetectionRange && avatarCenter.x > center.x))
	{
		m_TargetFound = true;
		return;
	}
	m_TargetFound = false;
}

void Enemy::HandleFlashingTimer(float elapsedSec)
{
	m_AccuFlashingSecs += elapsedSec;
	if (m_AccuFlashingSecs >= m_MaxFlashingSecs)
	{
		m_IsFlashing = false;
		m_AccuFlashingSecs = 0;
	}
}

void Enemy::HandleDeath()
{
	m_IsDead = true;
	m_pParasite->Activate(GetCenter());
}

void Enemy::ChangeEffectVolume(const int newVolume)
{
	std::for_each(m_pSounds.begin(), m_pSounds.end(), [&newVolume](std::pair<int, SoundEffect*> pair) {pair.second->SetVolume(newVolume); });
	m_pParasite->ChangeEffectVolume(newVolume);
}