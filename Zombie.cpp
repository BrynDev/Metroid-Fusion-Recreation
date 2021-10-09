#include "pch.h"
#include "Zombie.h"
#include "Enemy.h"
#include "Sprite.h"
#include "Level.h"
#include "Parasite.h"
#include <fstream>
#include <assert.h>


Zombie::Zombie(const Point2f& startPos, const bool isFlipped, Parasite* pParasite)
	:Enemy{16, 1, 20, startPos, isFlipped, 0, true, pParasite}
	, m_State{EnemyState::moving}
{
	/*m_pSprites.push_back(new Sprite{ "Resources/Zombie/Zombie_Walk.png",0.28f,28 });
	m_pSprites.push_back(new Sprite{ "Resources/Zombie/Zombie_Death.png",0.16f,7 });*/
	std::ifstream spriteInfo{ "Resources/Enemies/Zombie/SpriteInfo.txt" };
	assert(spriteInfo);
	InitSprites(spriteInfo);
}

void Zombie::Draw() const
{
	if (!m_IsDead)
	{
		if (m_IsFlashing)
		{
			m_pSprites.find(int(m_State))->second->Draw(m_Shape, m_IsFlipped, false, Color4f(1, 0.5f, 0.5f, 1));
		}
		else
		{
			m_pSprites.find(int(m_State))->second->Draw(m_Shape, m_IsFlipped);
		}
	}
	else
	{
		m_pParasite->Draw();
	}
}

void Zombie::Update(float elapsedSec, GameInfo& gameInfo)
{
	if (m_IsDead)
	{
		m_pParasite->Update(elapsedSec, gameInfo.pAvatar);
		return;
	}

	if (m_IsDying)
	{
		m_State = EnemyState::dying;
	}
	else
	{
		m_State = EnemyState::moving;
	}

	m_pSprites.find(int(m_State))->second->Update(elapsedSec);
	switch (m_State)
	{
	case EnemyState::moving:
		UpdatePos(elapsedSec);
		HandleCollision(gameInfo.pLevel);
		break;
	case EnemyState::dying:
		if (m_pSprites.find(int(m_State))->second->HasFinished())
		{
			HandleDeath();
		}
		break;
	}

	const float scale{ 2.3f };
	ScaleShape(scale, m_pSprites[int(m_State)]->GetFrameWidth(), m_pSprites[int(m_State)]->GetFrameHeight());
	
}

void Zombie::UpdatePos(float elapsedSec)
{
	m_Shape.left += m_Velocity.x * elapsedSec;
}

void Zombie::HandleCollision(const Level* pLevel)
{
	utils::HitInfo hitInfo;
	if (pLevel->CheckHorEnemyCollision(m_Shape, hitInfo))
	{
		if (m_Velocity.x > 0)
		{
			m_Shape.left = hitInfo.intersectPoint.x - m_Shape.width - 2;
		}
		else
		{
			m_Shape.left = hitInfo.intersectPoint.x + 2;
		}
		m_IsFlipped = !m_IsFlipped;
		m_Velocity.x = -m_Velocity.x;
	}
}

void Zombie::ResetUniqueDataMembers()
{
	m_State = EnemyState::moving;
}