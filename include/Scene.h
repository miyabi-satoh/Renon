///////////////////////////////////////////////////////////////////////////////
/// @file	Scene.h
/// @date	2014/07/29	
/// @author	Masayuki
/// @brief	class Scene, class ScnWait
///////////////////////////////////////////////////////////////////////////////

#ifndef SCENE_H_
#define SCENE_H_

#include <common.hpp>

class mxWindow;
class Surface;

///////////////////////////////////////////////////////////////////////////////
/// シーンの抽象基底クラス
///
class Scene {
public:
	enum {
		END = -1,
		Next = 0
	};

	/// デストラクタ
	virtual ~Scene() {
	}

	/// シーンリストのクリア
	static void Clear() { List.clear(); }

	/// 非同期シーンか
	virtual bool IsAsync() const { return false; }

	/// フレーム更新処理
	static int onUpdate();

	int UpdateFrame() {
		return UpdateFrame(++m_FrameCount);
	}

	/// @name メッセージハンドラ
	//@{
	static void OnLButtonDown(mxWindow *pWnd, BOOL fDoubleClick, const mxPoint &pt, UINT keyFlags);
	static void OnMouseMove(mxWindow *pWnd, const mxPoint &pt, UINT keyFlags);
	static void OnSize(mxWindow *pWnd, UINT state, const mxSize &sz);
	//@}

protected:
	typedef std::auto_ptr<Scene> Ptr;
	typedef std::list<Ptr> Container;
	static Container List;

	/// コンストラクタ
	Scene() {
		List.push_back(Ptr(this));
		m_FrameCount = 0;
	}

	/// フレームカウントの取得
	UINT GetCount() { return m_FrameCount; }

	/// フレームカウントの初期化
	void ResetCount() { m_FrameCount = 0; }

	/// フレーム更新処理
	virtual int UpdateFrame(UINT nFrameCount) = 0;

	/// @name メッセージハンドラ
	//@{
	virtual void onLButtonDown(mxWindow *pWnd, BOOL fDoubleClick, const mxPoint &pt, UINT keyFlags)
	{
	}
	virtual void onMouseMove(mxWindow *pWnd, const mxPoint &pt, UINT keyFlags)
	{
	}
	virtual void onSize(mxWindow *pWnd, UINT state, const mxSize &sz)
	{
	}
	//@}

private:
	UINT m_FrameCount;	///< フレームカウント
};

///////////////////////////////////////////////////////////////////////////////
/// 指定時間ウェイトするシーンクラス
///
class ScnWait : public Scene
{
public:
	static void Create(Surface *pSurface, int option);

protected:
	int UpdateFrame(UINT nFrameCount);

private:
	Surface *m_Surface;
	DWORD m_WaitTime;	///< ウェイト時間
	DWORD m_StartTime;	///< ウェイト開始時間

	ScnWait(Surface *pSurface, int nWaitTime) : Scene() {
		m_Surface = pSurface;
		m_WaitTime = nWaitTime;
		m_StartTime = 0;
	}

};


#endif /* SCENE_H_ */
