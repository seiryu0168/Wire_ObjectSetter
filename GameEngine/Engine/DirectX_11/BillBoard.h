#pragma once
#include<string>
#include"Direct3D.h"
#include"Texture.h"
struct CONSTANT_BUFFER
{
	XMMATRIX matWVP;
	XMFLOAT4 color;
};

struct VERTEX
{
	XMFLOAT3 position;
	XMFLOAT3 uv;
};

class BillBoard
{
	ID3D11Buffer* pVertexBuffer_;
	ID3D11Buffer* pConstantBuffer_;
	ID3D11Buffer* pIndexBuffer_;
	int hTexture_;
public:
	BillBoard();
	~BillBoard();

	//ÉçÅ[Éh
	HRESULT Load(std::string fileName);
	
	//ï`âÊ
	void Draw(XMMATRIX matW,XMFLOAT4 col);
	void Release();
};

