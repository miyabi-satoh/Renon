////////////////////////////////////////////////////////////////////////////////
/// @file	ScnSpecial.cpp
/// @date	2014/08/13	
/// @author	masayuki
/// @brief	class ScnSpecial
////////////////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "ScnSpecial.h"
#include "Surface.h"
#include "ResourceManager.h"
#include <dibtransition.hpp>

const mxRect MAK2_RECT[] = {
	mxRect(200, 150, 640, 250),	// 「ね、真琴っ、一緒に撮ろうよっ」
	mxRect(200,   0, 640, 150),	// 「真琴、真ん中、きなさい」
	mxRect(  0, 150, 200, 250),	// 「よし、真ん中入れよ」
	mxRect(200, 250, 640, 300),	// 「じゃ、撮るね」
	mxRect(  0,   0, 200, 150),	// 「おう」
	mxRect(200, 300, 640, 480),	// 「真琴もいい？」
	mxRect(  0, 300, 200, 480),	// うんっ。
};

///////////////////////////////////////////////////////////////////////////////
class Scn_MAK2 : public Scene
{
public:
	static void Create(Surface *pSurface, ResourceManager &ResMgr) {
		new Scn_MAK2(pSurface, ResMgr);
	}

	void onSize(mxWindow *pWnd, UINT state, const mxSize &sz) {
		m_Surface->Scaling(m_mak2, m_mak2);
		m_Surface->Scaling(m_siro, m_siro);

		mxRect rcSrc(MAK2_RECT[m_Index]);
		rcSrc *= m_Surface->GetScale();

		mxRect rcDst(rcSrc);
		rcDst += m_Surface->GetPaintRect().topLeft();

		if (m_Erase) {
			m_mak2.bitBlt(*m_Surface, rcDst, rcSrc.topLeft());
			for (int n = 1; n <= m_Step; n++) {
				mxTrans::Dot4x4<mxPixelOp::Copy> Trans(n);
				Trans.doTransition(
						m_Surface, rcDst,
						&m_siro, rcSrc.topLeft());
			}
		}
		else {
			m_siro.bitBlt(*m_Surface, rcDst, rcSrc.topLeft());
			for (int n = 1; n <= m_Step; n++) {
				mxTrans::Dot4x4<mxPixelOp::Copy> Trans(n);
				Trans.doTransition(
						m_Surface, rcDst,
						&m_mak2, rcSrc.topLeft());
			}
		}
	}

protected:
	int UpdateFrame(UINT nFrameCount) {
		m_Step = nFrameCount;
		if (m_Erase) {
			m_Step -= 60;
		}
		if (m_Step < 1) {
			return Scene::Next;
		}

		mxTrans::Dot4x4<mxPixelOp::Copy> Trans(m_Step);
		int nRet;

		mxRect rcSrc(MAK2_RECT[m_Index]);
		rcSrc *= m_Surface->GetScale();

		mxRect rcDst(rcSrc);
		rcDst += m_Surface->GetPaintRect().topLeft();
		if (m_Erase) {
			mxTraceR(m_Step);
			nRet = Trans.doTransition(
				m_Surface, rcDst,
				&m_siro, rcSrc.topLeft());
			if (nRet == TRANSITION_END) {
				ResetCount();
				m_Erase = !m_Erase;
				if (++m_Index > 6) {
					return Scene::END;
				}
			}
		}
		else {
			nRet = Trans.doTransition(
				m_Surface, rcDst,
				&m_mak2, rcSrc.topLeft());
			if (nRet == TRANSITION_END) {
				ResetCount();
				m_Erase = !m_Erase;
			}
		}

		return Scene::Next;
	}


private:
	Surface *m_Surface;
	mxDibSection m_mak2;
	mxDibSection m_siro;
	UINT m_Index;
	int m_Step;
	bool m_Erase;

	Scn_MAK2(Surface *pSurface, ResourceManager &ResMgr)
		: Scene(), m_mak2(), m_siro()
	{
		m_Surface = pSurface;
		m_Index = 0;
		m_Step = 0;
		m_Erase = false;

		mxString strName;

		strName = ResMgr.GetResourcePath(_T("S_MAK2.PDT"));
		Surface::LoadFromFile(strName.c_str(), m_mak2);
		m_Surface->Scaling(m_mak2, m_mak2);

		strName = ResMgr.GetResourcePath(_T("SIRO.PDT"));
		Surface::LoadFromFile(strName.c_str(), m_siro);
		m_Surface->Scaling(m_siro, m_siro);
	}

};

void ScnSpecial::Create(std::wstring type, Surface *pSurface, ResourceManager &ResMgr)
{
	if (type == L"S_MAK2") {
		Scn_MAK2::Create(pSurface, ResMgr);
	}
}
