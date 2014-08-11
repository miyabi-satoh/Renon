///////////////////////////////////////////////////////////////////////////////
/// @file	ScnSnow.h
/// @date	2014/08/01	
/// @author	Masayuki
/// @brief	class ScnSnow
///////////////////////////////////////////////////////////////////////////////

#ifndef SCNSNOW_H_
#define SCNSNOW_H_

#include "Scene.h"
class SnowSprite;
class ResourceManager;
class Surface;

///////////////////////////////////////////////////////////////////////////////
/// 雪のシーンクラス
///
class ScnSnow: public Scene {
public:
	/// 状態識別子
	enum {
		START	= 0x01,	///< 開始
		ACTIVE	= 0x02,	///< 移動中
		PAUSE	= 0x03,	///< 移動停止
		END		= 0x04,	///< 終了
		REMOVE	= 0x05, ///< 削除
		FADEIN	= 0x10,	///< フェードイン
		FADEOUT	= 0x20, ///< フェードアウト
	};

	/// ファクトリーメソッド
	static void Create(ResourceManager &ResMgr);

	/// デストラクタ
	~ScnSnow();

	static void SetState(int n) {
		m_State = n;
		m_dwFadeTime = 0;
	}

	bool 	IsAsync () const { return true; }

protected:
	int 	UpdateFrame (UINT nFrameCount);

private:
	std::vector<SnowSprite*> m_vSnow;	///< 雪の配列
	static int m_State;				///< 状態
	static UINT m_dwFadeTime;			///< フェードの基準時間

	/// コンストラクタ
	ScnSnow(ResourceManager &ResMgr);

};

#endif /* SCNSNOW_H_ */
