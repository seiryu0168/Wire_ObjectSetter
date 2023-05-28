#pragma once
#include"Direct3D.h"
#include"Texture.h"
#include"Transform.h"
//#define SAFE_DELETE_ARRAY(p)  if (p != nullptr) { delete [] p; p = nullptr; }

	struct VERTEX
	{
		XMVECTOR position;
		XMVECTOR uv;
		XMVECTOR normal;
	};
//コンスタントバッファー
	struct CONSTANT_BUFFER
	{
		XMMATRIX	matWVP;
		XMMATRIX    matNormal;
	};

//頂点情報
class Quad
{
protected:
	ID3D11Buffer* pVertexBuffer_;	//頂点バッファ
	ID3D11Buffer* pIndexBuffer_;	//インデックスバッファ
	ID3D11Buffer* pConstantBuffer_;	//コンスタントバッファ
	Texture* pTexture_;

	DWORD vertexNum_;
	VERTEX* vertices_;
	DWORD indexNum_;
	int* index_;




public:
	Quad();
	~Quad();
	virtual HRESULT Initialize();
	HRESULT CreateVertexBuffer();
	HRESULT CreateIndexBuffer();
	HRESULT CreateConstantBuffer();
	HRESULT TextureSet(LPCWSTR fileName);

	virtual void InitVertex();
	virtual void InitIndex();

	void ToPipeLine(Transform& transform);
	void bufferSet();
	void Draw(Transform& transform);
	void Release();
};

	