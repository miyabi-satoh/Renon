///////////////////////////////////////////////////////////////////////////////
/// @file	SnowSprite.cpp
/// @date	2014/08/01	
/// @author	Masayuki
/// @brief	class SnowSprite
///////////////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "SnowSprite.h"
#include "RenonDefs.h"
#include <dibtransition.hpp>

struct SnowPixOp
{
	int alpha;
	mxRGBA colorKey;

	inline void operator()(mxRGBA &X, const mxRGBA &Y) {
		if (Y != colorKey) {
			BYTE tmp;
//			tmp = (X.r * (255 - Y.r) + (Y.r * Y.r)) >> 8;
//			X.r = (X.r * (255 - alpha) + (Y.r * alpha)) >> 8;

//			tmp = (X.g * (255 - Y.g) + (Y.g * Y.g)) >> 8;
//			X.g = (X.g * (255 - alpha) + (Y.g * alpha)) >> 8;

//			tmp = (X.b * (255 - Y.b) + (Y.b * Y.b)) >> 8;
//			X.b = (X.b * (255 - alpha) + (Y.b * alpha)) >> 8;


			tmp = X.r + ((Y.r   * (Y.r - X.r)) >> 8);
			X.r = X.r + ((alpha * (tmp - X.r)) >> 8);

			tmp = X.g + ((Y.g   * (Y.g - X.g)) >> 8);
			X.g = X.g + ((alpha * (tmp - X.g)) >> 8);

			tmp = X.b + ((Y.b   * (Y.b - X.b)) >> 8);
			X.b = X.b + ((alpha * (tmp - X.b)) >> 8);
		}
	}
};

void SnowSprite::fastRender(
		mxDibSection &dstDib,
		const mxRect &dstRect,
		const mxPoint &srcPoint)
{
	mxTrans::Blt<SnowPixOp> Trans;
	Trans.getOperator().alpha = this->alpha();
	Trans.getOperator().colorKey = this->colorKey();
	Trans.doTransition(&dstDib, dstRect, this, srcPoint);
}

void SnowSprite::Init()
{
	// 初期x座標
	m_dx = 10 + rand() % 620;
	m_x = m_dx;
	// 初期y座標
	m_y = 0 - 20 - z() * (rand() % 120);
	// 落下速度
	m_speed = (1.9 * z() / NUM_SNOW) + 2.0;
	// 振幅
	m_amp = 10.0 + (rand() % 40);
	// 周期
	m_range = (200.0 * z() / NUM_SNOW) + 240.0;
	// 初期位相
	m_rad = PI * (rand() % 100) / 100.0;

	this->setPos(mxPoint(static_cast<int>(m_x), static_cast<int>(m_y)));
}

void SnowSprite::Move() {
	m_y += m_speed;
	if (z() & 0x01) {
		m_x = m_dx + m_amp * ::sin(m_rad + PI * m_y / m_range);
	}
	else {
		m_x = m_dx + m_amp * ::cos(m_rad + PI * m_y / m_range);
	}
	this->setPos(mxPoint(static_cast<int>(m_x), static_cast<int>(m_y)));
}

