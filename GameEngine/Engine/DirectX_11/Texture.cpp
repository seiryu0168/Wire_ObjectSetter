#include <wincodec.h>
#include"Direct3D.h"
#include "Texture.h"

#pragma comment( lib, "WindowsCodecs.lib" )
Texture::Texture()
{
	pSampler_ = nullptr;
	pSRV_ = nullptr;
}

Texture::~Texture()
{
	Release();
}

HRESULT Texture::Load(LPCWSTR fileName)
{
	//画像ファイルロード
	IWICImagingFactory* pFactory = nullptr;
	IWICBitmapDecoder* pDecoder = nullptr;
	IWICBitmapFrameDecode* pFrame = nullptr;
	IWICFormatConverter* pFormatConverter = nullptr;
	HRESULT hr = CoCreateInstance(CLSID_WICImagingFactory, nullptr, CLSCTX_INPROC_SERVER, IID_IWICImagingFactory, reinterpret_cast<void**>(&pFactory));
	hr = pFactory->CreateDecoderFromFilename(fileName, NULL, GENERIC_READ, WICDecodeMetadataCacheOnDemand, &pDecoder);
	if (FAILED(hr))
	{
		MessageBox(nullptr, L"Texture:テクスチャファイルのロードに失敗しました", L"エラー", MB_OK);
		SAFE_RELEASE(pFactory);
		return hr;
	}
	pDecoder->GetFrame(0, &pFrame);
	assert(pDecoder != nullptr);
	pFactory->CreateFormatConverter(&pFormatConverter);
	assert(pFactory != nullptr);
	pFormatConverter->Initialize(pFrame, GUID_WICPixelFormat32bppRGBA, WICBitmapDitherTypeNone, NULL, 1.0f, WICBitmapPaletteTypeMedianCut);
	assert(pFormatConverter != nullptr);
	
	//画像サイズを調べておく
	//UINT imgWidth;
	//UINT imgHeight;
	pFormatConverter->GetSize(&imgWidth_, &imgHeight_);

	//imageWidth_ = imgWidth;
	//imageHeight_ = imgHeight;
	//テクスチャ作成
	ID3D11Texture2D* pTexture;
	D3D11_TEXTURE2D_DESC texdec;
	texdec.Width = imgWidth_;
	texdec.Height = imgHeight_;
	texdec.MipLevels = 1;
	texdec.ArraySize = 1;
	texdec.Format = DXGI_FORMAT_R8G8B8A8_UNORM;
	texdec.SampleDesc.Count = 1;
	texdec.SampleDesc.Quality = 0;
	texdec.Usage = D3D11_USAGE_DYNAMIC;
	texdec.BindFlags = D3D11_BIND_SHADER_RESOURCE;
	texdec.CPUAccessFlags = D3D11_CPU_ACCESS_WRITE;
	texdec.MiscFlags = 0;
	hr = Direct3D::pDevice->CreateTexture2D(&texdec, nullptr, &pTexture);
	if (FAILED(hr))
	{
		MessageBox(nullptr, L"Texture:テクスチャ作成に失敗しました", L"エラー", MB_OK);
		SAFE_RELEASE(pTexture);
		return hr;
	}

	//テクスチャをコンテキストに渡す
	D3D11_MAPPED_SUBRESOURCE hMappedres;
	Direct3D::pContext->Map(pTexture, 0, D3D11_MAP_WRITE_DISCARD, 0, &hMappedres);
	pFormatConverter->CopyPixels(nullptr, imgWidth_ * 4, imgWidth_ * imgHeight_ * 4, (BYTE*)hMappedres.pData);
	Direct3D::pContext->Unmap(pTexture, 0);

	//サンプラー作成
	D3D11_SAMPLER_DESC  SamDesc;
	ZeroMemory(&SamDesc, sizeof(D3D11_SAMPLER_DESC));
	SamDesc.Filter = D3D11_FILTER_MIN_MAG_MIP_LINEAR;
	SamDesc.AddressU = D3D11_TEXTURE_ADDRESS_WRAP;
	SamDesc.AddressV = D3D11_TEXTURE_ADDRESS_WRAP;
	SamDesc.AddressW = D3D11_TEXTURE_ADDRESS_WRAP;
	hr = Direct3D::pDevice->CreateSamplerState(&SamDesc, &pSampler_);
	if (FAILED(hr))
	{
		MessageBox(nullptr, L"Texture:サンプラー作成に失敗しました", L"エラー",MB_OK);
		return hr;
	}
	//シェーダリソースビュー作成
	D3D11_SHADER_RESOURCE_VIEW_DESC srv = {};
	srv.Format = DXGI_FORMAT_R8G8B8A8_UNORM;
	srv.ViewDimension = D3D11_SRV_DIMENSION_TEXTURE2D;
	srv.Texture2D.MipLevels = 1;
	hr = Direct3D::pDevice->CreateShaderResourceView(pTexture, &srv, &pSRV_);
	if (FAILED(hr))
	{
		MessageBox(nullptr, L"Texture:シェーダーリソースビュー作成に失敗しました", L"エラー", MB_OK);
		return hr;
	}
	SAFE_RELEASE(pTexture);

	return S_OK;

}

HRESULT Texture::Load(std::string fileName)
{
	//パス名をファイル名と拡張子だけにする
	char name[_MAX_FNAME];	//ファイル名
	char ext[_MAX_EXT];		//拡張子
	_splitpath_s(fileName.c_str(), nullptr, 0, nullptr, 0, name, _MAX_FNAME, ext, _MAX_EXT);
	sprintf_s(name, "%s%s", name, ext);

	wchar_t wtext[FILENAME_MAX];
	size_t ret;
	mbstowcs_s(&ret, wtext, fileName.c_str(), fileName.length());

	//画像ファイルロード
	IWICImagingFactory* pFactory = nullptr;
	IWICBitmapDecoder* pDecoder = nullptr;
	IWICBitmapFrameDecode* pFrame = nullptr;
	IWICFormatConverter* pFormatConverter = nullptr;
	HRESULT hr=CoCreateInstance(CLSID_WICImagingFactory, nullptr, CLSCTX_INPROC_SERVER, IID_IWICImagingFactory, reinterpret_cast<void**>(&pFactory));
	hr = pFactory->CreateDecoderFromFilename(wtext, NULL, GENERIC_READ, WICDecodeMetadataCacheOnDemand, &pDecoder);
	if (FAILED(hr))
	{
		MessageBox(nullptr, L"Texture:テクスチャファイルのロードに失敗しました", L"エラー", MB_OK);
		SAFE_RELEASE(pFactory);
		return hr;
	}
	pDecoder->GetFrame(0, &pFrame);
	assert(pDecoder != nullptr);
	pFactory->CreateFormatConverter(&pFormatConverter);
	assert(pFactory != nullptr);
	pFormatConverter->Initialize(pFrame, GUID_WICPixelFormat32bppRGBA, WICBitmapDitherTypeNone, NULL, 1.0f, WICBitmapPaletteTypeMedianCut);
	assert(pFormatConverter != nullptr);

	//画像サイズを調べておく
	//UINT imgWidth;
	//UINT imgHeight;
	pFormatConverter->GetSize(&imgWidth_, &imgHeight_);

	//imageWidth_ = imgWidth;
	//imageHeight_ = imgHeight;
	//テクスチャ作成
	ID3D11Texture2D* pTexture;
	D3D11_TEXTURE2D_DESC texdec;
	texdec.Width = imgWidth_;
	texdec.Height = imgHeight_;
	texdec.MipLevels = 1;
	texdec.ArraySize = 1;
	texdec.Format = DXGI_FORMAT_R8G8B8A8_UNORM;
	texdec.SampleDesc.Count = 1;
	texdec.SampleDesc.Quality = 0;
	texdec.Usage = D3D11_USAGE_DYNAMIC;
	texdec.BindFlags = D3D11_BIND_SHADER_RESOURCE;
	texdec.CPUAccessFlags = D3D11_CPU_ACCESS_WRITE;
	texdec.MiscFlags = 0;
	hr = Direct3D::pDevice->CreateTexture2D(&texdec, nullptr, &pTexture);
	if (FAILED(hr))
	{
		MessageBox(nullptr, L"Texture:テクスチャ作成に失敗しました", L"エラー", MB_OK);
		SAFE_RELEASE(pTexture);
		return hr;
	}

	//テクスチャをコンテキストに渡す
	D3D11_MAPPED_SUBRESOURCE hMappedres;
	Direct3D::pContext->Map(pTexture, 0, D3D11_MAP_WRITE_DISCARD, 0, &hMappedres);
	pFormatConverter->CopyPixels(nullptr, imgWidth_ * 4, imgWidth_ * imgHeight_ * 4, (BYTE*)hMappedres.pData);
	Direct3D::pContext->Unmap(pTexture, 0);

	//サンプラー作成
	D3D11_SAMPLER_DESC  SamDesc;
	ZeroMemory(&SamDesc, sizeof(D3D11_SAMPLER_DESC));
	SamDesc.Filter = D3D11_FILTER_MIN_MAG_MIP_LINEAR;
	SamDesc.AddressU = D3D11_TEXTURE_ADDRESS_CLAMP;
	SamDesc.AddressV = D3D11_TEXTURE_ADDRESS_CLAMP;
	SamDesc.AddressW = D3D11_TEXTURE_ADDRESS_CLAMP;
	hr = Direct3D::pDevice->CreateSamplerState(&SamDesc, &pSampler_);
	if (FAILED(hr))
	{
		MessageBox(nullptr, L"Texture:サンプラー作成に失敗しました", L"エラー", MB_OK);
		return hr;
	}
	//シェーダリソースビュー作成
	D3D11_SHADER_RESOURCE_VIEW_DESC srv = {};
	srv.Format = DXGI_FORMAT_R8G8B8A8_UNORM;
	srv.ViewDimension = D3D11_SRV_DIMENSION_TEXTURE2D;
	srv.Texture2D.MipLevels = 1;
	hr = Direct3D::pDevice->CreateShaderResourceView(pTexture, &srv, &pSRV_);
	if (FAILED(hr))
	{
		MessageBox(nullptr, L"Texture:シェーダーリソースビュー作成に失敗しました", L"エラー", MB_OK);
		return hr;
	}
	SAFE_RELEASE(pTexture);
	SAFE_RELEASE(pFormatConverter);
	SAFE_RELEASE(pFrame);
	SAFE_RELEASE(pDecoder);
	SAFE_RELEASE(pFactory);

	return S_OK;

}



UINT Texture::GetWidth()
{
	return imgWidth_;
}

UINT Texture::GetHeight()
{
	return imgHeight_;
}

void Texture::Release()
{
	SAFE_RELEASE(pSRV_);
	SAFE_RELEASE(pSampler_);
}
