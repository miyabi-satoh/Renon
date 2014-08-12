///////////////////////////////////////////////////////////////////////////////
/// @file	Config.cpp
/// @date	2014/07/29	
/// @author	Masayuki
/// @brief	設定クラス
///////////////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "RenonDefs.h"
#include "Config.h"

Config::Config()
	: mxIniFile(IniFileName), m_KanonDir(), m_FontName(),
	  m_PlayerName1(), m_PlayerName2(), m_LastScript(), m_WindowPos()
{
}

void Config::Load()
{
	m_KanonDir = read(IniFileSection, Key_KanonPath, _T(""));
	m_FontName = read(IniFileSection, Key_FontName, DefaultFontName);
	m_PlayerName1 = read(IniFileSection, Key_PlayerName1, DefaultPlayerName1);
	m_PlayerName2 = read(IniFileSection, Key_PlayerName2, DefaultPlayerName2);
	m_LastScript = read(IniFileSection, Key_LastScript, _T(""));
	m_WindowPos.x = read(IniFileSection, Key_WindowLeft, CW_USEDEFAULT);
	m_WindowPos.y = read(IniFileSection, Key_WindowTop, CW_USEDEFAULT);
}

void Config::Save()
{
	write(IniFileSection, Key_KanonPath, m_KanonDir.c_str());
	write(IniFileSection, Key_FontName, m_FontName.c_str());
	write(IniFileSection, Key_PlayerName1, m_PlayerName1.c_str());
	write(IniFileSection, Key_PlayerName2, m_PlayerName2.c_str());
	write(IniFileSection, Key_LastScript, m_LastScript.c_str());
	write(IniFileSection, Key_WindowLeft, m_WindowPos.x);
	write(IniFileSection, Key_WindowTop, m_WindowPos.y);
}

void Config::Validate()
{
	// Kanonのデータフォルダチェック
	if (m_KanonDir.empty()) {
		throw mxException(_T("\"Kanon\"のデータフォルダが未選択です"));
	}
	// ファイルテスト
	mxString strFile = mxPathCombine(m_KanonDir.c_str(), _T("PDT"));
	strFile = mxPathCombine(strFile.c_str(), _T("SIRO.PDT"));
	if (!::PathFileExists(strFile.c_str())) {
		throw mxException(_T("\"Kanon\"のデータフォルダが不適切です"));
	}

	// 名前のチェック
	ValidateName(m_PlayerName1.c_str(), _T("名字"));
	ValidateName(m_PlayerName2.c_str(), _T("名前"));
}

void Config::ValidateName(LPCTSTR lpszName, LPCTSTR lpszType)
{
	TCHAR szErrorMsg[256];
	int len = ::_tcslen(lpszName);

	// 文字数チェック
	if (len == 0) {
		::_stprintf(szErrorMsg, _T("%sが未入力です"), lpszType);
		throw mxException(szErrorMsg);
	}
	else if (len > 6) {
		::_stprintf(szErrorMsg, _T("%sが6文字を超えています"), lpszType);
		throw mxException(szErrorMsg);
	}

	// 半角チェック
	std::vector<WORD> vType(len + 1);
	::GetStringTypeEx(
		LOCALE_SYSTEM_DEFAULT,
		CT_CTYPE3,
		lpszName,
		len,
		&vType[0]);
//	for (std::vector<WORD>::iterator it = vType.begin(); it != vType.end(); it++) {
//		if (*it & C3_HALFWIDTH) {
	BOOST_FOREACH(WORD w, vType) {
		if (w & C3_HALFWIDTH) {
			::_stprintf(szErrorMsg, _T("%sに半角文字が含まれています"), lpszType);
			throw mxException(szErrorMsg);
		}
	}

	// 記号チェック
	if (::_tcsstr(lpszName, _T("【")) || ::_tcsstr(lpszName, _T("】")) ||
		::_tcsstr(lpszName, _T("「")) || ::_tcsstr(lpszName, _T("」")))
	{
		::_stprintf(szErrorMsg, _T("%sに【】「」が含まれています"), lpszType);
		throw mxException(szErrorMsg);
	}
}
