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
	TextSprite(Config *pConfig, mxDibSection &dibGRDAT);
	/// デストラクタ
	~TextSprite() {
	}
	void 	fastRender (mxDibSection &dstDib, const mxRect &dstRect, const mxPoint &srcPoint);

	/// フレーム番号の変更
	static void SetFrameIndex(UINT n) { m_FrameIndex = n; }

	/// サイズ変更
	/// @param scale	拡大率
	/// @param rcPaint	描画領域
	void Resize(double scale, const mxRect &rcPaint);

	/// 2行目以降のインデント文字数を設定する
	void SetIndentChars(int n) { m_IndentChars = n; }

	/// テキストを描画する
	void DrawString(std::wstring s, mxDibSection &dib);

private:
	Config *m_Config;			///< 設定クラス

	mxFont m_Font;		///< フォント
	int m_FontHeight;	///< フォントの高さ
	int m_FontWidth;	///< フォントの幅
	int m_LineHeight;	///< １行の高さ
	int m_IndentChars;	///< ２行目以降のインデント文字数
	mxPoint m_pt;		///< 出力開始位置
	mxString m_Text;	///< 描画するテキスト

	static SpriteVector m_vFrameSprite;	///< フレーム(4種)
	static UINT m_FrameIndex;		///< フレーム番号(0～3)
};

#endif /* TEXTSPRITE_H_ */
