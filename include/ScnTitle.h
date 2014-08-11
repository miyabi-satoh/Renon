///////////////////////////////////////////////////////////////////////////////
/// @file	ScnTitle.h
/// @date	2014/08/01	
/// @author	Masayuki
/// @brief	class ScnTitle
///////////////////////////////////////////////////////////////////////////////

#ifndef SCNTITLE_H_
#define SCNTITLE_H_

#include <Scene.h>
class mxSprite;
class ResourceManager;
class Config;

///////////////////////////////////////////////////////////////////////////////
/// タイトル画面のシーン
///
class ScnTitle: public Scene {
public:
	static void Create(Surface *pSurface, Config *pConfig, ResourceManager &ResMgr);

	~ScnTitle();

protected:
	int 	UpdateFrame (UINT nFrameCount);
	void 	onLButtonDown (mxWindow *pWnd, BOOL fDoubleClick, const mxPoint &pt, UINT keyFlags);
	void 	onMouseMove (mxWindow *pWnd, const mxPoint &pt, UINT keyFlags);

private:
	Surface *m_Surface;
	Config *m_Config;
	std::vector<mxSprite*> m_vMenuOn;
	int m_Ret;

	ScnTitle(Surface *pSurface, Config *pConfig, ResourceManager &ResMgr);
};

#endif /* SCNTITLE_H_ */
