///////////////////////////////////////////////////////////////////////////////
/// @file	Surface.h
/// @date	2014/07/30	
/// @author	Masayuki
/// @brief	描画サーフェースクラス
///////////////////////////////////////////////////////////////////////////////

#ifndef SURFACE_H_
#define SURFACE_H_

#include <dibsection.hpp>
#include <sprite.hpp>
#include "TextSprite.h"

///////////////////////////////////////////////////////////////////////////////
/// 描画サーフェースクラス
///
class Surface: public mxDibSection {
public:
	/// Zオーダー
	enum {
		BACK		= 0,	///< 背景
		OBJECT_MIN	= 1,	///< 雪などのオブジェクト最小値
		CHARA		= 10,	///< キャラクタ
		OBJECT_MAX	= 20,	///< 雪などのオブジェクト最大値
		FRAME		= 30,	///< フレーム
		TEXT		= 40,	///< テキスト
		NEXTMARK	= 50,	///< 改頁マーク
	};

	/// コンストラクタ
	Surface(Config *pConfig, ResourceManager &ResMgr);
	/// デストラクタ
	~Surface();

	/// @name メンバアクセス
	//@{
	mxSprite& GetBackSprite() { return *m_BackSprite; }
	mxSprite& GetCharSprite() { return *m_CharSprite; }
	TextSprite& GetTextSprite() { return *m_TextSprite; }
	mxSprite& GetMarkSprite(UINT n) { return *m_vMarkSprite[n]; }
	const mxSprite& GetBackSprite() const { return *m_BackSprite; }
	const mxSprite& GetCharSprite() const { return *m_CharSprite; }
	const TextSprite& GetTextSprite() const { return *m_TextSprite; }
	const mxSprite& GetMarkSprite(UINT n) const { return *m_vMarkSprite[n]; }
	double GetScale() const { return m_Scale; }
	const mxRect& GetPaintRect() const { return m_PaintRect; }
	const mxDibSection& GetBackup() const { return m_Backup; }
	//@}

	/// 背景スプライトへの画像読み込み
	void LoadBackground(LPCTSTR lpszName) {
		LoadFromFile(lpszName, *m_BackSprite);
	}
	/// キャラスプライトへの画像読み込み
	void LoadCharacter(LPCTSTR lpszName) {
		LoadFromFile(lpszName, *m_CharSprite);
		// 透過色を設定する
		m_CharSprite->setColorKey(m_CharSprite->getPixel(mxPoint(0,0)));
	}
	/// スプライトの統合と拡大処理
	void CompositeAndScaling(mxDibSection &dib);

	/// トランジション終了時の処理
	void EndTransition();

	/// バックアップの復元
	void Restore() {
		m_Backup.bitBlt(*this, m_PaintRect);
	}

	/// スプライトの追加
	void InsertSprite(mxSprite *p);
	/// スプライトの削除
	void RemoveSprite(mxSprite *p);
	/// スプライトリストのソート
	void SortSprite();

	/// サイズの変更
	void Resize(const mxSize &sz);

	/// 画像ファイルの読み込み
	static void LoadFromFile(LPCTSTR lpszName, mxDibSection &dib);

private:
	SpriteList m_List;	///< スプライトリスト

	mxSprite *m_BackSprite;		///< 背景
	mxSprite *m_CharSprite;		///< キャラクタ
	TextSprite *m_TextSprite;	///< テキスト
	std::vector<mxSprite*> m_vMarkSprite;	///< 改頁マーク
	double m_Scale;				///< 拡大率
	mxRect m_PaintRect;			///< 描画領域(拡大対応)

	mxDibSection m_Backup;		///< バックアップ
};

#endif /* SURFACE_H_ */
