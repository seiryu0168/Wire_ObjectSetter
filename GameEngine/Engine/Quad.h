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
//�R���X�^���g�o�b�t�@�[
	struct CONSTANT_BUFFER
	{
		XMMATRIX	matWVP;
		XMMATRIX    matNormal;
	};

//���_���
class Quad
{
protected:
	ID3D11Buffer* pVertexBuffer_;	//���_�o�b�t�@
	ID3D11Buffer* pIndexBuffer_;	//�C���f�b�N�X�o�b�t�@
	ID3D11Buffer* pConstantBuffer_;	//�R���X�^���g�o�b�t�@
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

	