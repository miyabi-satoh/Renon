///////////////////////////////////////////////////////////////////////////////
/// @file	ScnTransition.cpp
/// @date	2014/07/30	
/// @author	Masayuki
/// @brief	画面切り替えのシーン
///////////////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "ScnTransition.h"
#include "RenonDefs.h"
#include "Surface.h"
#include <dibtransition.hpp>

///////////////////////////////////////////////////////////////////////////////
class DotTile : public Scene
{
public:
	static void Create(Surface *pSurface, int option) {
		new DotTile(pSurface, option);
	}

	void onSize(mxWindow *pWnd, UINT state, const mxSize &sz) {
		mxDibSection newImage;
		m_Surface->CompositeAndScaling(newImage);

		for (UINT n = 1; n <= m_Step; n++) {
			mxTrans::Dot4x4<mxPixelOp::Copy> Trans(n);
			Trans.doTransition(
					m_Surface,
					m_Surface->GetPaintRect(),
					&newImage,
					mxPoint(0, 0));
		}
	}

protected:
	int UpdateFrame(UINT nFrameCount) {
		if (nFrameCount == 1) {	// 初回のみ
			m_StartTime = ::timeGetTime();
			m_Surface->Restore();
			m_Surface->GetTextSprite().setAlpha(0);
		}

		mxDibSection newImage;
		m_Surface->CompositeAndScaling(newImage);

		int nRet = 0;
		m_Step = 1 + 15 * (::timeGetTime() - m_StartTime) / m_ProcessingTime;
		if (m_Step > nFrameCount) {
			m_Step = nFrameCount;
		}

		mxTrans::Dot4x4<mxPixelOp::Copy> Trans(m_Step);
		nRet = Trans.doTransition(
				m_Surface,
				m_Surface->GetPaintRect(),
				&newImage,
				mxPoint(0, 0));

		if (nRet == TRANSITION_END) {
			m_Surface->EndTransition();
			return Scene::END;
		}

		return Scene::Next;
	}

private:
	Surface *m_Surface;		///< 描画サーフェス
	UINT m_ProcessingTime;	///< 処理時間
	DWORD m_StartTime;		///< 処理開始時間
	UINT m_Step;			///< ステップ

	DotTile(Surface *pSurface, int option) {
		m_Surface = pSurface;
		m_ProcessingTime = option;
		m_StartTime = 0;
		m_Step = 1;
	}
};

///////////////////////////////////////////////////////////////////////////////
class Fadein : public Scene
{
public:
	static void Create(Surface *pSurface, int option) {
		new Fadein(pSurface, option);
	}

protected:
	int UpdateFrame(UINT nFrameCount) {
		if (nFrameCount == 1) {	// 初回のみ
			m_StartTime = ::timeGetTime();
			m_Surface->GetTextSprite().setAlpha(0);
		}

		mxDibSection newImage;
		m_Surface->CompositeAndScaling(newImage);

		int nRet = Scene::Next;
		int nStep = static_cast<int>((255.0 * (::timeGetTime() - m_StartTime)) / m_ProcessingTime);
		if (nStep > 255) {
			nStep = 255;
			nRet = Scene::END;
		}
		mxTrans::Blt<mxPixelOp::AlphaBlend> Trans;
		Trans.getOperator().alpha = nStep;
		Trans.doTransition(
				m_Surface,
				m_Surface->GetPaintRect(),
				&(m_Surface->GetBackup()),
				mxPoint(0, 0),
				&newImage,
				mxPoint(0, 0));

		if (nRet == Scene::END) {
			m_Surface->EndTransition();
		}

		return nRet;
	}

private:
	Surface *m_Surface;		///< 描画サーフェス
	int m_ProcessingTime;	///< 処理時間
	DWORD m_StartTime;		///< 処理開始時間

	Fadein(Surface *pSurface, int option) {
		m_Surface = pSurface;
		m_ProcessingTime = option;
		m_StartTime = 0;
	}
};

///////////////////////////////////////////////////////////////////////////////
class AccordionT2B : public Scene
{
public:
	static void Create(Surface *pSurface, int option) {
		new AccordionT2B(pSurface, option);
	}
	void onSize(mxWindow *pWnd, UINT state, const mxSize &sz) {
		mxDibSection newImage;
		m_Surface->CompositeAndScaling(newImage);

		int bandW = static_cast<int>(14 * m_Surface->GetScale());
		for (UINT n = 1; n <= GetCount(); n++) {
			mxTrans::AccordionT2B<mxPixelOp::Copy> Trans(n, bandW);
			 Trans.doTransition(
					m_Surface,
					m_Surface->GetPaintRect(),
					&newImage,
					mxPoint(0, 0));
		}

	}

protected:
	int UpdateFrame(UINT nFrameCount) {
		if (nFrameCount == 1) {	// 初回のみ
			m_StartTime = ::timeGetTime();
			m_Surface->Restore();
			m_Surface->GetTextSprite().setAlpha(0);
		}

		mxDibSection newImage;
		m_Surface->CompositeAndScaling(newImage);

		int nRet = 0;
		int bandW = static_cast<int>(14 * m_Surface->GetScale());
		mxTrans::AccordionT2B<mxPixelOp::Copy> Trans(nFrameCount, bandW);
		nRet = Trans.doTransition(
				m_Surface,
				m_Surface->GetPaintRect(),
				&newImage,
				mxPoint(0, 0));

		if (nRet == TRANSITION_END) {
			m_Surface->EndTransition();
			return Scene::END;
		}

		return Scene::Next;
	}

private:
	Surface *m_Surface;		///< 描画サーフェス
	UINT m_ProcessingTime;	///< 処理時間
	DWORD m_StartTime;		///< 処理開始時間

	AccordionT2B(Surface *pSurface, int option) {
		m_Surface = pSurface;
		m_ProcessingTime = option;
		m_StartTime = 0;
	}
};

///////////////////////////////////////////////////////////////////////////////
class AccordionL2R : public Scene
{
public:
	static void Create(Surface *pSurface, int option) {
		new AccordionL2R(pSurface, option);
	}
	void onSize(mxWindow *pWnd, UINT state, const mxSize &sz) {
		mxDibSection newImage;
		m_Surface->CompositeAndScaling(newImage);

		int bandW = static_cast<int>(14 * m_Surface->GetScale());
		for (UINT n = 1; n <= GetCount(); n++) {
			mxTrans::AccordionL2R<mxPixelOp::Copy> Trans(n, bandW);
			 Trans.doTransition(
					m_Surface,
					m_Surface->GetPaintRect(),
					&newImage,
					mxPoint(0, 0));
		}

	}

protected:
	int UpdateFrame(UINT nFrameCount) {
		if (nFrameCount == 1) {	// 初回のみ
			m_StartTime = ::timeGetTime();
			m_Surface->Restore();
			m_Surface->GetTextSprite().setAlpha(0);
		}

		mxDibSection newImage;
		m_Surface->CompositeAndScaling(newImage);

		int nRet = 0;
		int bandW = static_cast<int>(14 * m_Surface->GetScale());
		mxTrans::AccordionL2R<mxPixelOp::Copy> Trans(nFrameCount, bandW);
		nRet = Trans.doTransition(
				m_Surface,
				m_Surface->GetPaintRect(),
				&newImage,
				mxPoint(0, 0));

		if (nRet == TRANSITION_END) {
			m_Surface->EndTransition();
			return Scene::END;
		}

		return Scene::Next;
	}

private:
	Surface *m_Surface;		///< 描画サーフェス
	UINT m_ProcessingTime;	///< 処理時間
	DWORD m_StartTime;		///< 処理開始時間

	AccordionL2R(Surface *pSurface, int option) {
		m_Surface = pSurface;
		m_ProcessingTime = option;
		m_StartTime = 0;
	}
};

///////////////////////////////////////////////////////////////////////////////
void ScnTransition::Create(
		Surface *pSurface,
		const std::wstring type,
		int option)
{
	if (type == TRANS_DOTTILE) {
		DotTile::Create(pSurface, option);
	}
	else if (type == TRANS_FADEIN) {
		Fadein::Create(pSurface, option);
	}
	else if (type == TRANS_ACCT2B) {
		AccordionT2B::Create(pSurface, option);
	}
	else if (type == TRANS_ACCL2R) {
		AccordionL2R::Create(pSurface, option);
	}
}
