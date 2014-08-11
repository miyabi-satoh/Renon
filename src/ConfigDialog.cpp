///////////////////////////////////////////////////////////////////////////////
/// @file	ConfigDialog.cpp
/// @date	2014/07/29	
/// @author	Masayuki
/// @brief	設定ダイアログボックスクラス
///////////////////////////////////////////////////////////////////////////////

#include "ConfigDialog.h"
#include "RenonDefs.h"
#include "Config.h"
#include <dc.hpp>
#include <folderdialog.hpp>

bool ConfigDialog::onInitDialog(HWND hWndFocus)
{
	// フォント選択コンボボックスを初期化する
	mxZero<LOGFONT> lf;
	lf->lfCharSet = SHIFTJIS_CHARSET;
	mxWindowDC dc(this);
	dc.enumFontFamilies(lf.get(), EnumFontFamExProc, reinterpret_cast<LPARAM>(this));
	HWND hCombo = getItem(IDC_COMBO_FONT);
	int n = ComboBox_FindStringExact(hCombo, -1, m_Config->GetFontName());
	if (n == CB_ERR) {
		n = ComboBox_FindStringExact(hCombo, -1, DefaultFontName);
	}
	ComboBox_SetCurSel(hCombo, n);
	UpdateFontSample();

	// Kanonのパス
	setItemText(IDC_KANON_PATH, m_Config->GetKanonDir());

	// プレイヤー名
	Edit_LimitText(getItem(IDC_PLAYER_NAME1), 6);
	setItemText(IDC_PLAYER_NAME1, m_Config->GetPlayerName1());

	Edit_LimitText(getItem(IDC_PLAYER_NAME2), 6);
	setItemText(IDC_PLAYER_NAME2, m_Config->GetPlayerName2());


	return mxDialog::onInitDialog(hWndFocus);
}
bool ConfigDialog::onCommand(int id, HWND hwndCtl, UINT codeNotify)
{
	switch (id) {
	case IDC_COMBO_FONT:
		if (codeNotify == CBN_SELCHANGE) {
			UpdateFontSample();
			return true;
		}
		break;
	case IDC_BROWSE_KANON_PATH: {
		mxFolderDialog dlg(this, BIF_RETURNONLYFSDIRS);
		if (dlg.doModal() == IDOK) {
			setItemText(IDC_KANON_PATH, dlg.getSelectedPath());
		}

		return true; }

	case IDC_BUTTON_NAME1:
		setItemText(IDC_PLAYER_NAME1, DefaultPlayerName1);
		return true;

	case IDC_BUTTON_NAME2:
		setItemText(IDC_PLAYER_NAME2, DefaultPlayerName2);
		return true;
	}
	return mxDialog::onCommand(id, hwndCtl, codeNotify);
}

bool ConfigDialog::onOK()
{
	m_Config->SetFontName(getItemText(IDC_COMBO_FONT).c_str());
	m_Config->SetKanonDir(getItemText(IDC_KANON_PATH).c_str());
	m_Config->SetPlayerName1(getItemText(IDC_PLAYER_NAME1).c_str());
	m_Config->SetPlayerName2(getItemText(IDC_PLAYER_NAME2).c_str());

	try {
		m_Config->Validate();
	}
	catch (const mxException &e) {
		messageBox(e.what(), NULL, MB_OK | MB_ICONSTOP);
		return true;
	}

	m_Config->Save();

	return mxDialog::onOK();
}

bool ConfigDialog::onCancel()
{
	m_Config->Load();	// Validateしてる可能性があるため、読みなおす
	return mxDialog::onCancel();
}

void ConfigDialog::UpdateFontSample()
{
	HWND hStatic = getItem(IDC_STATIC_FONTSAMPLE);
	SetWindowFont(hStatic, ::GetStockObject(SYSTEM_FONT), FALSE);

	m_Font.create(getItemText(IDC_COMBO_FONT).c_str(), 26);
	SetWindowFont(hStatic, m_Font, TRUE);
}

int CALLBACK ConfigDialog::EnumFontFamExProc(
		ENUMLOGFONTEX *lpelfe,
		NEWTEXTMETRICEX *lpntme,
		DWORD FontType,
		LPARAM lParam)
{
	LPCTSTR lpszFacename = lpelfe->elfLogFont.lfFaceName;
	if (lpszFacename[0] == _T('@')) {
		return 1;
	}
	if (!(lpelfe->elfLogFont.lfPitchAndFamily & FIXED_PITCH)) {
		return 1;
	}
	if (lpelfe->elfLogFont.lfHeight < 26) {
		return 1;
	}

	ConfigDialog *pDlg = reinterpret_cast<ConfigDialog*>(lParam);
	ComboBox_AddString(pDlg->getItem(IDC_COMBO_FONT), lpszFacename);

	return 1;
}
