///////////////////////////////////////////////////////////////////////////////
/// @file	RenonApp.cpp
/// @date	2014/07/27	
/// @author	Masayuki
/// @brief	Renonアプリケーションクラス
///////////////////////////////////////////////////////////////////////////////

#include "RenonApp.h"
#include "MainWindow.h"
#include "resource.h"
#include "RenonDefs.h"
#include "ConfigDialog.h"
#include <dibsection.hpp>

IMPLEMENT_APP(RenonApp)

void RenonApp::initialize ()
{
	// ２重起動の防止
	mxAutoHandle hMutex(::CreateMutex(NULL, FALSE, MainWindowClassName));
	if (::GetLastError() == ERROR_ALREADY_EXISTS) {
		::PostQuitMessage(0);
		return;
	}

	// GDI+を使う
	USE_GDIPLUS;

	m_pMainWnd = new MainWindow(&m_Config, &m_ResourceManager);

	// 設定を読み込む
	m_Config.Load();
	try {
		m_Config.Validate();
	}
	catch (const mxException &e) {
		m_pMainWnd->messageBox(
				_T("初期設定を行ってください"),
				_T("確認"),
				MB_OK | MB_ICONINFORMATION);

		ConfigDialog dlg(&m_Config, m_pMainWnd);
		if (dlg.doModal() == IDCANCEL) {
			::PostQuitMessage(0);
			return;
		}

	}

	// ウィンドウクラスを登録する
	mxRegisterWndClass(
			MainWindowClassName,
			mxLoadIcon(IDI_ICON));

	// メインウィンドウを作成、表示する
	m_pMainWnd->create(MainWindowClassName);
	m_pMainWnd->show(SW_SHOW);
	m_pMainWnd->update();
}

void RenonApp::finalize ()
{
	delete m_pMainWnd;
	mxTrace(_T("----- Program END -----"));
}

void RenonApp::doIdle ()
{
	m_pMainWnd->sendMessage(WM_IDLE);
}

bool RenonApp::preTranslateMessage (MSG *lpMsg)
{
	return false;
}
