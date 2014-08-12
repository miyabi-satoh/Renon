///////////////////////////////////////////////////////////////////////////////
/// @file	ScnText.cpp
/// @date	2014/08/01	
/// @author	Masayuki
/// @brief	
///////////////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "ScnText.h"
#include "Surface.h"

void ScnText::Create(std::wstring s, Surface *pSurface)
{
	new ScnText(s, pSurface);
}

ScnText::ScnText(std::wstring s, Surface *pSurface)
	: Scene(), m_Text(s), m_CastName()
{
	m_Surface = pSurface;
	m_Ret = Scene::Next;
	m_MarkIndex = 0;

	// 台詞の発言者を取得する
	std::wstring::iterator it = m_Text.begin();
	if (*it == _T('[')) {
		for (it++; it != m_Text.end(); it++) {
			if (*it == _T(']')) {
				break;
			}
			m_CastName += *it;
		}
		m_Text.erase(m_Text.begin(), it + 1);
	}
	if (m_CastName.empty()) {
		m_Surface->GetTextSprite().SetIndentChars(0);
	}
	else {
		m_Surface->GetTextSprite().SetIndentChars(m_CastName.length() + 1);
	}
	m_Text = m_CastName + m_Text;
	m_it = m_Text.begin();
}

ScnText::~ScnText()
{
	// 改頁マークを非表示にする
	for (UINT n = 0; n < 16; n++) {
		m_Surface->GetMarkSprite(n).setAlpha(0);
	}
}

int ScnText::UpdateFrame(UINT nFrameCount)
{
	if (nFrameCount == 1) {
		m_Surface->GetTextSprite().setAlpha(0xFF);
	}

	if (m_it != m_Text.end()) {
		m_it++;
	}
	else {
		for (UINT n = 0; n < 16; n++) {
			m_Surface->GetMarkSprite(n).setAlpha(0);
		}
		m_Surface->GetMarkSprite((m_MarkIndex / 3) % 16).setAlpha(0xFF);
		m_MarkIndex++;
	}

	// スプライトの統合イメージを取得する
	mxDibSection newImage;;
	m_Surface->CompositeAndScaling(newImage);


	std::wstring str(m_Text.begin(), m_it);
	m_Surface->GetTextSprite().DrawString(str, newImage);

	// サーフェスに描画する
	newImage.bitBlt(*m_Surface, m_Surface->GetPaintRect());

	return m_Ret;
}

void ScnText::onLButtonDown(
		mxWindow *pWnd,
		BOOL fDoubleClick,
		const mxPoint &pt,
		UINT keyFlags)
{
	if (m_it == m_Text.end()) {
		m_Ret = Scene::END;
	}
}
