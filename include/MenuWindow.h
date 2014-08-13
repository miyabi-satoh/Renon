////////////////////////////////////////////////////////////////////////////////
/// @file	MenuWindow.h
/// @date	2014/08/12	
/// @author	masayuki
/// @brief	class MenuWindow
////////////////////////////////////////////////////////////////////////////////

#ifndef MENUWINDOW_H_
#define MENUWINDOW_H_

#include <window.hpp>

class MenuWindow: public mxWindow {
public:
	MenuWindow();
	virtual ~MenuWindow();

protected:
	virtual bool 	onCommand (int id, HWND hwndCtl, UINT codeNotify);
	virtual bool 	onCreate (LRESULT *lRes, CREATESTRUCT *lpcs);
};

#endif /* MENUWINDOW_H_ */
