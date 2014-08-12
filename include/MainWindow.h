///////////////////////////////////////////////////////////////////////////////
/// @file	MainWindow.h
/// @date	2014/07/27	
/// @author	Masayuki
/// @brief	メインウィンドウクラス
///////////////////////////////////////////////////////////////////////////////

#ifndef MAINWINDOW_H_
#define MAINWINDOW_H_

#include <window.hpp>
#include <mci.hpp>
#include "Scene.h"
#include "Surface.h"

class Config;
class ResourceManager;

///////////////////////////////////////////////////////////////////////////////
/// メインウィンドウクラス
///
class MainWindow: public mxWindow {
public:
	/// コンストラクタ
	MainWindow(Config *pConfig, ResourceManager *pResMgr)
		: mxWindow(), m_vBuffer(), m_Bgm(), m_Surface(pConfig, *pResMgr)
	{
		m_Config = pConfig;
		m_ResMgr = pResMgr;
		m_CurPtr = NULL;
		m_BgmLoopCount = 0;
		m_BgmTrackNo = 0;
	}
	/// デストラクタ
	~MainWindow() {
	}
protected:
	/// @name mxWindowからの継承
	//@{
	bool 	handleMessage (LRESULT *lRes, UINT uMsg, WPARAM wParam, LPARAM lParam);
	bool 	onCommand (int id, HWND hwndCtl, UINT codeNotify);
	bool 	onCreate (LRESULT *lRes, CREATESTRUCT *lpcs);
	bool 	onDestroy ();
	bool 	onEraseBkgnd (LRESULT *lRes, HDC hdc);
	bool 	onLButtonDown (BOOL fDoubleClick, const mxPoint &pt, UINT keyFlags);
	bool 	onMouseMove (const mxPoint &pt, UINT keyFlags);
	bool 	onPaint (mxPaintDC *pDC);
	bool 	onSize (UINT state, const mxSize &sz);
	bool 	onTimer (UINT id);
	bool 	onMciNotify (UINT nFlags, MCIDEVICEID devID);
	//@}

	/// @name 独自のメッセージハンドラ
	//@{
	bool onIdle();
	bool onLoadScript(BOOL bSystem, LPCTSTR lpszName);
	//@}

private:
	Config *m_Config;					///< 設定管理クラス
	ResourceManager *m_ResMgr;	///< リソース管理クラス

	// スクリプト関係 //////////////////////////////////////////////////////////
	typedef std::vector<char> CharBuffer;

	CharBuffer m_vBuffer;		///< スクリプトファイルバッファ
	WCHAR *m_CurPtr;			///< ↑での現在位置

	void LoadScript(LPCTSTR lpszName);
	void LoadSystemScript(LPCTSTR lpszName);
	void ParseNextLine();
	std::wstring ParseNextString();
	int ParseNextInt(int nDefault);
	bool ParseTransition();

	// BGM関係 /////////////////////////////////////////////////////////////////
	mxMCI m_Bgm;
	int m_BgmLoopCount;
	int m_BgmTrackNo;

	void PlayBGM(int nLoop, int nTrackNo);
	void StopBGM(bool bFade);

	// SE関係 //////////////////////////////////////////////////////////////////
	mxMCI m_Se;

	void PlaySE(int nLoop, LPCTSTR lpszName);

	// 描画関係 ////////////////////////////////////////////////////////////////
	Surface m_Surface;

};

#endif /* MAINWINDOW_H_ */
