#pragma once
#include <string>

class Texture;

class Sprite
{
public:
	Sprite(const std::string& filename, float frameSec, int nrCols, int nrRows = 1 );
	~Sprite();
	Sprite(const Sprite& other) = delete;
	Sprite& operator=(const Sprite& rhs) = delete;
	Sprite(Sprite&& other) = delete;
	Sprite& operator=(Sprite&& rhs) = delete;

	void Update(float elapsedSec);
	void Draw(const Rectf& rect, const bool isHorFlipped, const bool isVertFlipped = false, const Color4f& color = Color4f(1,1,1,1)) const;
	void Draw(const Point2f& pos, const bool isHorFlipped) const;
	float GetFrameWidth() const;
	float GetFrameHeight() const;
	bool HasFinished();
	void Reset();

private:
	Texture* m_pTexture;
	const int m_Cols;
	const int m_Rows;
	const float m_FrameSec;
	float m_AccuSec;
	int m_ActFrame;
	bool m_HasPlayedOnce;
	


};
