////////////////////////////////////////////////////////////////////////////////
/// @file	ScnSpecial.h
/// @date	2014/08/13	
/// @author	masayuki
/// @brief	class ScnSpecial
////////////////////////////////////////////////////////////////////////////////

#ifndef SCNSPECIAL_H_
#define SCNSPECIAL_H_

#include <Scene.h>
class Surface;
class ResourceManager;

///////////////////////////////////////////////////////////////////////////////
/// 特殊演出シーンのファクトリークラス
///
class ScnSpecial: public Scene {
public:
	static void Create(std::wstring type, Surface *pSurface, ResourceManager &ResMgr);

	~ScnSpecial() {
	}

private:
	ScnSpecial() {
	}

};

#endif /* SCNSPECIAL_H_ */
