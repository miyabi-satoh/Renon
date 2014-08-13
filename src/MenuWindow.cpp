////////////////////////////////////////////////////////////////////////////////
/// @file	MenuWindow.cpp
/// @date	2014/08/12	
/// @author	masayuki
/// @brief	class MenuWindow
////////////////////////////////////////////////////////////////////////////////

#include "MenuWindow.h"
#include "RenonApp.h"

MenuWindow::MenuWindow()
	: mxWindow()
{
}

MenuWindow::~MenuWindow()
{
}

bool MenuWindow::onCommand(int id, HWND hwndCtl, UINT codeNotify)
{
	theApp.getMainWnd()->sendMessage(WM_COMMAND, MAKELONG(id, codeNotify),
			reinterpret_cast<LPARAM>(hwndCtl));
	return true;
}
bool MenuWindow::onCreate(LRESULT *lRes, CREATESTRUCT *lpcs)
{
	return true;
}
