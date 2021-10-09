#include "pch.h"
#include "Menu.h"
#include "Texture.h"
#include "Sprite.h"
#include "MenuItem.h"
#include <algorithm>
#include <fstream>
#include <assert.h>


Menu::Menu(const Point2f& windowCenter)
	:m_BackgroundTexture{ "Resources/Menu/MenuBackground.png" }
	, m_IsOpen{false}
{
	m_BackgroundPos = Point2f{ windowCenter.x - m_BackgroundTexture.GetWidth() / 2.f, windowCenter.y - m_BackgroundTexture.GetHeight() / 2.f };
	InitItems();
	InitCursor();
}

void Menu::InitCursor()
{
	InitCursorSprite();
	const int scale{ 2 };
	const int leftGap{10};
	m_CursorShape = Rectf(0, 0, scale * m_pCursorSprite->GetFrameWidth(), scale * m_pCursorSprite->GetFrameHeight());
	Point2f itemTextPos{ m_pItems[0]->GetTextPos() };
	m_CursorShape.left = itemTextPos.x - m_CursorShape.width - leftGap;
	m_CursorShape.bottom = itemTextPos.y;
}

void Menu::InitCursorSprite()
{
	std::ifstream spriteInfo{ "Resources/Menu/SpriteInfo.txt" };
	assert(spriteInfo);

	std::string filePath{};
	std::string frameSec{};
	std::string nrCols{};
	std::getline(spriteInfo, filePath, ',');
	std::getline(spriteInfo, frameSec, ',');
	std::getline(spriteInfo, nrCols);
	m_pCursorSprite = new Sprite{ filePath, std::stof(frameSec), std::stoi(nrCols) };
}

void Menu::InitItems()
{
	int widthOffset{ 50 };
	int HeightOffset{ 50 };
	Point2f itemOffset{ m_BackgroundPos.x + widthOffset, m_BackgroundPos.y + m_BackgroundTexture.GetHeight() - HeightOffset };
	m_pItems.push_back(new MenuItem{ "Music volume", itemOffset });
	m_pItems.push_back(new MenuItem{ "Effect volume", Point2f{itemOffset.x, itemOffset.y - HeightOffset} });
	m_pItems.front()->ToggleSelected();
}

Menu::~Menu()
{
	delete m_pCursorSprite;
	std::for_each(m_pItems.cbegin(), m_pItems.cend(), [](MenuItem* item) {delete item; });
}

void Menu::Draw() const
{
	m_BackgroundTexture.Draw(m_BackgroundPos);
	std::for_each(m_pItems.cbegin(), m_pItems.cend(), [](MenuItem* item) {item->Draw(); });
	//std::vector<MenuItem*>::const_iterator selectedIt{ std::find_if(m_pItems.cbegin(), m_pItems.cend(), [](MenuItem* item) {return item->IsSelected(); }) };
	m_pCursorSprite->Draw(m_CursorShape, false);

}

void Menu::Update(float elapsedSec)
{
	m_pCursorSprite->Update(elapsedSec);
}

void Menu::MoveCursorUp()
{
	using vectIt = std::vector<MenuItem*>::const_iterator;
	vectIt selectedIt = std::find_if(m_pItems.cbegin(), m_pItems.cend(), [](MenuItem* pItem) {return pItem->IsSelected(); }) ;
	if (*selectedIt == m_pItems.front())
	{
		return;
	}
	std::any_of(m_pItems.begin(), m_pItems.end(), [this](MenuItem* pItem) {
		if (pItem->IsSelected())
		{
			pItem->ToggleSelected();
			m_pItems.front()->ToggleSelected();
			ChangeCursorPos();
			return true;
		}
		return false;
	});
}

void Menu::MoveCursorDown()
{
	using vectIt = std::vector<MenuItem*>::const_iterator;
	vectIt selectedIt = std::find_if(m_pItems.cbegin(), m_pItems.cend(), [](MenuItem* pItem) {return pItem->IsSelected(); });
	if (*selectedIt == m_pItems.back())
	{
		return;
	}
	std::any_of(m_pItems.begin(), m_pItems.end(), [this](MenuItem* pItem) {
		if (pItem->IsSelected())
		{
			pItem->ToggleSelected();
			m_pItems.back()->ToggleSelected();
			ChangeCursorPos();
			return true;
		}
		return false;
	});
}

void Menu::DecreaseItemValue()
{
	std::any_of(m_pItems.begin(), m_pItems.end(), [](MenuItem* pItem) {
		if (pItem->IsSelected())
		{
			pItem->DecreaseValue();
			return true;
		}
		return false;
	});
}

void Menu::IncreaseItemValue()
{
	std::any_of(m_pItems.begin(), m_pItems.end(), [](MenuItem* pItem) {
		if (pItem->IsSelected())
		{
			pItem->IncreaseValue();
			return true;
		}
		return false;
	});
}

void Menu::ToggleOpen()
{
	m_IsOpen = !m_IsOpen;
}

bool Menu::IsOpen() const
{
	return m_IsOpen;
}

void Menu::ChangeCursorPos()
{
	std::any_of(m_pItems.cbegin(), m_pItems.cend(), [this](MenuItem* pItem) {
		if (pItem->IsSelected())
		{
			m_CursorShape.bottom = pItem->GetTextPos().y;
			return true;
		}
		return false;
	});
}

int Menu::GetMusicVolume() const
{
	return m_pItems.front()->GetValue();
}

int Menu::GetEffectVolume() const
{
	return m_pItems.back()->GetValue();
}