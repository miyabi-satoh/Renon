///////////////////////////////////////////////////////////////////////////////
/// @file	ResourceManager.h
/// @date	2014/07/30	
/// @author	Masayuki
/// @brief	リソース管理クラス
///////////////////////////////////////////////////////////////////////////////

#ifndef RESOURCEMANAGER_H_
#define RESOURCEMANAGER_H_

#include <common.hpp>

class Config;

///////////////////////////////////////////////////////////////////////////////
/// リソース管理クラス
///
class ResourceManager {
public:
	/// コンストラクタ
	ResourceManager() : m_FileMap(), m_CurrentStore() {
		m_Config = NULL;
	}
	/// デストラクタ
	~ResourceManager() {
		// 一時ファイルの削除
		FileMap::iterator it;
		for (it = m_FileMap.begin(); it != m_FileMap.end(); it++) {
			::DeleteFile(it->second.c_str());
		}
	}

	void SetConfig(const Config *p) { m_Config = p; }

	void SetCurrentStore(const mxString &s) { m_CurrentStore = s; }

	/// アクセス可能なリソースのフルパスを取得します
	/// @param リソース名
	/// @return フルパス
	/// @retval 空文字列	リソースが見つからなかった
	mxString GetResourcePath(LPCTSTR lpszName);

private:
	typedef std::map<mxString, mxString> FileMap;

	FileMap m_FileMap;			///< 一時ファイルのマップ
	mxString m_CurrentStore;	///< 現在のフォルダまたはアーカイブ
	const Config *m_Config;		///< 設定クラス
};

#endif /* RESOURCEMANAGER_H_ */
