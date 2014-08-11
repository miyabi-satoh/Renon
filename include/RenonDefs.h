///////////////////////////////////////////////////////////////////////////////
/// @file	RenonDefs.h
/// @date	2014/07/27	
/// @author	Masayuki
/// @brief	各種定数
///////////////////////////////////////////////////////////////////////////////

#ifndef RENONDEFS_H_
#define RENONDEFS_H_

#define MainWindowClassName	_T("RenonMainWindow")
#define AppName	_T("Recollection of \"Kanon\"")
#define AppVersion	_T("2.1.0")

// 設定関係 ////////////////////////////////////////////////////////////////////
#define IniFileName		_T("Renon.ini")
#define IniFileSection		_T("Renon")
// キー名
#define Key_KanonPath		_T("KanonPath")
#define Key_FontName		_T("FontName")
#define Key_PlayerName1	_T("PlayerName1")
#define Key_PlayerName2	_T("PlayerName2")
#define Key_WindowLeft		_T("WindowLeft")
#define Key_WindowTop		_T("WindowTop")
#define Key_LastScript		_T("LastScript")
// デフォルト値
#define DefaultFontName	_T("ＭＳ ゴシック")
#define DefaultPlayerName1	_T("相沢")
#define DefaultPlayerName2	_T("祐一")

// スクリプト関係 //////////////////////////////////////////////////////////////
#define SYSTEM_DAT		_T("System.dat")
#define BOOTSTRAP_SCN	_T("/bootstrap.scn")
#define MENU_SCN		_T("/menu.scn")
#define EXIT_SCN		_T("/exit.scn")
#define CR		L'\r'
#define LF		L'\n'
#define TAB	L'\t'
#define NUL	L'\0'
#define FILTER_SCN	_T("シナリオファイル(*.scn)|*.scn|すべてのファイル(*.*)|*.*||")

// メッセージ関係 //////////////////////////////////////////////////////////////
#define WM_IDLE		(WM_APP + 100)
#define WM_LOAD_SCRIPT	(WM_APP + 101)

// タイマー関係 ////////////////////////////////////////////////////////////////
#define TIMER_REPLAY	1
#define TIMER_FPS		2

// シーン関係 //////////////////////////////////////////////////////////////////
#define TRANS_DOTTILE	L"DT"
#define TRANS_FADEIN	L"FI"
#define TRANS_ACCT2B	L"AT"
#define TRANS_ACCB2T	L"AB"
#define TRANS_ACCL2R	L"AL"
#define TRANS_ACCR2L	L"AR"

// 雪 //////////////////////////////////////////////////////////////////////////
#define NUM_SNOW	20
#define PI			3.14159265

#endif /* RENONDEFS_H_ */
