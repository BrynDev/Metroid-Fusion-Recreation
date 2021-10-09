#include "pch.h"
#include "PowerUpManager.h"
#include <algorithm>
#include "PowerUp.h"
#include "MissilePowerUp.h"
#include "GameInfo.h"


PowerUpManager::PowerUpManager()
{
	m_pPowerUps.push_back(new MissilePowerUp{ Point2f(180,639) });
	m_pPowerUps.push_back(new MissilePowerUp{ Point2f(5695,2192) });
}


PowerUpManager::~PowerUpManager()
{
	std::for_each(m_pPowerUps.begin(), m_pPowerUps.end(), [](PowerUp* pPowerUp) { delete pPowerUp; });
}

void PowerUpManager::Draw() const
{
	std::for_each(m_pPowerUps.begin(), m_pPowerUps.end(), [](PowerUp* pPowerUp) { if(!pPowerUp->WasTaken())pPowerUp->Draw(); });
}

void PowerUpManager::Update(float elapsedSec, GameInfo& info)
{
	std::for_each(m_pPowerUps.begin(), m_pPowerUps.end(), [elapsedSec, &info](PowerUp* pPowerUp) { if (!pPowerUp->WasTaken())pPowerUp->Update(elapsedSec, info.pAvatar); });
}

void PowerUpManager::ChangeEffectVolume(const int newVolume)
{
	std::for_each(m_pPowerUps.begin(), m_pPowerUps.end(), [&newVolume](PowerUp* pPowerUp) { pPowerUp->ChangeEffectVolume(newVolume); });
}