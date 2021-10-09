#pragma once

class Level;

class GameObject
{
public:
	GameObject();
	virtual ~GameObject() = default;
	virtual void Update(float elapsedSec, const Level* pLevel) = 0;
	virtual void Draw() const = 0;
	virtual Rectf GetShape() const;
protected:
};

