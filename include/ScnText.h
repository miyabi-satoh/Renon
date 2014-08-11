///////////////////////////////////////////////////////////////////////////////
/// @file	ScnText.h
/// @date	2014/08/01	
/// @author	Masayuki
/// @brief	class ScnText
///////////////////////////////////////////////////////////////////////////////

#ifndef SCNTEXT_H_
#define SCNTEXT_H_

#include <Scene.h>
class Surface;

///////////////////////////////////////////////////////////////////////////////
/// テキスト描画のシーン
///
class ScnText: public Scene {
public:
	static void Create(std::wstring s, Surface *pSurface);

	~ScnText();

protected:
	int 	UpdateFrame (UINT nFrameCount);
	void 	onLButtonDown (mxWindow *pWnd, BOOL fDoubleClick, const mxPoint &pt, UINT keyFlags);

private:
	ScnText(std::wstring s, Surface *pSurface);

	Surface *m_Surface;	///< 描画サーフェス

	int m_Ret;	///< UpdateFrameの戻り値

	std::wstring m_Text;			///< テキスト
	std::wstring m_CastName;		///< 発言者名
	std::wstring::iterator m_it;	///< テキスト中の現在位置
};

#endif /* SCNTEXT_H_ */
