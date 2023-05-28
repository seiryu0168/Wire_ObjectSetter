#include "Direct2D.h"
#include"../SAFE_DELETE_RELEASE.h"

namespace D2D
{
	ID2D1Factory*		   pFactory_	  = nullptr;	//ファクトリ
	ID2D1RenderTarget* pRenderTarget_ = nullptr;    //レンダーターゲット

	//ID2D1SolidColorBrush*  pColorBrush_   = nullptr;	//ブラシ	
	//IDWriteFactory*		   pWriteFactory_ = nullptr;	//文字描画のファクトリ
	//IDWriteTextFormat*	   pTextFormat_   = nullptr;	//テキストフォーマット
	//const wchar_t*		   pWszText       = nullptr;	//テキスト
	UINT32				   textLength_	  = 0;
	D2D1_RECT_F			   layoutRect_;
	float				   dpiScaleX_;
	float				   dpiScaleY_;
}

HRESULT D2D::Initialize(int winW, int winH, HWND hWnd)
{
	HRESULT hr;
	hr= D2D1CreateFactory(D2D1_FACTORY_TYPE_SINGLE_THREADED, &pFactory_);
	if (FAILED(hr))
	{
		MessageBox(nullptr, L"ファクトリの作成に失敗", L"エラー", MB_OK);
		return hr;
	}
	IDXGISurface* pBackBuffer;
	Direct3D::GetSwapChain()->GetBuffer(0, IID_PPV_ARGS(&pBackBuffer));
	//hr = DWriteCreateFactory(DWRITE_FACTORY_TYPE_SHARED,__uuidof(IDWriteFactory),reinterpret_cast<IUnknown**>(&pWriteFactory_));
	//if (FAILED(hr))
	//{
	//	MessageBox(nullptr, L"Direct2D : テキスト描画用ファクトリの作成に失敗", L"エラー", MB_OK);
	//	return hr;
	//}
	//pWszText = L"Hello World!";
	//textLength_ = (UINT32)wcslen(pWszText);
	//
	////文字のフォーマット作成
	//hr = pWriteFactory_->CreateTextFormat(L"Gabliora", NULL, DWRITE_FONT_WEIGHT_REGULAR, DWRITE_FONT_STYLE_NORMAL, DWRITE_FONT_STRETCH_NORMAL, 72.0f, L"en-us", &pTextFormat_);
	//if (FAILED(hr))
	//{
	//	MessageBox(nullptr, L"Direct2D : テキストフォント作成に失敗", L"エラー", MB_OK);
	//	return hr;
	//}
	//
	//
	////アライメント設定
	//hr = pTextFormat_->SetTextAlignment(DWRITE_TEXT_ALIGNMENT_JUSTIFIED);
	//if (FAILED(hr))
	//{
	//	MessageBox(nullptr, L"Direct2D : アライメント設定に失敗", L"エラー", MB_OK);
	//	return hr;
	//}
	//
	//hr = pTextFormat_->SetParagraphAlignment(DWRITE_PARAGRAPH_ALIGNMENT_CENTER);
	//if (FAILED(hr))
	//{
	//	MessageBox(nullptr, L"Direct2D : アライメント設定に失敗", L"エラー", MB_OK);
	//	return hr;
	//}
	RECT rect;
	GetClientRect(hWnd, &rect);
	
	dpiScaleX_ = (float)GetDpiForWindow(hWnd);
	dpiScaleY_ = (float)GetDpiForWindow(hWnd);
	//D2D1_SIZE_U size = D2D1::Size<UINT>(rect.right, rect.bottom);
	D2D1_RENDER_TARGET_PROPERTIES prop = D2D1::RenderTargetProperties(D2D1_RENDER_TARGET_TYPE_DEFAULT, D2D1::PixelFormat(DXGI_FORMAT_UNKNOWN, D2D1_ALPHA_MODE_PREMULTIPLIED), dpiScaleX_, dpiScaleY_);
	hr = pFactory_->CreateDxgiSurfaceRenderTarget(pBackBuffer,prop , &pRenderTarget_);
	if (FAILED(hr))
	{
		MessageBox(nullptr, L"Direct2D : レンダーターゲットの作成に失敗", L"エラー", MB_OK);
		return hr;
	}

	//hr = pRenderTarget_->CreateSolidColorBrush(D2D1::ColorF(D2D1::ColorF::White),&pColorBrush_);
	//if (FAILED(hr))
	//{
	//	MessageBox(nullptr, L"Direct2D : ブラシの作成に失敗", L"エラー", MB_OK);
	//	return hr;
	//}
	//XMINT2 trns(0, 0);
	//rect.top += trns.y;
	//rect.bottom += trns.y;
	//rect.left += trns.x;
	//rect.right += trns.x;
	//layoutRect_ = D2D1::RectF(static_cast<FLOAT>(rect.left) / dpiScaleX_,
	//						  static_cast<FLOAT>(rect.top) / dpiScaleY_,
	//						  static_cast<FLOAT>(rect.right - rect.left) / dpiScaleX_,
	//						  static_cast<FLOAT>(rect.bottom - rect.top) / dpiScaleY_);	
	//layoutRect_ = D2D1::RectF(static_cast<FLOAT>(rect.left),
	//	static_cast<FLOAT>(rect.top),
	//	static_cast<FLOAT>(rect.right - rect.left),
	//	static_cast<FLOAT>(rect.bottom - rect.top));



	return S_OK;
}

ID2D1Factory* D2D::Get2DFactory()
{
	return pFactory_;
}

ID2D1RenderTarget* D2D::GetRenderTarget()
{
	return pRenderTarget_;
}

void D2D::Release()
{
	SAFE_RELEASE(pFactory_);
	SAFE_RELEASE(pRenderTarget_);
}

void D2D::RenderTest()
{
	HRESULT hr;
	ID2D1SolidColorBrush* pGreenBrush=nullptr;
	
	hr = pRenderTarget_->CreateSolidColorBrush(D2D1::ColorF(1.0f, 1.0f, 0.0f), &pGreenBrush);
	if (FAILED(hr))
	{
		MessageBox(nullptr, L"Direct2D : テスト失敗", L"エラー", MB_OK);
		//return hr;
	}
	
	//D2D1_ELLIPSE ell = D2D1::Ellipse(D2D1::Point2F(1120.0f, 120.0f), 100.0f, 100.0f);
	//pRenderTarget_->DrawEllipse(ell, pGreenBrush, 10.0f);

	


	SAFE_RELEASE(pGreenBrush);
}

void D2D::BeginDraw()
{
	//pRenderTarget_->Clear();
	pRenderTarget_->BeginDraw();
}

//void D2D::Draw()
//{
//	pRenderTarget_->BeginDraw();
//	pRenderTarget_->SetTransform(D2D1::IdentityMatrix());	
//	pRenderTarget_->DrawTextW(pWszText, textLength_, pTextFormat_, layoutRect_, pColorBrush_);
//}

void D2D::EndDraw()
{
	pRenderTarget_->EndDraw();
}

int D2D::GetdpiX()
{
	return (int)dpiScaleX_;
}

int D2D::GetdpiY()
{
	return (int)dpiScaleY_;
}
