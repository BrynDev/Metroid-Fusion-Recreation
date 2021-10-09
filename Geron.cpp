#include "pch.h"
#include "Geron.h"
#include "Enemy.h"
#include "Sprite.h"
#include "Missile.h"
#include "Parasite.h"
#include <fstream>
#include <assert.h>


Geron::Geron(const Point2f& pos, Parasite* pParasite)
	:Enemy(30,30,0,pos, false, 0, false,pParasite)
{
	std::ifstream spriteInfo{ "Resources/Enemies/Geron/SpriteInfo.txt" };
	assert(spriteInfo);
	InitSprites(spriteInfo);

	const int scale{ 2 };
	m_Shape.width = m_pSprites[0]->GetFrameWidth() * scale;
	m_Shape.height = m_pSprites[0]->GetFrameHeight() * scale;
}

void Geron::HandleHit(const AvatarProjectile* pAvProj)
{
	if (dynamic_cast<const Missile*>(pAvProj) != nullptr)
	{
		m_Health -= pAvProj->GetDamage();
		if (m_Health <= 0)
		{
			m_IsDying = true;
		}
		else
		{
			m_IsFlashing = true;
		}
		m_WasDamageTaken = true;
	}
}

void Geron::Draw() const
{
	if (!m_IsDead)
	{
		if (m_IsFlashing)
		{
			m_pSprites.find(int(m_IsDying))->second->Draw(m_Shape, false, false, Color4f(1, 0.5f, 0.5f, 1));
		}
		else
		{
			m_pSprites.find(int(m_IsDying))->second->Draw(m_Shape, m_IsFlipped);
		}
	}
	else
	{
		m_pParasite->Draw();
	}
}

void Geron::Update(float elapsedSec, GameInfo& info)
{
	if (m_IsDead)
	{
		m_pParasite->Update(elapsedSec,info.pAvatar);
		return;
	}
	if (m_IsDying && m_pSprites.find(int(m_IsDying))->second->HasFinished())
	{
		HandleDeath();
	}

	m_pSprites.find(int(m_IsDying))->second->Update(elapsedSec);

	if (m_IsFlashing)
	{
		HandleFlashingTimer(elapsedSec);
	}
}

void Geron::ResetUniqueDataMembers()
{
	//nothing unique to reset
}
