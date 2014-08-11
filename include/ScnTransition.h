///////////////////////////////////////////////////////////////////////////////
/// @file	ScnTransition.h
/// @date	2014/07/30	
/// @author	Masayuki
/// @brief	画面切り替えのシーン
///////////////////////////////////////////////////////////////////////////////

#ifndef SCNTRANSITION_H_
#define SCNTRANSITION_H_

#include "Scene.h"

class Surface;

///////////////////////////////////////////////////////////////////////////////
/// 画面切り替えシーンのファクトリークラス
///
class ScnTransition: public Scene {
public:
	static void Create(
			Surface *pSurface,
			const std::wstring type,
			int option);

private:
	ScnTransition() : Scene() {
	}
	~ScnTransition() {
	}
};

#endif /* SCNTRANSITION_H_ */
