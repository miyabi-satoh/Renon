///////////////////////////////////////////////////////////////////////////////
/// @file	Surface.cpp
/// @date	2014/07/30	
/// @author	Masayuki
/// @brief	class Surface
///////////////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "Surface.h"
#include <file.hpp>
#include <globals.hpp>
#include <findfile.hpp>
#include <spi.hpp>

Surface::Container Surface::List;

struct SpriteSortFunctor
{
	bool operator()(const Surface::SpritePtr &lhs, const Surface::SpritePtr &rhs) const {
		return lhs->z() < rhs->z();
	}
};

Surface::Surface()
	: mxDibSection(), m_PaintRect(0, 0, 640, 480), m_Backup()
{
	m_Scale = 1.0;

	m_BackSprite = new mxSprite();
	m_BackSprite->setZ(BACK);
	InsertSprite(m_BackSprite);

	m_CharSprite = new mxSprite();
	m_CharSprite->setZ(CHARA);
	InsertSprite(m_CharSprite);

	m_TextSprite = new TextSprite();
	m_TextSprite->create(mxSize(592, 112));
	m_TextSprite->setPos(mxPoint(22, 350));
	m_TextSprite->setZ(Surface::FRAME);
	m_TextSprite->setAlpha(0);
	InsertSprite(m_TextSprite);
}

Surface::~Surface()
{
}

void Surface::CompositeAndScaling(mxDibSection &dib) {
	mxDibSection compositeDib(mxSize(640, 480));
	BOOST_FOREACH(SpritePtr &p, List) {
		p->render(compositeDib);
	}

	cv::Mat mSrc(compositeDib.height(), compositeDib.width(), CV_8UC4);
	::CopyMemory(mSrc.data, compositeDib.bits(), compositeDib.getSizeImage());

	cv::Mat mDst(m_PaintRect.height(), m_PaintRect.width(), CV_8UC4);
	cv::resize(mSrc, mDst, mDst.size());

	dib.create(m_PaintRect.size());
	::CopyMemory(dib.bits(), mDst.data, dib.getSizeImage());
}

void Surface::EndTransition()
{
	mxDibSection compositeDib(mxSize(640, 480));
	m_BackSprite->render(compositeDib);
	m_CharSprite->render(compositeDib);

	cv::Mat mSrc(compositeDib.height(), compositeDib.width(), CV_8UC4);
	::CopyMemory(mSrc.data, compositeDib.bits(), compositeDib.getSizeImage());

	cv::Mat mDst(m_Backup.height(), m_Backup.width(), CV_8UC4);
	cv::resize(mSrc, mDst, mDst.size());
	::CopyMemory(m_Backup.bits(), mDst.data, m_Backup.getSizeImage());
}

void Surface::InsertSprite(mxSprite *p) {
	List.push_back(SpritePtr(p));
}

void Surface::RemoveSprite(mxSprite *p) {
	Container::iterator it;
	for (it = List.begin(); it != List.end(); it++) {
		if (it->get() == p) {
			mxTrace(_T("Remove Sprite ") << p);
			List.erase(it);
			break;
		}
	}
}

void Surface::SortSprite()
{
	List.sort(SpriteSortFunctor());
}

void Surface::Resize(const mxSize &sz) {
	// 描画領域を計算する
	double scaleX = sz.cx / 640.0;
	double scaleY = sz.cy / 480.0;
	m_Scale = (scaleX < scaleY) ? scaleX : scaleY;
	int cx = static_cast<int>(640 * m_Scale);
	int cy = static_cast<int>(480 * m_Scale);
	int x = (sz.cx - cx) / 2;
	int y = (sz.cy - cy) / 2;
	m_PaintRect = mxRect(mxPoint(x, y), mxSize(cx, cy));

	// 自分自身をリサイズ
	this->create(sz);

	// バックアップをリサイズ
	if (m_Backup.bits() == NULL) {
		m_Backup.create(sz);
	}
	else {
		cv::Mat mSrc(m_Backup.height(), m_Backup.width(), CV_8UC4);
		::CopyMemory(mSrc.data, m_Backup.bits(), m_Backup.getSizeImage());

		cv::Mat mDst(m_PaintRect.height(), m_PaintRect.width(), CV_8UC4);
		cv::resize(mSrc, mDst, mDst.size());

		m_Backup.create(m_PaintRect.size());
		::CopyMemory(m_Backup.bits(), mDst.data, m_Backup.getSizeImage());
	}
	// テキストをリサイズ
	m_TextSprite->Resize(m_Scale, m_PaintRect);

	// TODO:スプライトを再描画?
	// TODO:テキストを再描画?

}

void Surface::LoadFromFile(LPCTSTR lpszName, mxDibSection &dib)
{
	//
	// 標準の機能で読み込む
	//
	if (dib.create(lpszName)) {
//		mxTrace(_T("Load image:") << lpszName);
		return;
	}

	//
	// Susie Plug-in の機能を使って読み込む
	//

	// ファイルの先頭2KBを読み込んでおく
	std::vector<char> vBuffer(2000, 0);
	mxFile file(lpszName,
			mxFile::modeRead | mxFile::shareRead | mxFile::openExisting);
	file.read(&vBuffer[0], vBuffer.size());

	// Susie Plug-inを検索する
	mxString strSpi;
	strSpi = mxPathCombine(mxGetSelfFolderPath().c_str(), _T("*.spi"));

	mxFindFile ff(strSpi.c_str());
	while (ff.next()) {
		mxSPI spi;
		try {
			spi.load(ff.getFilePath().c_str());
			char lpszPath[MAX_PATH + 1];
#ifdef UNICODE
			::WideCharToMultiByte(
				CP_ACP,
				0,
				lpszName,
				-1,
				lpszPath,
				MAX_PATH,
				NULL,
				NULL);
#else
			::strcpy(lpszPath, lpszName);
#endif
			if (spi.isSupported(lpszPath,
					reinterpret_cast<DWORD>(&vBuffer[0])))
			{
				HANDLE hBInfo;
				HANDLE hBm;
				if (spi.getPicture(lpszPath, 0, 0, &hBInfo, &hBm) == 0) {
					BITMAPINFO *pBMI = reinterpret_cast<BITMAPINFO*>(::LocalLock(hBInfo));
					void *pBits = ::LocalLock(hBm);
					dib.create(pBMI, pBits);
//					mxTrace(_T("Load image:") << lpszName);

					::LocalUnlock(hBInfo);
					::LocalUnlock(hBm);
					::LocalFree(hBInfo);
					::LocalFree(hBm);
					return;
				}
			}
		}
		catch (const mxException &e) {
			mxTrace(e.what());
		}
	}
}
