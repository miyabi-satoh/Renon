///////////////////////////////////////////////////////////////////////////////
/// @file	AboutDialog.h
/// @date	2014/07/30	
/// @author	Masayuki
/// @brief	バージョン情報ダイアログボックスクラス
///////////////////////////////////////////////////////////////////////////////

#ifndef ABOUTDIALOG_H_
#define ABOUTDIALOG_H_

#include <dialog.hpp>
#include "resource.h"

///////////////////////////////////////////////////////////////////////////////
/// バージョン情報ダイアログボックスクラス
///
class AboutDialog: public mxDialog {
public:
	AboutDialog(mxWindow *pWndOwner)
		: mxDialog(IDD_ABOUT, pWndOwner)
	{
	}
	~AboutDialog() {
	}

protected:
	bool 	onInitDialog (HWND hWndFocus);
};

#endif /* ABOUTDIALOG_H_ */
