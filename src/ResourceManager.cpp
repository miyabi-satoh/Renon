///////////////////////////////////////////////////////////////////////////////
/// @file	ResourceManager.cpp
/// @date	2014/07/30	
/// @author	Masayuki
/// @brief	リソース管理クラス
///////////////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "Config.h"
#include "ResourceManager.h"
#include <archiver.hpp>

mxString ResourceManager::GetResourcePath(LPCTSTR lpszName)
{
	if (lpszName == NULL || *lpszName == _T('\0')) {
		return mxString();
	}

	std::vector<mxString> vString;
	boost::split(vString, lpszName, boost::is_any_of(_T("/")));

	if (vString.size() == 1) {	// 普通のファイル
		if (::PathFileExists(lpszName)) {	// フルパス指定
			// フォルダパスを取得して保存
			SetCurrentStore(mxPathRemoveFileSpec(lpszName));

			return mxString(lpszName);
		}
		// 現在のフォルダ内にあるか？
		mxString strPath;
		strPath = mxPathCombine(m_CurrentStore.c_str(), lpszName);
		if (::PathFileExists(strPath.c_str())){
			return strPath;
		}
		// KanonのPDTフォルダ内にあるか？
		strPath = mxPathCombine(m_Config->GetKanonDir(), _T("PDT"));
		strPath = mxPathCombine(strPath.c_str(), lpszName);
		if (::PathFileExists(strPath.c_str())){
			return strPath;
		}
		// KanonのDATフォルダ内にあるか？
		strPath = mxPathCombine(m_Config->GetKanonDir(), _T("DAT"));
		strPath = mxPathCombine(strPath.c_str(), lpszName);
		if (::PathFileExists(strPath.c_str())){
			return strPath;
		}
		// アーカイブの/忘れ？
		vString.resize(2);
		vString[0] = m_CurrentStore;
		vString[1] = mxString(lpszName);
	}
	// アーカイブから展開する

	if (vString[0].empty()) {
		vString[0] = m_CurrentStore;
		if (vString[0].empty()) {
			return mxString();
		}
	}
	else {
		SetCurrentStore(vString[0]);
	}
	mxString strKey = vString[0] + _T('/') + vString[1];

	// 展開済みなら再利用する
	if (m_FileMap.find(strKey) != m_FileMap.end()) {
		return m_FileMap[strKey];
	}

	// 一時ファイルに展開する
	mxArchiver archiver;
	archiver.open(
			vString[0].c_str(),
			mxFile::modeRead | mxFile::shareRead | mxFile::openExisting);
	UINT numFiles = archiver.getFileCount();
	mxZero<mxArchiver::FileInfo> fi;
	for (UINT n = 0; n < numFiles; n++) {
		archiver.getFileInfo(n, fi.get());
		if (vString[1] == fi->szName) {
			mxString strTempFile = mxGetTempPath();
			try {
				archiver.extractToFile(fi.get(), strTempFile.c_str());
				m_FileMap[strKey] = strTempFile;
				return strTempFile;
			}
			catch (const mxException &e) {
				::DeleteFile(strTempFile.c_str());
			}
			break;
		}
	}

	return mxString();
}

