#pragma once
#include "GameInfo.h"

class Manager
{
public:
	virtual void Draw() const = 0;
	virtual void Update(float elapsedSec, GameInfo& gameInfo) = 0;

	virtual ~Manager() = default;
};

