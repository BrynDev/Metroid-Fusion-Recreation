#include "pch.h"
#include "MenuItem.h"
#include <string>
#include "Texture.h"


MenuItem::MenuItem(const std::string& text, const Point2f& bottomLeft)
	:m_Text{text, "Resources/Menu/Metroid-Fusion.ttf",20,Color4f(1,1,1,1)}
	, m_TextPos{bottomLeft}
	, m_Value{10}
	, m_IsSelected{false}
	, m_ValueMin{0}
	, m_ValueMax{10}
{
	for (int number{}; number <= 10; ++number)
	{
		m_ValueTextures.push_back(Texture{ std::to_string(number), "Resources/Menu/Metroid-Fusion.ttf", 20,Color4f(1, 1, 1, 1) });
	}
	const int valueDistance{ 40 };
	m_ValuePos = Point2f(bottomLeft.x + m_Text.GetWidth() + valueDistance, bottomLeft.y);
}

void MenuItem::Draw() const
{
	m_Text.Draw(m_TextPos);
	m_ValueTextures[m_Value].Draw(m_ValuePos);
}

void MenuItem::ToggleSelected()
{
	m_IsSelected = !m_IsSelected;
}

void MenuItem::DecreaseValue()
{
	if (m_Value > m_ValueMin)
	{
		--m_Value;
	}
}

void MenuItem::IncreaseValue()
{
	if (m_Value < m_ValueMax)
	{
		++m_Value;
	}
}

bool MenuItem::IsSelected() const
{
	return m_IsSelected;
}

Point2f MenuItem::GetTextPos() const
{
	return m_TextPos;
}

Point2f MenuItem::GetValuePos() const
{
	return m_ValuePos;
}

int MenuItem::GetValue() const
{
	return m_Value;
}
