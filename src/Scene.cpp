///////////////////////////////////////////////////////////////////////////////
/// @file	Scene.cpp
/// @date	2014/07/29	
/// @author	Masayuki
/// @brief	class ScnWait
///////////////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "Scene.h"
#include "Surface.h"

Scene::Container Scene::List;

int Scene::onUpdate()
{
	int nSync = 0;
	for (Container::iterator it = List.begin(); it != List.end(); ) {
		if (!(*it)->IsAsync()) {
			nSync++;
		}
		if ((*it)->UpdateFrame() == Scene::END) {
			it = List.erase(it);
			continue;
		}
		it++;
	}
	return nSync;
}

void Scene::OnLButtonDown(
		mxWindow *pWnd,
		BOOL fDoubleClick,
		const mxPoint &pt,
		UINT keyFlags)
{
	BOOST_FOREACH(Ptr &p, List) {
		p->onLButtonDown(pWnd, fDoubleClick, pt, keyFlags);
	}
}

void Scene::OnMouseMove(mxWindow *pWnd, const mxPoint &pt, UINT keyFlags)
{
	BOOST_FOREACH(Ptr &p, List) {
		p->onMouseMove(pWnd, pt, keyFlags);
	}

}

void Scene::OnSize(mxWindow *pWnd, UINT state, const mxSize &sz)
{
	BOOST_FOREACH(Ptr &p, List) {
		p->onSize(pWnd, state, sz);
	}
}

void ScnWait::Create(Surface *pSurface, int option)
{
	new ScnWait(pSurface, option);
}

int ScnWait::UpdateFrame(UINT nFrameCount)
{
	if (nFrameCount == 1) {
		m_StartTime = ::timeGetTime();
	}

	// スプライトの統合イメージを取得する
	mxDibSection newImage;;
	m_Surface->CompositeAndScaling(newImage);

	// サーフェスに描画する
	newImage.bitBlt(*m_Surface, m_Surface->GetPaintRect());

	if (::timeGetTime() - m_StartTime > m_WaitTime) {
		return Scene::END;
	}

	return Scene::Next;
}
