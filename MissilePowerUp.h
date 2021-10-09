#pragma once
#include "PowerUp.h"

class MissilePowerUp : public PowerUp
{
public:
	MissilePowerUp(const Point2f& pos);
	virtual ~MissilePowerUp() = default;
};

