///////////////////////////////////////////////////////////////////////////////
/// @file	Config.h
/// @date	2014/07/29	
/// @author	Masayuki
/// @brief	設定クラス
///////////////////////////////////////////////////////////////////////////////

#ifndef CONFIG_H_
#define CONFIG_H_

#include "RenonDefs.h"
#include "inifile.hpp"

///////////////////////////////////////////////////////////////////////////////
/// 設定クラス
///
class Config : private mxIniFile {
public:
	/// コンストラクタ
	Config()
		: mxIniFile(IniFileName), m_KanonDir(), m_FontName(),
		  m_PlayerName1(), m_PlayerName2(), m_LastScript(), m_WindowPos()
	{
	}
	/// デストラクタ
	~Config() {
	}

	/// 設定の読み込み
	void Load();

	/// 設定の保存
	void Save();

	/// 設定内容の検証
	/// @throw mxException
	void Validate();

	/// @name 値の取得
	//@{
	LPCTSTR GetKanonDir() const			{ return m_KanonDir.c_str(); }
	LPCTSTR GetFontName() const			{ return m_FontName.c_str(); }
	LPCTSTR GetPlayerName1() const		{ return m_PlayerName1.c_str(); }
	LPCTSTR GetPlayerName2() const		{ return m_PlayerName2.c_str(); }
	LPCTSTR GetLastScript() const			{ return m_LastScript.c_str(); }
	const mxPoint& GetWindowPos() const	{ return m_WindowPos; }
	//@}

	/// @name 値の設定
	//@{
	void SetKanonDir(LPCTSTR lp)			{ m_KanonDir = lp; }
	void SetFontName(LPCTSTR lp)			{ m_FontName = lp; }
	void SetPlayerName1(LPCTSTR lp)		{ m_PlayerName1 = lp; }
	void SetPlayerName2(LPCTSTR lp)		{ m_PlayerName2 = lp; }
	void SetLastScript(LPCTSTR lp)		{ m_LastScript = lp; }
	void SetWindowPos(const mxPoint& pt)	{ m_WindowPos = pt; }
	//@}

private:
	mxString m_KanonDir;	///< Kanonのフォルダ
	mxString m_FontName;	///< フォント名
	mxString m_PlayerName1;	///< プレイヤー名字
	mxString m_PlayerName2;	///< プレイヤー名前
	mxString m_LastScript;	///< 最後に開いたスクリプト
	mxPoint m_WindowPos;	///< ウィンドウ位置

	/// 名前の検証
	/// @param lpszName 検証する値
	/// @param lpszType "名字" or "名前"
	/// @throw mxException
	void ValidateName(LPCTSTR lpszName, LPCTSTR lpszType);
};

#endif /* CONFIG_H_ */
