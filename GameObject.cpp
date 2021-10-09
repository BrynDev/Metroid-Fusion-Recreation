#include "pch.h"
#include "GameObject.h"


GameObject::GameObject()
{
}

Rectf GameObject::GetShape() const
{
	return Rectf(0, 0, 0, 0);
}

