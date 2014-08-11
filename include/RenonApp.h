///////////////////////////////////////////////////////////////////////////////
/// @file	RenonApp.h
/// @date	2014/07/27	
/// @author	Masayuki
/// @brief	Renonアプリケーションクラス
///////////////////////////////////////////////////////////////////////////////

#ifndef RENONAPP_H_
#define RENONAPP_H_

#include <app.hpp>
#include "Config.h"
#include "ResourceManager.h"

DECLARE_APP(RenonApp)

///////////////////////////////////////////////////////////////////////////////
/// Renonアプリケーションクラス
///
class RenonApp: public mxApp {
public:
	/// コンストラクタ
	RenonApp()
		: m_Config(), m_ResourceManager()
	{
		m_ResourceManager.SetConfig(&m_Config);
	}
	/// デストラクタ
	~RenonApp() {
	}

protected:
	void 	initialize ();
	void 	finalize ();
	void 	doIdle ();
	bool 	preTranslateMessage (MSG *lpMsg);

private:
	Config m_Config;					///< 設定管理クラス
	ResourceManager m_ResourceManager;	///< リソース管理クラス
};

#endif /* RENONAPP_H_ */
