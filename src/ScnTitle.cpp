///////////////////////////////////////////////////////////////////////////////
/// @file	ScnTitle.cpp
/// @date	2014/08/01	
/// @author	Masayuki
/// @brief	
///////////////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "ScnTitle.h"
#include "Surface.h"
#include "ResourceManager.h"
#include "RenonDefs.h"
#include "AboutDialog.h"
#include "ConfigDialog.h"
#include "Config.h"
#include "resource.h"
#include <window.hpp>
#include <filedialog.hpp>

const mxRect rcMenu[] = {
	mxRect(mxPoint(205, 210), mxSize(220, 47)),
	mxRect(mxPoint(205, 261), mxSize(220, 47)),
	mxRect(mxPoint(205, 312), mxSize(220, 47)),
	mxRect(mxPoint(205, 363), mxSize(220, 47)),
	mxRect(mxPoint(205, 414), mxSize(220, 47)),
};
enum {
	ID_MENU_START = 0,
	ID_MENU_RESTART,
	ID_MENU_CONFIG,
	ID_MENU_VERSION,
	ID_MENU_QUIT,
	NUM_MENU,
};

void ScnTitle::Create(Surface *pSurface, Config *pConfig, ResourceManager &ResMgr)
{
	new ScnTitle(pSurface, pConfig, ResMgr);
}

ScnTitle::ScnTitle(Surface *pSurface, Config *pConfig, ResourceManager &ResMgr)
	: Scene(), m_vMenuOn()
{
	m_Surface = pSurface;
	m_Config = pConfig;
	m_Ret = Scene::Next;

	// メニューON画像を読み込む
	mxDibSection dib;
	mxString strPath = ResMgr.GetResourcePath(_T("TI_ON.PDT"));
	Surface::LoadFromFile(strPath.c_str(), dib);

	for (int i = 0; i < NUM_MENU; i++) {
		mxSprite *p = new mxSprite();
		p->create(rcMenu[i].size());
		p->setPos(rcMenu[i].topLeft());
		p->setColorKey(dib.getPixel(mxPoint(0, 0)));
		p->setAlpha(0);
		dib.bitBlt(*p, p->getRect(), rcMenu[i].topLeft());

		m_vMenuOn.push_back(p);
		m_Surface->InsertSprite(p);
	}
}
ScnTitle::~ScnTitle()
{
	BOOST_FOREACH(mxSprite *p, m_vMenuOn) {
		m_Surface->RemoveSprite(p);
	}
	m_Surface->GetCharSprite().destroy();
}

int ScnTitle::	UpdateFrame(UINT nFrameCount)
{
	// スプライトの統合イメージを取得する
	mxDibSection newImage;;
	m_Surface->CompositeAndScaling(newImage);

	// サーフェスに描画する
	newImage.bitBlt(*m_Surface, m_Surface->GetPaintRect());

	return m_Ret;
}

void ScnTitle::onLButtonDown(
		mxWindow *pWnd,
		BOOL fDoubleClick,
		const mxPoint &pt,
		UINT keyFlags)
{
	mxPoint ptMouse(pt);
	ptMouse -= m_Surface->GetPaintRect().topLeft();
	ptMouse /= m_Surface->GetScale();

	int i;
	for (i = 0; i < NUM_MENU; i++) {
		if (::PtInRect(&rcMenu[i], ptMouse)) {
			break;
		}
	}

	switch (i) {
	case ID_MENU_START: {
		mxFileDialog dlg(
				true,
				OFN_FILEMUSTEXIST,
				FILTER_SCN,
				pWnd);
		if (dlg.doModal() == IDOK) {
			pWnd->sendMessage(
					WM_LOAD_SCRIPT,
					FALSE,
					reinterpret_cast<LPARAM>(dlg.getPathName().c_str()));
			m_Ret = Scene::END;
		}
		break; }

	case ID_MENU_RESTART: {
		mxString strName = m_Config->GetLastScript();
		if (!strName.empty()) {
			pWnd->sendMessage(
					WM_LOAD_SCRIPT,
					FALSE,
					reinterpret_cast<LPARAM>(strName.c_str()));
			m_Ret = Scene::END;
		}
		break; }

	case ID_MENU_CONFIG: {
		ConfigDialog dlg(m_Config, pWnd);
		dlg.doModal();
		break; }

	case ID_MENU_VERSION: {
		AboutDialog dlg(pWnd);
		dlg.doModal();
		break; }

	case ID_MENU_QUIT:
		pWnd->sendMessage(
				WM_LOAD_SCRIPT,
				TRUE,
				reinterpret_cast<LPARAM>(EXIT_SCN));
		m_Ret = Scene::END;
		break;

	default:
		return;
	}
}

void ScnTitle::onMouseMove(mxWindow *pWnd, const mxPoint &pt, UINT keyFlags)
{
	mxPoint ptMouse(pt);
	ptMouse -= m_Surface->GetPaintRect().topLeft();
	ptMouse /= m_Surface->GetScale();

	BOOST_FOREACH(mxSprite *p, m_vMenuOn) {
		mxRect rc(p->getPos(), p->getSize());
		if (::PtInRect(&rc, ptMouse)) {
			p->setAlpha(0xFF);
		}
		else {
			p->setAlpha(0);
		}
	}

	mxString strName = m_Config->GetLastScript();
	if (strName.empty()) {
		m_vMenuOn[ID_MENU_RESTART]->setAlpha(0);
	}
}
