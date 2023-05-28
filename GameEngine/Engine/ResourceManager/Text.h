#pragma once
#include"../DirectX_11/Direct2D.h"
//#include"Engine/DirectX_11/Direct3D.h"
#include<string>
typedef D2D_RECT_F TEXT_RECT;//left top right bottom
typedef D2D1_POINT_2F TEXT_POSITION;

class Text
{
	struct FontData
	{
		float fontSize_;
		wchar_t* pFontName_;		//フォント名
		wchar_t* pLocale_;			//ロケール
		DWRITE_FONT_WEIGHT	fontWaight_;		//フォントの太さ
		DWRITE_FONT_STYLE   fontStyle_;			//フォントスタイル
		DWRITE_FONT_STRETCH fontStretch_;
		IDWriteFontCollection* pCollection_;	//フォントコレクション
		FontData()
		{
			fontSize_ = 72.0f;
			pFontName_ = nullptr;
			pLocale_ = nullptr;
			fontWaight_ = DWRITE_FONT_WEIGHT_REGULAR;
			fontStyle_ = DWRITE_FONT_STYLE_NORMAL;
			fontStretch_ = DWRITE_FONT_STRETCH_NORMAL;
			pCollection_ = nullptr;
		}
	};
private:
	size_t				  textLength_;		//テキストの長さ
	D2D1_RECT_F			  layoutRect_;		//レイアウトレクト
	wchar_t*			  pText_;			//テキスト
	wchar_t*			  pFontName_;
	ID2D1SolidColorBrush* pColorBrush_;		//ブラシ	
	IDWriteFactory*		  pWriteFactory_;	//文字描画のファクトリ
	IDWriteTextFormat*	  pTextFormat_;     //テキストフォーマット
	IDWriteTextLayout*	  pLayout_;			//テキストレイアウト
public:
	TEXT_POSITION				  transform2D;		//座標
	Text();
	~Text();
	int Load(const std::string& text, const std::string& fontName, TEXT_RECT rect, STARTING_TYPE type);
	void Draw();
	void SetAlinmentType(STARTING_TYPE type);
	HRESULT SetFont(const FontData& data);
	void SetColor(XMFLOAT4 color);
	HRESULT SetTextSize(float size,UINT32 startPos,UINT32 length);
	HRESULT SetTextSize(float size);
	HRESULT SetFontWeight(DWRITE_FONT_WEIGHT weightType,UINT32 startPos,UINT32 length);
	HRESULT SetText(std::string text);
	void SetTransform(TEXT_POSITION pos);
	void SetTextLayout();
	void SetRatio(float ratioX,float ratioY);
	void SetPosition(XMFLOAT2 position);
	void SetRect(TEXT_RECT rect);
	const TEXT_RECT& GetRect() { return layoutRect_; }
	int GetTextSize();
	void Release();

};

