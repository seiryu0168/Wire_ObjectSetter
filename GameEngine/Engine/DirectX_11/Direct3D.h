#pragma once
#include <d3d11.h>
#include<assert.h>
#include"Math.h"
#include<vector>
#include"../SAFE_DELETE_RELEASE.h"
using namespace DirectX;

//リンカ
#pragma comment(lib, "d3d11.lib")
#pragma comment(lib, "d3dcompiler.lib")

enum class SHADER_TYPE
{
	SHADER_2D = 0,
	SHADER_3D,
	SHADER_EFF,
	SHADER_OUTLINE,
	SHADER_TOON,
	SHADER_MAX,
};
enum classTARGET
{
	TARGET_BACK_BUFFER=0,
	TARGET_RENDER_TARGET_1,
	TARGET_RENDER_TARGET_2,
	TARGET_RENDER_TARGET_3,
	TARGET_RENDER_TARGET_MAX
};

enum class BLEND_MODE
{
	BLEND_DEFAULT = 0,
	BLEND_ADD,
	BLEND_MAX
};


namespace Direct3D
{
	enum RENDER_TYPE
	{
		BACK_BUFFER = 0,
		RENDER_TARGET,
		RENDER_MAX
	};
	extern ID3D11Device* pDevice;				//デバイス
	extern ID3D11DeviceContext* pContext;		//デバイスコンテキスト
	

	//extern int screenWidth;
	//extern int screenHeight;
	

	//初期化
	HRESULT Initialize(int winW, int winH, HWND hWnd);

	//シェーダー準備
    HRESULT InitShader();
	HRESULT InitShader2D();
	HRESULT InitShader3D();
	HRESULT InitShaderEFF();
	HRESULT InitShaderOutLine();
	HRESULT InitShaderToon();

	void SetShader(SHADER_TYPE type);
	void SetBlendMode(BLEND_MODE mode);
	ID3D11Device* GetDevice();
	ID3D11DeviceContext* GetContext();
	IDXGISwapChain* GetSwapChain();
	int GetScreenWidth();
	int GetScreenHeight();
	HWND GetWindowHandle(); 
	void SetDepthBufferWriteEnable(bool isWrite);

	//描画開始
	void BeginDraw();

	//描画終了
	void EndDraw();

	//解放
	void Release();

	//インラインで関数を置き換えるので少し早くなる
	//変えられたくない引数はconstをつける
	//inline XMVECTOR operator-(const XMFLOAT3& p1, const XMFLOAT3& p2)
	//{
	//	XMVECTOR a;
	//	XMVECTOR b;
	//
	//
	//
	//	return a + b;// XMLoadFloat3(&p1) - XMLoadFloat3(&p2);
	//}
	//
	////＝がつくオペレーターは参照で引数を設定する必要がある
	//
	//inline XMVECTOR operator*=(XMVECTOR& v1, const XMMATRIX& m1)
	//{
	//	v1 = XMVector3TransformCoord(v1, m1);
	//	return v1;
	//}
};