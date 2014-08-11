///////////////////////////////////////////////////////////////////////////////
/// @file	ConfigDialog.h
/// @date	2014/07/29	
/// @author	Masayuki
/// @brief	設定ダイアログボックスクラス
///////////////////////////////////////////////////////////////////////////////

#ifndef CONFIGDIALOG_H_
#define CONFIGDIALOG_H_

#include <dialog.hpp>
#include <font.hpp>
#include "resource.h"

class Config;

///////////////////////////////////////////////////////////////////////////////
/// 設定ダイアログボックスクラス
///
class ConfigDialog: public mxDialog {
public:
	/// コンストラクタ
	ConfigDialog(Config *pConfig, mxWindow *pWndOwner)
		: mxDialog(IDD_CONFIG, pWndOwner)
	{
		m_Config = pConfig;
	}
	/// デストラクタ
	~ConfigDialog() {
	}

protected:
	/// @name メッセージハンドラ
	//@{
	bool 	onInitDialog (HWND hWndFocus);
	bool 	onCommand (int id, HWND hwndCtl, UINT codeNotify);
	bool 	onOK ();
	bool 	onCancel ();
	//@}

private:
	Config *m_Config;	///< 設定クラス
	mxFont m_Font;		///< フォントサンプルのハンドル

	/// フォントサンプルを更新する
	void UpdateFontSample();

	/// 使用可能なフォントを列挙し、コンボボックスに追加する
	static int CALLBACK EnumFontFamExProc(
			ENUMLOGFONTEX *lpelfe,
			NEWTEXTMETRICEX *lpntme,
			DWORD FontType,
			LPARAM lParam);
};

#endif /* CONFIGDIALOG_H_ */
