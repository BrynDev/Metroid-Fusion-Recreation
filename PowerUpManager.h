#pragma once
#include "Manager.h"
#include <vector>

class PowerUp;

class PowerUpManager : public Manager
{
public:
	PowerUpManager();
	virtual ~PowerUpManager();
	PowerUpManager(const PowerUpManager& other) = delete;
	PowerUpManager& operator=(const PowerUpManager& rhs) = delete;
	PowerUpManager(PowerUpManager&& other) = delete;
	PowerUpManager& operator=(PowerUpManager&& rhs) = delete;

	virtual void Draw() const override;
	virtual void Update(float elapsedSec, GameInfo& gameInfo) override;
	void ChangeEffectVolume(const int newVolume);
private:
	std::vector<PowerUp*> m_pPowerUps;
};

