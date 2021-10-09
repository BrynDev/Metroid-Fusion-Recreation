#pragma once
#include "Texture.h"
#include "structs.h"
#include <vector>

class Sprite;
class MenuItem;

class Menu
{
public:
	Menu(const Point2f& windowCenter);
	~Menu();
	Menu(const Menu& other) = delete;
	Menu& operator=(const Menu& rhs) = delete;
	Menu(Menu&& other) = delete;
	Menu& operator=(Menu&& rhs) = delete;
	void Draw() const;
	void Update(float elapsedSec);
	void ToggleOpen();
	bool IsOpen() const;
	void MoveCursorUp();
	void MoveCursorDown();
	void IncreaseItemValue();
	void DecreaseItemValue();
	int GetMusicVolume() const;
	int GetEffectVolume() const;
private:
	const Texture m_BackgroundTexture;
	Point2f m_BackgroundPos;
	Sprite* m_pCursorSprite;
	Rectf m_CursorShape;
	bool m_IsOpen;
	std::vector<MenuItem*> m_pItems;

	void InitItems();
	void InitCursor();
	void InitCursorSprite();
	void ChangeCursorPos();
};

