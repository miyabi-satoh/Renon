///////////////////////////////////////////////////////////////////////////////
/// @file	MainWindow.cpp
/// @date	2014/07/27	
/// @author	Masayuki
/// @brief	メインウィンドウクラス
///////////////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "MainWindow.h"
#include "resource.h"
#include "RenonDefs.h"
#include "Config.h"
#include "AboutDialog.h"
#include "ConfigDialog.h"
#include "ResourceManager.h"
#include "ScnTransition.h"
#include "ScnSnow.h"
#include "ScnTitle.h"
#include "ScnText.h"
#include "TextSprite.h"
#include <menu.hpp>
#include <dc.hpp>
#include <file.hpp>
#include <findfile.hpp>

typedef int (CALLBACK *LP_SETNKFOPTION)(LPSTR);
typedef void (CALLBACK *LP_NKFCONVERT)(LPSTR, LPCSTR);
UINT g_FpsCount = 0;

inline bool isEOL(const WCHAR *p)
{
	return (*p == NUL || *p == LF);
}

bool MainWindow::handleMessage (
		LRESULT *lRes,
		UINT uMsg,
		WPARAM wParam,
		LPARAM lParam)
{
	switch (uMsg) {
	case WM_IDLE:
		return onIdle();

	case WM_LOAD_SCRIPT:
		return onLoadScript(wParam, reinterpret_cast<LPCTSTR>(lParam));

	}
	return mxWindow::handleMessage(lRes, uMsg, wParam, lParam);
}

bool MainWindow::onCommand (int id, HWND hwndCtl, UINT codeNotify)
{
	switch (id) {
	case IDR_MNU_TITLE:	// タイトル画面へ戻る
		Scene::Clear();
		ScnSnow::Create(&m_Surface, *m_ResMgr);
		LoadSystemScript(MENU_SCN);
		return true;

	case IDR_MNU_QUIT:	// 終了
		sendMessage(WM_CLOSE);
		return true;

	case IDR_MNU_FRAME1:	// ウィンドウ枠 パターン1
	case IDR_MNU_FRAME2:	// ウィンドウ枠 パターン2
	case IDR_MNU_FRAME3:	// ウィンドウ枠 パターン3
	case IDR_MNU_FRAME4: {	// ウィンドウ枠 パターン4
		mxMenu mainMenu(getMenu());
		mainMenu.checkRadioItem(IDR_MNU_FRAME1, IDR_MNU_FRAME4, id);
		TextSprite::SetFrameIndex(id - IDR_MNU_FRAME1);
		return true; }

	case IDR_MNU_CONFIG: {	// 環境設定
		ConfigDialog dlg(m_Config, this);
		dlg.doModal();
		return true; }

	case IDR_MNU_VERSION: {	// バージョン情報
		AboutDialog dlg(this);
		dlg.doModal();
		return true; }
	}

	return mxWindow::onCommand(id, hwndCtl, codeNotify);
}

bool MainWindow::onCreate (LRESULT *lRes, CREATESTRUCT *lpcs)
{
	setStyle(WS_POPUPWINDOW | WS_CAPTION | WS_MINIMIZEBOX);
	setText(AppName);
	// メニュー設定
	mxMenu mainMenu(mxLoadMenu(IDR_MENU_MAIN));
	// 「タイトル画面へ戻る」はスクリプトを読むまで使用不可にする
	mainMenu.enableItem(IDR_MNU_TITLE, MF_GRAYED, MF_BYCOMMAND);
	setMenu(mainMenu);

	// ウィンドウのサイズを計算する
	mxRect rc(0, 0, 640, 480);
//	mxRect rc(0, 0, 940, 680);
	adjustWindowRect(&rc);

	// ウィンドウの位置を計算する
	mxPoint pt(m_Config->GetWindowPos());
	if (pt.x == CW_USEDEFAULT || pt.y == CW_USEDEFAULT) {
		mxRect rcDesktop;
		::SystemParametersInfo(SPI_GETWORKAREA, 0, &rcDesktop, 0);
		pt.x = (rcDesktop.right - rc.width()) / 2;
		pt.y = (rcDesktop.bottom - rc.height()) / 2;
	}

	move(pt, rc.size(), FALSE);

	// BGMリプレイ用のタイマーをセットする
	setTimer(TIMER_REPLAY, 1000);
	// FPS用のタイマーをセットする
	setTimer(TIMER_FPS, 10000);

	ScnSnow::Create(&m_Surface, *m_ResMgr);
	LoadSystemScript(BOOTSTRAP_SCN);

	return true;
}

bool MainWindow::onDestroy ()
{
	// タイマーの終了
	killTimer(TIMER_REPLAY);
	killTimer(TIMER_FPS);

	// ウィンドウ位置の保存
	m_Config->SetWindowPos(getRect().topLeft());
	m_Config->Save();

	// シーンのクリア
	Scene::Clear();

	// サウンドの停止
	m_Bgm.stop();
	m_Se.stop();
	return mxWindow::onDestroy();
}

bool MainWindow::onEraseBkgnd (LRESULT *lRes, HDC hdc)
{
	*lRes = TRUE;
	return true;
}

bool MainWindow::onLButtonDown (BOOL fDoubleClick, const mxPoint &pt, UINT keyFlags)
{
	Scene::OnLButtonDown(this, fDoubleClick, pt, keyFlags);
	return true;
}

bool MainWindow::onMouseMove (const mxPoint &pt, UINT keyFlags)
{
	Scene::OnMouseMove(this, pt, keyFlags);
	return true;
}

bool MainWindow::onPaint (mxPaintDC *pDC)
{
	g_FpsCount++;

	mxRect rcPaint = pDC->getPaintRect();
	m_Surface.bitBlt(*pDC, rcPaint, rcPaint.topLeft());

	return true;
}

bool MainWindow::onSize (UINT state, const mxSize &sz)
{
	if (m_Surface.getSize() != sz) {
		m_Surface.Resize(sz);
		Scene::OnSize(this, state, sz);
	}
	return true;
}

bool MainWindow::onTimer (UINT id)
{
	static DWORD dwBgmPosition = 0;

	switch (id) {
	case TIMER_REPLAY:
		try {
			DWORD dwRet = m_Bgm.getStatus(MCI_STATUS_POSITION);
			if (dwRet == dwBgmPosition) {
				m_Bgm.stop();
				PlayBGM(m_BgmLoopCount - 1, m_BgmTrackNo);
			}
			dwBgmPosition = dwRet;
		}
		catch (const mxException &e) {
			mxTrace(e.what());
		}
		return true;

	case TIMER_FPS:
		mxTraceR((g_FpsCount / 10) << _T("FPS"));
		g_FpsCount = 0;
		return true;
	}

	return mxWindow::onTimer(id);
}

bool MainWindow::onMciNotify (UINT nFlags, MCIDEVICEID devID)
{
	switch (nFlags) {
	case MCI_NOTIFY_SUCCESSFUL:
		if (devID == m_Bgm.getDeviceId()) {
			PlayBGM(m_BgmLoopCount - 1, m_BgmTrackNo);
		}
		return true;
	}

	return mxWindow::onMciNotify(nFlags, devID);
}

bool MainWindow::onIdle()
{
	static UINT nTime = ::timeGetTime();
	const UINT nWaitTime = 1000 / 30;	// 30FPS

	if (Scene::onUpdate() == 0) {
		ParseNextLine();
	}

	do {
		::Sleep(1);
	} while (::timeGetTime() - nTime < nWaitTime);

	nTime = ::timeGetTime();

	invalidateRect(m_Surface.GetPaintRect(), FALSE);
	return true;
}

bool MainWindow::onLoadScript(BOOL bSystem, LPCTSTR lpszName)
{
	if (bSystem) {
		LoadSystemScript(lpszName);
	}
	else {
		LoadScript(lpszName);
		m_Config->SetLastScript(lpszName);
		m_Config->Save();
		mxMenu mainMenu(getMenu());
		mainMenu.enableItem(IDR_MNU_TITLE, MF_ENABLED, MF_BYCOMMAND);
	}
	return true;
}

void MainWindow::LoadScript(LPCTSTR lpszName)
{
//	mxTrace(lpszName);
	mxString strName = m_ResMgr->GetResourcePath(lpszName);
	if (strName.empty()) {
		mxTrace(lpszName << _T(" ---> Not found !"));
		return;
	}
//	mxTrace(_T("--->") << strName.c_str());

	mxFile inFile(strName.c_str(),
			mxFile::modeRead | mxFile::shareRead | mxFile::openExisting);

	// ファイルサイズ＋αのバッファを確保する
	UINT nSize = inFile.getSize();
	CharBuffer vBuffer(nSize + 32, 0);

	// 読み込み
	inFile.seekToBegin();
	inFile.read(&vBuffer[0], nSize);

	// Unicodeバッファを確保する
	m_CurPtr = NULL;
	m_vBuffer.clear();
	m_vBuffer.resize(nSize * 2 + 32, 0);

	mxAutoClose<HMODULE> nkf32(::LoadLibrary(_T("nkf32.dll")));
	LP_SETNKFOPTION SetNkfOption = reinterpret_cast<LP_SETNKFOPTION>(
			::GetProcAddress(nkf32, "SetNkfOption"));
	LP_NKFCONVERT NkfConvert = reinterpret_cast<LP_NKFCONVERT>(
			::GetProcAddress(nkf32, "NkfConvert"));
	if (SetNkfOption && NkfConvert) {
		SetNkfOption(const_cast<LPSTR>("-w16L0"));
		NkfConvert(&m_vBuffer[0], &vBuffer[0]);
	}
	else {
		::MultiByteToWideChar(
				CP_ACP, 0, &vBuffer[0], -1,
				reinterpret_cast<WCHAR*>(&m_vBuffer[0]),
				m_vBuffer.size() / sizeof(WCHAR));
	}
	m_CurPtr = reinterpret_cast<WCHAR*>(&m_vBuffer[0]);

	// '\r' -> '\n'に変換する
	// '\t' -> ' 'に変換する
	WCHAR *p;
	for (p = m_CurPtr; *p != NUL; p++) {
		if (*p == CR) {
			*p = LF;
		}
		else if (*p == TAB) {
			*p = L' ';
		}
	}

	// 範囲コメントの処理
	for (p = m_CurPtr; *p != NUL; p++) {
		if (*p == L'/' && *(p + 1) == L'*') {
			// 次の*/まで' 'に置換する
			while (*p != NUL && !(*p == L'*' && *(p + 1) == L'/')) {
				*p++ = ' ';
			}
			// "*/"も消さないとね
			*p++ = ' ';
			*p++ = LF;	// 行頭扱いにする
		}
	}

	// 行コメントの処理
	for (p = m_CurPtr; *p != NUL; p++) {
		if (*p == L'/' && *(p + 1) == L'/') {
			// 行末まで' 'に置換する
			while (!isEOL(p)) {
				*p++ = L' ';
			}
		}
	}

	// セクション(-----)を見つけて、ジャンプメニューを初期化する
	mxMenu mainMenu(getMenu());
	mxMenu subMenu(mainMenu.getSubMenu(1));

	int menuPos = 0;
	// すでにあるメニュー項目を削除する
	while (subMenu.getItemCount()) {
		subMenu.deleteItem(0, MF_BYPOSITION);
	}

	mxZero<MENUITEMINFO> mii;
	mii->cbSize = sizeof(MENUITEMINFO);
	mii->fMask = MIIM_ID | MIIM_STRING | MIIM_DATA;

	p = m_CurPtr;
	WCHAR *pTmp;
	while (*p != NUL) {
		// 行頭の"-"を見つける
		if (*p == L'-') {
			while (*p == L'-') { p++; }	// 連続する'-'をスキップ
			while (*p == L' ') { p++; }	// 連続する' 'をスキップ

			pTmp = p;
			while (!isEOL(p)) { p++; }

			std::wstring str(pTmp, p);
			if (str.length() > 0) {
				mii->wID = IDR_MNU_JUMP + menuPos + 1;
				mii->dwTypeData = const_cast<LPTSTR>(str.c_str());
				mii->dwItemData = reinterpret_cast<ULONG_PTR>(p + 1);
				subMenu.insertItem(menuPos++, mii.get(), MF_BYPOSITION);
			}
		}
		// 次の行へ
		while (!isEOL(p)) { p++; }
		p++;
	}

	UINT uEnable = (menuPos == 0) ? MF_GRAYED : MF_ENABLED;
	mainMenu.enableItem(1, uEnable, MF_BYPOSITION);
	drawMenuBar();
}

void MainWindow::LoadSystemScript(LPCTSTR lpszName)
{
	m_ResMgr->SetCurrentStore(
			mxPathCombine(mxGetSelfFolderPath().c_str(), SYSTEM_DAT));
	LoadScript(lpszName);
	// 「タイトル画面へ戻る」はスクリプトを読むまで使用不可にする
	mxMenu mainMenu(getMenu());
	mainMenu.enableItem(IDR_MNU_TITLE, MF_GRAYED, MF_BYCOMMAND);
	drawMenuBar();
}

void MainWindow::ParseNextLine()
{
	if (m_CurPtr == NULL) {
		return;
	}
	else if (*m_CurPtr == L'\0') {
		LoadSystemScript(MENU_SCN);
		return;
	}

	while (*m_CurPtr != L'\0') {
		// 空白文字を読み飛ばす
		while (*m_CurPtr == L' ' || *m_CurPtr == L'\n') { m_CurPtr++; }

		// 行末を探す
		WCHAR *pEol = m_CurPtr;
		while (!isEOL(pEol)) { pEol++; }

		if (*m_CurPtr == L'-') {			// セクション区切り -> 無視
		}
		else if (*m_CurPtr == L'=') {	// ウィンドウタイトルの変更
			while (*m_CurPtr == L'=') { m_CurPtr++; }
			while (*m_CurPtr == L' ') { m_CurPtr++; }

			mxString strWindowTitle(AppName);
			std::wstring strTitle(m_CurPtr, pEol);
			if (strTitle.length() > 0) {
				strWindowTitle += _T(" - ") + strTitle;
			}
			mxTrace("Set window title");
			setText(strWindowTitle.c_str());
		}
		else if (*m_CurPtr == L'<') {	// ウェイト命令
			m_CurPtr++;
			int nWaitTime = ParseNextInt(0);
			if (nWaitTime > 0) {
				mxTrace(_T("Wait ") << nWaitTime << _T("ms"));
				ScnWait::Create(&m_Surface, nWaitTime);
				// 次回のために次の行頭へ
				m_CurPtr = pEol + 1;
				return;
			}
		}
		else if (*m_CurPtr == L'$') {	// SEの再生
			m_CurPtr++;
			std::wstring strFile = ParseNextString();
			if (strFile.length() > 0) {
				PlaySE(1, strFile.c_str());
			}

		}
		else if (*m_CurPtr == L'#') {	// BGMの再生・停止
			int count = 0;
			while (*m_CurPtr == L'#') { m_CurPtr++; count++; }

			int nTrack = ParseNextInt(0);
			if (nTrack > 0) {
				mxTrace(_T("Play BGM"));
				int nLoop = (count == 1) ? -1 : count - 1;
				PlayBGM(nLoop, nTrack);
			}
			else {
				mxTrace(_T("Stop BGM"));
				StopBGM(count == 1);
			}
		}
		else if (*m_CurPtr == L'*') {
			m_CurPtr++;
			std::wstring str = ParseNextString();
			if (str == L"MENU") {
				ScnTitle::Create(&m_Surface, m_Config, *m_ResMgr);
				// 次回のために次の行頭へ
				m_CurPtr = pEol + 1;
				return;
			}
			else if (str == L"EXIT") {
				mxTrace(_T("Exit"));
				sendMessage(WM_CLOSE);
				// 次回のために次の行頭へ
				m_CurPtr = pEol + 1;
				return;
			}
		}
		else if (*m_CurPtr == L'+') {	// レイヤーの固定
			m_CurPtr++;
			std::wstring str = ParseNextString();
			if (str == L"BC") {
				// 背景レイヤーとキャラクタレイヤーを統合する
				m_Surface.GetCharSprite().render(m_Surface.GetBackSprite());
				m_Surface.GetCharSprite().destroy();
			}
		}
		else if (*m_CurPtr == L'@') {	// 画像の読み込み
			m_CurPtr++;
			if (*m_CurPtr == L'B') {	// 背景
				m_CurPtr++;
				std::wstring strFile = ParseNextString();
				strFile = m_ResMgr->GetResourcePath(strFile.c_str());
				if (strFile.empty()) {
					m_Surface.GetBackSprite().destroy();
				}
				else {
//					mxTrace(_T("Load background"));
					m_Surface.GetBackSprite().setAlpha(0xFF);
					m_Surface.LoadBackground(strFile.c_str());
				}
			}
			else if (*m_CurPtr == L'C') {	// キャラクタ
				m_CurPtr++;
				std::wstring strFile = ParseNextString();
				strFile = m_ResMgr->GetResourcePath(strFile.c_str());
				if (strFile.empty()) {
					m_Surface.GetCharSprite().destroy();
				}
				else {
//					mxTrace(_T("Load character"));
					m_Surface.GetCharSprite().setAlpha(0xFF);
					m_Surface.LoadCharacter(strFile.c_str());
				}
			}
			else if (*m_CurPtr == L'S') {	// 特殊演出
				m_CurPtr++;
				std::wstring strID = ParseNextString();
				if (strID.length() > 0) {
//					m_SceneList.push_back(ScenePtr(new ScnSpecial(strID.c_str())));
					// 次回のために次の行頭へ
					m_CurPtr = pEol + 1;
					return;
				}
			}
			else {	// 読み飛ばす
				m_CurPtr = pEol;
			}

			// トランジションも指定？
			if (ParseTransition()) {
				// 次回のために次の行頭へ
				m_CurPtr = pEol + 1;
				return;
			}
		}
		else if (*m_CurPtr == L'!') {	// トランジション
			m_CurPtr++;
			if (ParseTransition()) {
				// 次回のために次の行頭へ
				m_CurPtr = pEol + 1;
				return;
			}
		}
		else if (*m_CurPtr == L'&') {	//  非同期効果
			m_CurPtr++;
			std::wstring strObject = ParseNextString();
			std::wstring strAction = ParseNextString();
			if (strObject == L"SNOW") {
				if (strAction == L"START") {
					ScnSnow::SetState(ScnSnow::START);
				}
				else if (strAction == L"END") {
					ScnSnow::SetState(ScnSnow::END);
				}
				else if (strAction == L"PAUSE") {
					ScnSnow::SetState(ScnSnow::PAUSE);
				}
				else if (strAction == L"DELETE") {
					ScnSnow::SetState(ScnSnow::REMOVE);
				}
				else if (strAction == L"FADEIN") {
					ScnSnow::SetState(ScnSnow::FADEIN);
				}
				else if (strAction == L"FADEOUT") {
					ScnSnow::SetState(ScnSnow::FADEOUT);
				}
			}
		}
		else if (*m_CurPtr == L'>') {	// スクリプト読み込み
			m_CurPtr++;
			LoadScript(ParseNextString().c_str());
			return;
		}
		else {
			std::wstring str(m_CurPtr, pEol);
			if (str.length() > 0) {
				// プレイヤー名を置換する
				boost::algorithm::replace_all(str, DefaultPlayerName1, m_Config->GetPlayerName1());
				boost::algorithm::replace_all(str, DefaultPlayerName2, m_Config->GetPlayerName2());

				ScnText::Create(str, &m_Surface);
//				m_SceneList.push_back(ScenePtr(new ScnText(str.c_str())));
				// 次回のために次の行頭へ
				m_CurPtr = pEol + 1;
				return;
			}
		}
		// 次の行へ
		m_CurPtr = pEol + 1;
	}
}

std::wstring MainWindow::ParseNextString()
{
	// 空白文字を読み飛ばす
	while (*m_CurPtr == L' ') { m_CurPtr++; }

	WCHAR *p = m_CurPtr;
	// 次の空白文字または行末までスキップ
	while (!isEOL(m_CurPtr) && *m_CurPtr != L' ') { m_CurPtr++; }

	return std::wstring(p, m_CurPtr);
}

int MainWindow::ParseNextInt(int nDefault)
{
	int nRet = nDefault;

	// 空白文字を読み飛ばす
	while (*m_CurPtr == L' ') { m_CurPtr++; }

	WCHAR *p = m_CurPtr;
	while (::iswdigit(*m_CurPtr)) { m_CurPtr++; }

	if (p != m_CurPtr) {
		std::wstring str(p, m_CurPtr);
		nRet = ::wcstol(str.c_str(), NULL, 10);
	}

	// 非空白文字を読み飛ばす
	while (!isEOL(m_CurPtr) && *m_CurPtr != L' ') { m_CurPtr++; }

	return nRet;
}

bool MainWindow::ParseTransition()
{
	std::wstring strTrans = ParseNextString();
	int nOption = ParseNextInt(1);
	if (nOption <= 0) { nOption = 1; }

	ScnTransition::Create(&m_Surface, strTrans, nOption);
//	m_SceneList.push_back(ScenePtr(new ScnTransition(strTrans, nOption)));

	return true;
}
void MainWindow::PlaySE(int nLoop, LPCTSTR lpszName)
{
	m_Se.stop();
	if (nLoop == 0) {
		return;
	}

	mxString strName = m_ResMgr->GetResourcePath(lpszName);
	if (strName.empty()) {
		return;
	}

	try {
		m_Se.play(strName.c_str());
	}
	catch (const mxException &e) {
		mxTrace(e.what());
	}
}

void MainWindow::PlayBGM(int nLoop, int nTrackNo)
{
	if (m_Bgm.getStatus(MCI_STATUS_MODE) == MCI_MODE_PLAY &&
		nTrackNo == m_BgmTrackNo)
	{
		// すでに再生中なので、ループカウントだけ更新する
		m_BgmLoopCount = nLoop - 1;
		return;
	}

	m_Bgm.stop();
	if (nLoop == 0) {
		return;
	}

	// MP3 -> WAVの順に再生を試みる
	LPCTSTR lpszExt[] = {
		_T("mp3"),
		_T("wav"),
	};
	mxString strBGMDir;

	strBGMDir = mxPathCombine(mxGetSelfFolderPath().c_str(), _T("BGM"));
	for (int i = 0; i < 2; i++) {
		TCHAR szSearchMask[MAX_PATH];
		::_stprintf(szSearchMask, _T("*%02d*.%s"), nTrackNo, lpszExt[i]);

		mxString strPath = mxPathCombine(strBGMDir.c_str(), szSearchMask);
		mxFindFile ff(strPath.c_str());
		while (ff.next()) {
			try {
				m_Bgm.play(ff.getFilePath().c_str());
				// 再生成功
				m_BgmTrackNo = nTrackNo;
				m_BgmLoopCount = nLoop;
				return;
			}
			catch (const mxException &e) {
				mxTrace(_T("BGM play failed"));
			}
		}
	}

	//
	// イチかバチか、CDDAに期待
	//

	// 利用可能なドライブ名の列挙
	TCHAR szDrive[MAX_PATH] = _T("");
	::GetLogicalDriveStrings(MAX_PATH - 1, szDrive);

	for (TCHAR *pDrive = szDrive; *pDrive != '\0'; pDrive = _tcschr(pDrive, '\0') + 1)
	{
		// そのドライブがCD-ROMであるかを調べる
		unsigned int uiType = ::GetDriveType(pDrive);
		if (DRIVE_CDROM != uiType) {
			continue;
		}
		// ディスクが挿入されているか
		ULARGE_INTEGER iFreeBytesAvailable;
		ULARGE_INTEGER iTotalNumberOfBytes;
		if (!::GetDiskFreeSpaceEx(pDrive, &iFreeBytesAvailable, &iTotalNumberOfBytes, NULL)) {
			continue;
		}

		// Kanon.ico があるかチェック
		TCHAR szIconPath[MAX_PATH];
		::_stprintf(szIconPath, _T("%sKanon.ico"), pDrive);

		if (!::PathFileExists(szIconPath)) {
			continue;
		}

		try {
			m_Bgm.play(pDrive[0], nTrackNo);
			// 再生成功
			m_BgmTrackNo = nTrackNo;
			m_BgmLoopCount = nLoop;
			return;
		}
		catch (const mxException &e) {
#ifdef _DEBUG
			::_tprintf(e.what());
#endif
		}
	}
}

void MainWindow::StopBGM(bool bFade)
{
	if (bFade) {
		// ミキサーデバイスの数を取得する
		UINT nMixerDevs = ::mixerGetNumDevs();
		if (nMixerDevs > 0) {
			MMRESULT mmr;
			// ミキサーデバイスをオープンする
			mxAutoClose<HMIXER> hMixer;
			mmr = ::mixerOpen(*hMixer, 0, 0, 0, MIXER_OBJECTF_MIXER);
			// ※エラーでbreakしたいがためのwhile
			//   ループ処理ではない
			while (mmr == MMSYSERR_NOERROR) {
				// ライン情報を取得する
				MIXERLINE mxl;
				mxl.cbStruct = sizeof(MIXERLINE);
				mxl.dwComponentType = MIXERLINE_COMPONENTTYPE_DST_SPEAKERS;
				mmr = ::mixerGetLineInfo(
					reinterpret_cast<HMIXEROBJ>(hMixer.get()),
					&mxl,
					MIXER_GETLINEINFOF_COMPONENTTYPE | MIXER_OBJECTF_HMIXER);
				if (mmr != MMSYSERR_NOERROR) {
					break;
				}

				// ラインコントロールを取得する
				MIXERLINECONTROLS mxlc;
				MIXERCONTROL mxc;
				mxlc.cbStruct = sizeof(MIXERLINECONTROLS);
				mxlc.dwLineID = mxl.dwLineID;
				mxlc.dwControlType = MIXERCONTROL_CONTROLTYPE_VOLUME;
				mxlc.cControls = 1;
				mxlc.cbmxctrl = sizeof(MIXERCONTROL);
				mxlc.pamxctrl = &mxc;
				mmr = ::mixerGetLineControls(
					reinterpret_cast<HMIXEROBJ>(hMixer.get()),
					&mxlc,
					MIXER_GETLINECONTROLSF_ONEBYTYPE | MIXER_OBJECTF_HMIXER);
				if (mmr != MMSYSERR_NOERROR) {
					break;
				}
				DWORD minVol = mxc.Bounds.dwMinimum;	// 最小音量

				// 音量を取得する
				MIXERCONTROLDETAILS mxcd;
				MIXERCONTROLDETAILS_UNSIGNED mxcdVolume;
				mxcd.cbStruct = sizeof(MIXERCONTROLDETAILS);
				mxcd.dwControlID = mxc.dwControlID;
				mxcd.cChannels = 1;
				mxcd.cMultipleItems = 0;
				mxcd.cbDetails = sizeof(MIXERCONTROLDETAILS_UNSIGNED);
				mxcd.paDetails = &mxcdVolume;
				mmr = ::mixerGetControlDetails(
					reinterpret_cast<HMIXEROBJ>(hMixer.get()),
					&mxcd,
					MIXER_GETCONTROLDETAILSF_VALUE | MIXER_OBJECTF_HMIXER);
				if (mmr != MMSYSERR_NOERROR) {
					break;
				}
				DWORD curVol = mxcdVolume.dwValue;	// 現在音量
				DWORD difVol = curVol - minVol;

				DWORD dwTime = ::timeGetTime();
				double per;
				while ((per = (1.0 - ((::timeGetTime() - dwTime) / 1000.0))) > 0) {
					mxcdVolume.dwValue = minVol + static_cast<DWORD>(difVol * per);
					::mixerSetControlDetails(
						reinterpret_cast<HMIXEROBJ>(hMixer.get()),
						&mxcd,
						MIXER_SETCONTROLDETAILSF_VALUE | MIXER_OBJECTF_HMIXER);
					::Sleep(1);
				}

				// 停止
				m_Bgm.stop();

				// ボリュームを元に戻す
				mxcdVolume.dwValue = curVol;
				::mixerSetControlDetails(
					reinterpret_cast<HMIXEROBJ>(hMixer.get()),
					&mxcd,
					MIXER_SETCONTROLDETAILSF_VALUE | MIXER_OBJECTF_HMIXER);
				break;
			}
		}
	}

	m_Bgm.stop();
	m_BgmTrackNo = 0;
}

