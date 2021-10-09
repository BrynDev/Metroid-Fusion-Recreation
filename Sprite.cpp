#include "pch.h"

#include "Sprite.h"
#include "Texture.h"
#include "structs.h"

Sprite::Sprite(const std::string& filename, float frameSec, int nrCols, int nrRows)
	:m_pTexture{ nullptr }
	,m_Cols{ nrCols }
	,m_Rows{ nrRows }
	,m_FrameSec {frameSec}
	, m_AccuSec{}
	, m_ActFrame{}
	, m_HasPlayedOnce{false}

{
	m_pTexture = new Texture{ filename };
}

Sprite::~Sprite()
{
	delete m_pTexture;
	m_pTexture = nullptr;
}

void Sprite::Update(float elapsedSec)
{
	if (m_Cols <= 1)
	{
		return;
	}

	m_AccuSec += elapsedSec;

	if (m_AccuSec >= m_FrameSec)
	{
		++m_ActFrame;
		m_ActFrame %= (m_Cols * m_Rows);
		m_AccuSec = 0;
		if ((m_ActFrame+1) % (m_Cols * m_Rows) == 0) //if active frame is last frame, sprite has fully played
		{
			m_HasPlayedOnce = true;
		}
	}

}

void Sprite::Draw(const Rectf& shape, const bool isHorFlipped, const bool isVertFlipped, const Color4f& color) const
{
	const float width{ GetFrameWidth() };
	const float height{ GetFrameHeight() };
	Rectf srcRect{m_ActFrame * width, 0.f, width, height};
	Rectf destRect{ -shape.width / 2.f, -shape.height / 2.f, shape.width, shape.height };
	if (isHorFlipped || isVertFlipped)
	{
		glPushMatrix();
		{
			glTranslatef(shape.left + shape.width / 2.f, shape.bottom + shape.height / 2.f, 0.f);
			if (isHorFlipped)
			{
				glScalef(-1, 1, 1);
			}
			if (isVertFlipped)
			{
				glScalef(1, -1, 1);
			}
			
			m_pTexture->Draw(destRect, srcRect, color);
		}
		glPopMatrix();
	}
	else
	{
		glPushMatrix();
		{
			glTranslatef(shape.left + shape.width / 2.f, shape.bottom + shape.height / 2.f, 0.f);
			m_pTexture->Draw(destRect, srcRect, color);
		}
		glPopMatrix();
	}
}

void Sprite::Draw(const Point2f& pos, const bool isHorFlipped) const
{
	Draw(Rectf(pos.x, pos.y, m_pTexture->GetWidth() / m_Cols, m_pTexture->GetHeight() / m_Rows), isHorFlipped);
}

float Sprite::GetFrameWidth() const
{
	return (m_pTexture->GetWidth() / m_Cols);
}

float Sprite::GetFrameHeight() const
{
	return (m_pTexture->GetHeight() / m_Rows);
}

bool Sprite::HasFinished()
{
	if (m_HasPlayedOnce)
	{
		m_HasPlayedOnce = false; //reset HasPlayedOnce so the sprite can play properly again for the next object
		return true;
	}
	else
	{
		return false;
	}
	
}

void Sprite::Reset()
{
	m_AccuSec = 0;
	m_ActFrame = 0;
	m_HasPlayedOnce = false;
}