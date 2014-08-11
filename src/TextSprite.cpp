////////////////////////////////////////////////////////////////////////////////
/// @file	TextSprite.cpp
/// @date	2014/08/02	
/// @author	Masayuki
/// @brief	class TextSprite
////////////////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "TextSprite.h"
#include "ResourceManager.h"
#include "Surface.h"
#include "Config.h"
#include "RenonApp.h"
#include <window.hpp>
#include <dibtransition.hpp>

UINT TextSprite::m_FrameIndex = 0;
const int DEFAULT_FONT_HEIGHT = 26;	///< フォントの高さ
const int DEFAULT_FONT_WIDTH = 24;		///< フォントの幅
const int DEFAULT_LINE_HEIGHT = 28;	///< １行の高さ
const int DEFAULT_COLS = 22;			///< １行の文字数
const mxPoint ptDefault(54, 365);
const mxRGBA TransparentColor = mxRGBA(0, 255, 0);		///< 透過色
const mxRGBA HalfTransparentColor = mxRGBA(0, 128, 0);	///< 半透明色

////////////////////////////////////////////////////////////////////////////////
/// テキストフレームを描画するピクセルオペレータ
///
struct FramePixOp
{
	void operator()(mxRGBA &X, const mxRGBA &Y) const {
		if (Y == TransparentColor) {
			// 透過色なので何もしない
		}
		else if (Y == HalfTransparentColor) {
			// 半透明
			X.r = (X.r * 80) >> 8;
			X.g = (X.g * 112) >> 8;
			X.b = (X.b * 160) >> 8;
		}
		else {
			X = Y;
		}
	}
};

void TextSprite::Resize(double scale, const mxRect &rcPaint)
{
	m_FontHeight = static_cast<int>(DEFAULT_FONT_HEIGHT * scale);
	m_FontWidth = static_cast<int>(DEFAULT_FONT_WIDTH * scale);
	m_LineHeight = static_cast<int>(DEFAULT_LINE_HEIGHT * scale);
	m_pt = ptDefault * scale;

	if (m_vFrameSprite.empty()) {
		mxString strName = m_ResMgr->GetResourcePath(_T("GRDAT.PDT"));
		mxDibSection dibGRDAT;
		Surface::LoadFromFile(strName.c_str(), dibGRDAT);

		for (int n = 0; n < 4; n++) {
			mxSprite *p = new mxSprite();
			p->create(mxSize(592, 112));
			p->setPos(mxPoint(22, 350));
			p->setZ(Surface::FRAME);
			p->setAlpha(0);

			// 全体を透過色で塗りつぶす
			mxTrans::Blt<mxPixelOp::Fill> Filler;
			Filler.getOperator().rgbColor = TransparentColor;
			Filler.doTransition(p, p->getRect());
			// 文字表示領域を半透明色で塗りつぶす
			Filler.getOperator().rgbColor = HalfTransparentColor;
			Filler.doTransition(p, mxRect(5, 5, 587, 107));

			// 一時ビットマップにコピー
			mxDibSection dibSrc;
			dibSrc.create(mxSize(112, 64));
			dibGRDAT.bitBlt(dibSrc, dibSrc.getRect(), mxPoint(8, 8 + 100 * n));

			const mxSize szCorner(40, 24);	// コーナーのサイズ
			const mxSize szHoSide(32, 24);	// 横辺のサイズ
			const mxSize szVeSide(40, 16);	// 縦辺のサイズ

			// フレームを透過コピー
			if (dibGRDAT.width() > 0) {
				mxTrans::Blt<mxPixelOp::TransparentCopy> TransCopier;
				TransCopier.getOperator().colorKey = dibGRDAT.getPixel(mxPoint(0, 0));
				int w = p->width();
				int h = p->height();
				for (int dx = szCorner.cx; dx < w - szCorner.cx; dx += szHoSide.cx) {
					TransCopier.doTransition(p, mxRect(mxPoint(dx, 0), szHoSide), &dibSrc, mxPoint(40, 0));					// 上
					TransCopier.doTransition(p, mxRect(mxPoint(dx, h - szHoSide.cy), szHoSide), &dibSrc, mxPoint(40, 40));	// 下
				}
				for (int dy = szCorner.cy; dy < h - szCorner.cy; dy += szVeSide.cy) {
					TransCopier.doTransition(p, mxRect(mxPoint(0, dy), szVeSide), &dibSrc, mxPoint(0, 24));					// 左
					TransCopier.doTransition(p, mxRect(mxPoint(w - szVeSide.cx, dy), szVeSide), &dibSrc, mxPoint(72, 24));	// 右
				}
				TransCopier.doTransition(p, mxRect(mxPoint(0, 0), szCorner), &dibSrc, mxPoint(0, 0));								// 左上
				TransCopier.doTransition(p, mxRect(mxPoint(0, h - szCorner.cy), szCorner), &dibSrc, mxPoint(0, 40));				// 左下
				TransCopier.doTransition(p, mxRect(mxPoint(w - szCorner.cx, 0), szCorner), &dibSrc, mxPoint(72, 0));				// 右上
				TransCopier.doTransition(p, mxRect(mxPoint(w - szCorner.cx, h - szCorner.cy), szCorner), &dibSrc, mxPoint(72, 40));	// 右下
			}
			m_vFrameSprite.push_back(SpritePtr(p));
		}
	}
}

void TextSprite::fastRender(
		mxDibSection &dstDib,
		const mxRect &dstRect,
		const mxPoint &srcPoint)
{
	// フレームだけレンダリング
	mxTrans::Blt<FramePixOp> Trans;
	Trans.doTransition(&dstDib, dstRect, m_vFrameSprite[m_FrameIndex].get(), srcPoint);
}

void TextSprite::DrawString(std::wstring s, mxDibSection &dib)
{
	m_Font.create(m_Config->GetFontName(), m_FontHeight, 0, SHIFTJIS_CHARSET);
	HGDIOBJ hOldFont = dib.selectObject(m_Font);
	dib.setBkMode(TRANSPARENT);

	mxPoint pt(m_pt);
	for (std::wstring::iterator it = s.begin(); it != s.end(); it++) {
		mxString c(it, it + 1);
		if (c == _T("\\")) {	// 強制改行
			pt.x = m_pt.x;
			pt.y += m_LineHeight;
			continue;
		}
		else if (pt.x >= m_pt.x + m_FontWidth * (DEFAULT_COLS + 1)) {	// 禁則文字だろうと何だろうと改行
			pt.x = m_pt.x;
			pt.y += m_LineHeight;
		}
		else if (pt.x >= m_pt.x + m_FontWidth * DEFAULT_COLS) {	// 通常の改行
			if (c != _T("、") && c != _T("。") && c != _T("」") && c != _T("』") && c != _T("）")) {
				pt.x = m_pt.x;
				pt.y += m_LineHeight;
			}
		}
		// 2行目以降のインデント
		if (pt.y >= m_pt.y + m_LineHeight && pt.x == m_pt.x) {
			pt.x = m_pt.x + m_FontWidth * m_IndentChars;
		}

		// 影文字
		dib.setTextColor(RGB(0,0,0));
		dib.textOut(pt, c.c_str(), 1);

		// 本体
		dib.setTextColor(RGB(255, 255, 255));
		dib.textOut(pt, c.c_str(), 1);

		mxTrace(pt.x << _T(",") << pt.y);
		pt.x += m_FontWidth;
	}

	dib.selectObject(hOldFont);
}
