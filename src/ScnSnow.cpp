///////////////////////////////////////////////////////////////////////////////
/// @file	ScnSnow.cpp
/// @date	2014/08/01	
/// @author	Masayuki
/// @brief	
///////////////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "ScnSnow.h"
#include "SnowSprite.h"
#include "ResourceManager.h"
#include "RenonDefs.h"
#include "Surface.h"

const mxRect rcSnow[] = {
	mxRect(0, 0, 10, 10),
	mxRect(0, 11, 16, 27),
};
int ScnSnow::m_State = ScnSnow::END;
UINT ScnSnow::m_dwFadeTime = 0;

void ScnSnow::Create(	ResourceManager &ResMgr)
{
	new ScnSnow(ResMgr);
}

ScnSnow::ScnSnow(ResourceManager &ResMgr)
	: Scene(), m_vSnow()
{
	// 雪の画像を読み込む
	mxDibSection dib;
	mxString strPath = ResMgr.GetResourcePath(_T("YUKI.PDT"));
	Surface::LoadFromFile(strPath.c_str(), dib);

	SnowSprite *p;
	for (int i = 0; i < NUM_SNOW; i++) {
		p = new SnowSprite(Surface::OBJECT_MIN + i);
		p->setColorKey(dib.getPixel(mxPoint(0,0)));
		m_vSnow.push_back(p);
		Surface::InsertSprite(p);

		if (i < NUM_SNOW / 2) {
			p->create(rcSnow[0].size());
			dib.bitBlt(*p, p->getRect(), rcSnow[0].topLeft());
		}
		else {
			p->create(rcSnow[1].size());
			dib.bitBlt(*p, p->getRect(), rcSnow[1].topLeft());
		}
	}
	Surface::SortSprite();
}

ScnSnow::~ScnSnow()
{
	mxTrace(_T("ScnSnow::~ScnSnow"));

	BOOST_FOREACH(SnowSprite *p, m_vSnow) {
		Surface::RemoveSprite(p);
	}
}

int ScnSnow::UpdateFrame(UINT nFrameCount)
{
	int alpha = 255;
	switch (m_State & 0xF0) {
	case ScnSnow::FADEIN:
		if (m_dwFadeTime == 0) {
			m_dwFadeTime = ::timeGetTime();
			m_State |= ScnSnow::START;
		}
		alpha = static_cast<int>(((::timeGetTime() - m_dwFadeTime) << 8) / 3000.0);
		if (alpha > 255) {
			alpha = 255;
		}
		break;
	case ScnSnow::FADEOUT:
		if (m_dwFadeTime == 0) {
			m_dwFadeTime = ::timeGetTime();
			m_State |= ScnSnow::END;
		}
		alpha = static_cast<int>(255 - ((::timeGetTime() - m_dwFadeTime) << 8) / 3000.0);
		if (alpha < 0) {
			alpha = 0;
		}
		break;
	}


	for (int i = 0; i < NUM_SNOW; i++) {
		switch (m_State & 0x0F) {
		case ScnSnow::START:
		case ScnSnow::REMOVE:
			m_vSnow[i]->Init();
			break;

		case ScnSnow::ACTIVE:
			m_vSnow[i]->Move();
			if (m_vSnow[i]->Y() > 480) {
				m_vSnow[i]->Init();
			}
			break;

		case ScnSnow::PAUSE:
			break;

		case ScnSnow::END:
			if (0 - rcSnow[1].height() < m_vSnow[i]->Y() && m_vSnow[i]->Y() < 480) {
				m_vSnow[i]->Move();
			}
			break;
		}
		m_vSnow[i]->setAlpha(alpha);
	}

	if ((m_State & 0x0F) == ScnSnow::START) {
		m_State = (m_State & 0xF0) | ScnSnow::ACTIVE;
	}

	return Scene::Next;
}
