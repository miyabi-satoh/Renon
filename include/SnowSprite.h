///////////////////////////////////////////////////////////////////////////////
/// @file	SnowSprite.h
/// @date	2014/08/01	
/// @author	Masayuki
/// @brief	class SnowSprite
///////////////////////////////////////////////////////////////////////////////

#ifndef SNOWSPRITE_H_
#define SNOWSPRITE_H_

#include <sprite.hpp>

///////////////////////////////////////////////////////////////////////////////
/// 雪のスプライトクラス
///
class SnowSprite: public mxSprite {
public:
	SnowSprite(int z) : mxSprite() {
		this->setZ(z);
		Init();
	}
	~SnowSprite() {
	}

	void Init();
	void Move();

	void 	fastRender (mxDibSection &dstDib, const mxRect &dstRect, const mxPoint &srcPoint);

private:
	double m_dx;	///< x座標のずれ
	double m_x;		///< x座標
	double m_y;		///< y座標
	double m_speed;	///< 落下速度
	double m_amp;	///< 振幅
	double m_range;	///< 周期
	double m_rad;	///< 初期位相
};

#endif /* SNOWSPRITE_H_ */
