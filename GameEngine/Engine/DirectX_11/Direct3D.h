#pragma once
#include <d3d11.h>
#include<assert.h>
#include"Math.h"
#include<vector>
#include"../SAFE_DELETE_RELEASE.h"
using namespace DirectX;

//�����J
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
	extern ID3D11Device* pDevice;				//�f�o�C�X
	extern ID3D11DeviceContext* pContext;		//�f�o�C�X�R���e�L�X�g
	

	//extern int screenWidth;
	//extern int screenHeight;
	

	//������
	HRESULT Initialize(int winW, int winH, HWND hWnd);

	//�V�F�[�_�[����
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

	//�`��J�n
	void BeginDraw();

	//�`��I��
	void EndDraw();

	//���
	void Release();

	//�C�����C���Ŋ֐���u��������̂ŏ��������Ȃ�
	//�ς���ꂽ���Ȃ�������const������
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
	////�������I�y���[�^�[�͎Q�Ƃň�����ݒ肷��K�v������
	//
	//inline XMVECTOR operator*=(XMVECTOR& v1, const XMMATRIX& m1)
	//{
	//	v1 = XMVector3TransformCoord(v1, m1);
	//	return v1;
	//}
};