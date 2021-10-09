#pragma once
#include <vector>
#include "Texture.h"

class MenuItem
{
public:
	MenuItem(const std::string& text, const Point2f& bottomLeft);
	~MenuItem() = default;
	void Draw() const;
	void ToggleSelected();
	bool IsSelected() const;
	Point2f GetTextPos() const;
	Point2f GetValuePos() const;
	int GetValue() const;

	void IncreaseValue();
	void DecreaseValue();
private:
	const Texture m_Text;
	std::vector<Texture> m_ValueTextures;
	const Point2f m_TextPos;
	Point2f m_ValuePos;
	int m_Value;
	bool m_IsSelected;
	const int m_ValueMax;
	const int m_ValueMin;
};

