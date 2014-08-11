///////////////////////////////////////////////////////////////////////////////
/// @file	AboutDialog.cpp
/// @date	2014/07/30	
/// @author	Masayuki
/// @brief	
///////////////////////////////////////////////////////////////////////////////

#include "AboutDialog.h"
#include "RenonDefs.h"

bool AboutDialog::onInitDialog (HWND hWndFocus)
{
	TCHAR szVersion[256];
	::_stprintf(szVersion, _T("%s Ver %s"), AppName, AppVersion);
	setItemText(IDC_STATIC_VERSION, szVersion);

	return true;
}
