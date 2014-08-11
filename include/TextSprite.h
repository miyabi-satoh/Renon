////////////////////////////////////////////////////////////////////////////////
/// @file	TextSprite.h
/// @date	2014/08/02	
/// @author	Masayuki
/// @brief	class TextSprite
////////////////////////////////////////////////////////////////////////////////

#ifndef TEXTSPRITE_H_
#define TEXTSPRITE_H_

#include <sprite.hpp>
#include <font.hpp>
class Config;
class ResourceManager;

////////////////////////////////////////////////////////////////////////////////
/// テキストスプライトクラス
///
class TextSprite: public mxSprite {
public:
	/// コンストラクタ
	TextSprite() : m_Font(), m_pt(), m_Text() {
		m_Config = NULL;
		m_ResMgr = NULL;
		m_FontHeight = 0;
		m_FontWidth = 0;
		m_LineHeight = 0;
		m_IndentChars = 0;
	}
	/// デストラクタ
	~TextSprite() {
	}
	/// フレーム番号の変更
	static void SetFrameIndex(UINT n) { m_FrameIndex = n; }

	/// サイズ変更
	/// @param scale	拡大率
	/// @param rcPaint	描画領域
	void Resize(double scale, const mxRect &rcPaint);

	void 	fastRender (mxDibSection &dstDib, const mxRect &dstRect, const mxPoint &srcPoint);

	void SetConfig(Config *pConfig) { m_Config = pConfig; }
	void SetResourceManager(ResourceManager *pResMgr) { m_ResMgr = pResMgr; }
	void SetIndentChars(int n) { m_IndentChars = n; }

	/// テキストを描画する
	void DrawString(std::wstring s, mxDibSection &dib);

private:
	Config *m_Config;			///< 設定クラス
	ResourceManager *m_ResMgr;	///< リソース管理クラス

	mxFont m_Font;		///< フォント
	int m_FontHeight;	///< フォントの高さ
	int m_FontWidth;	///< フォントの幅
	int m_LineHeight;	///< １行の高さ
	int m_IndentChars;	///< ２行目以降のインデント文字数
	mxPoint m_pt;		///< 出力開始位置
	mxString m_Text;	///< 描画するテキスト

	typedef std::auto_ptr<mxSprite> SpritePtr;
	typedef std::vector<SpritePtr> SpriteContainer;
	SpriteContainer m_vFrameSprite;	///< フレーム(4種)
	static UINT m_FrameIndex;		///< フレーム番号(0～3)

};

#endif /* TEXTSPRITE_H_ */
