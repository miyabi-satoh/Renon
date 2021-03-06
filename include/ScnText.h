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
class Config;

///////////////////////////////////////////////////////////////////////////////
/// テキスト描画のシーン
///
class ScnText: public Scene {
public:
	static void Create(std::wstring s, Surface *pSurface, Config *pConfig);

	~ScnText();

protected:
	int 	UpdateFrame (UINT nFrameCount);
	void 	onLButtonDown (mxWindow *pWnd, BOOL fDoubleClick, const mxPoint &pt, UINT keyFlags);

private:
	ScnText(std::wstring s, Surface *pSurface, Config *pConfig);

	Config *m_Config;	///< 設定管理クラス
	Surface *m_Surface;	///< 描画サーフェス

	int m_Ret;	///< UpdateFrameの戻り値

	std::wstring m_Text;			///< テキスト
	std::wstring m_CastName;		///< 発言者名
	std::wstring::iterator m_it;	///< テキスト中の現在位置
	UINT m_MarkIndex;				///< 改頁マークのインデックス
};

#endif /* SCNTEXT_H_ */
