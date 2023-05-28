#pragma once
#include"Direct3D.h"
#include"Texture.h"
#include"../GameObject/Transform.h"
#include"../SAFE_DELETE_RELEASE.h"

class Sprite
{
	struct VERTEX
	{
		XMVECTOR position;
		XMVECTOR uv;
	};
	//�R���X�^���g�o�b�t�@�[
	struct CONSTANT_BUFFER
	{
		XMMATRIX matWorld;
		XMMATRIX matUVTrans;
		XMFLOAT4 color;
		XMFLOAT4 ChangeColor;
	};
protected:
	ID3D11Buffer* pVertexBuffer_;	//���_�o�b�t�@
	ID3D11Buffer* pIndexBuffer_;	//�C���f�b�N�X�o�b�t�@
	ID3D11Buffer* pConstantBuffer_;	//�R���X�^���g�o�b�t�@

	DWORD vertexNum_;
	VERTEX* vertices_;
	DWORD indexNum_;
	int* index_;
	
	XMFLOAT3 size_;
	int hPict_;
	HRESULT CreateVertexBuffer();
	HRESULT CreateIndexBuffer();
	HRESULT CreateConstantBuffer();

	void ToPipeLine(Transform& transform);
	void bufferSet();
	void Release();

public:
	Sprite();
	~Sprite();
	virtual HRESULT Initialize();
	virtual void InitVertex();
	virtual void InitIndex();
	HRESULT Load(std::string fileName);
	
	void SetSize(float width, float height) { size_ = { width,height, 1.0f}; }
	XMFLOAT3 GetSize() { return size_; }
	void Draw(Transform& transform,RECT rect,XMFLOAT4 changeColor,float alpha);
};

