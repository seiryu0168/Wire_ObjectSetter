#include "Dice.h"

Dice::Dice()
{
}
Dice::~Dice()
{

}

//HRESULT Dice::Initialize()
//{
//	 InitVertex();
//	 InitIndex();
//	if (FAILED(CreateVertexBuffer()))
//	{
//		return E_FAIL;
//	}
//	if (FAILED(CreateIndexBuffer()))
//	{
//		return E_FAIL;
//	}
//	if (FAILED(CreateConstantBuffer()))
//	{
//		return E_FAIL;
//	}
//	if (FAILED(TextureSet(L"Assets\\dice.png")))
//	{
//		return E_FAIL;
//	}
//
//	 return S_OK;
//}
void Dice::InitVertex()
{
	//���_��UV���W
	VERTEX vertices[] =
	{
		//1
		XMVectorSet(-1.0f,  1.0f, -1.0f, 0.0f),	XMVectorSet(0.0f,0.0f,0.0f,0.0f),XMVectorSet(0.0f,0.0f,-1.0f,0.0f),		// �l�p�`�̒��_�i����j
		XMVectorSet(1.0f,  1.0f, -1.0f, 0.0f),	XMVectorSet(0.25f,0.0f,0.0f,0.0f),XMVectorSet(0.0f,0.0f,-1.0f,0.0f),	// �l�p�`�̒��_�i�E��j
		XMVectorSet(1.0f, -1.0f, -1.0f, 0.0f),	XMVectorSet(0.25f,0.5f,0.0f,0.0f),XMVectorSet(0.0f,0.0f,-1.0f,0.0f),	// �l�p�`�̒��_�i�E���j
		XMVectorSet(-1.0f, -1.0f, -1.0f, 0.0f),	XMVectorSet(0.0f,0.5f,0.0f,0.0f), XMVectorSet(0.0f,0.0f,-1.0f,0.0f),	// �l�p�`�̒��_�i�����j
		//2
		XMVectorSet(-1.0f,  1.0f, 1.0f, 0.0f),	XMVectorSet(0.25f,0.0f,0.0f,0.0f),XMVectorSet(0.0f,1.0f,0.0f,0.0f),		// �l�p�`�̒��_�i���㉜�j
		XMVectorSet(1.0f,  1.0f, 1.0f, 0.0f),	XMVectorSet(0.5f,0.0f,0.0f,0.0f),XMVectorSet(0.0f,1.0f,0.0f,0.0f),		// �l�p�`�̒��_�i�E�㉜�j
		XMVectorSet(1.0f, 1.0f, -1.0f, 0.0f),	XMVectorSet(0.5f,0.5f,0.0f,0.0f),XMVectorSet(0.0f,1.0f,0.0f,0.0f),		// �l�p�`�̒��_�i�E��j
		XMVectorSet(-1.0f, 1.0f, -1.0f, 0.0f),	XMVectorSet(0.25f,0.5f,0.0f,0.0f), XMVectorSet(0.0f,1.0f,0.0f,0.0f),	// �l�p�`�̒��_�i����j
		//3
		XMVectorSet(-1.0f,  1.0f, 1.0f, 0.0f),	XMVectorSet(0.5f,0.0f,0.0f,0.0f),XMVectorSet(-1.0f,0.0f,0.0f,0.0f),		// �l�p�`�̒��_�i���㉜�j
		XMVectorSet(-1.0f,  1.0f, -1.0f, 0.0f),	XMVectorSet(0.75f,0.0f,0.0f,0.0f),XMVectorSet(-1.0f,0.0f,0.0f,0.0f),	// �l�p�`�̒��_�i����j
		XMVectorSet(-1.0f, -1.0f, -1.0f, 0.0f),	XMVectorSet(0.75f,0.5f,0.0f,0.0f),XMVectorSet(-1.0f,0.0f,0.0f,0.0f),	// �l�p�`�̒��_�i�����j
		XMVectorSet(-1.0f, -1.0f, 1.0f, 0.0f),	XMVectorSet(0.5f,0.5f,0.0f,0.0f), XMVectorSet(-1.0f,0.0f,0.0f,0.0f),	// �l�p�`�̒��_�i�������j
		//4
		XMVectorSet(1.0f,  1.0f, -1.0f, 0.0f),	XMVectorSet(0.75f,0.0f,0.0f,0.0f),XMVectorSet(1.0f,0.0f,0.0f,0.0f),		// �l�p�`�̒��_�i�E��j
		XMVectorSet(1.0f,  1.0f, 1.0f, 0.0f),	XMVectorSet(1.0f,0.0f,0.0f,0.0f),	XMVectorSet(1.0f,0.0f,0.0f,0.0f),	// �l�p�`�̒��_�i�E�㉜�j
		XMVectorSet(1.0f, -1.0f, 1.0f, 0.0f),	XMVectorSet(1.0f,0.5f,0.0f,0.0f),	XMVectorSet(1.0f,0.0f,0.0f,0.0f),	// �l�p�`�̒��_�i�E�����j
		XMVectorSet(1.0f, -1.0f, -1.0f, 0.0f),	XMVectorSet(0.75f,0.5f,0.0f,0.0f), XMVectorSet(1.0f,0.0f,0.0f,0.0f),	// �l�p�`�̒��_�i�E���j
		//5
		XMVectorSet(-1.0f,  -1.0f, -1.0f, 0.0f),	XMVectorSet(0.0f,0.5f,0.0f,0.0f),	XMVectorSet(0.0f,-1.0f,0.0f,0.0f),	// �l�p�`�̒��_�i�����j
		XMVectorSet(1.0f,  -1.0f, -1.0f, 0.0f),	XMVectorSet(0.25f,0.5f,0.0f,0.0f),	XMVectorSet(0.0f,-1.0f,0.0f,0.0f),	// �l�p�`�̒��_�i�E���j
		XMVectorSet(1.0f, -1.0f, 1.0f, 0.0f),	XMVectorSet(0.25f,1.0f,0.0f,0.0f),	XMVectorSet(0.0f,-1.0f,0.0f,0.0f),	// �l�p�`�̒��_�i�E�����j
		XMVectorSet(-1.0f, -1.0f, 1.0f, 0.0f),	XMVectorSet(0.0f,1.0f,0.0f,0.0f),   XMVectorSet(0.0f,-1.0f,0.0f,0.0f),	// �l�p�`�̒��_�i�������j
		//6
		XMVectorSet(1.0f, 1.0f, 1.0f, 0.0f),	XMVectorSet(0.5f,0.5f,0.0f,0.0f),   XMVectorSet(0.0f,0.0f,1.0f,0.0f),	// �l�p�`�̒��_�i���㉜�j
		XMVectorSet(-1.0f, 1.0f, 1.0f, 0.0f),	XMVectorSet(0.25f,0.5f,0.0f,0.0f),   XMVectorSet(0.0f,0.0f,1.0f,0.0f),	// �l�p�`�̒��_�i�E�㉜�j
		XMVectorSet(-1.0f, -1.0f, 1.0f, 0.0f),	XMVectorSet(0.25f,1.0f,0.0f,0.0f),   XMVectorSet(0.0f,0.0f,1.0f,0.0f),	// �l�p�`�̒��_�i�E�����j
		XMVectorSet(1.0f, -1.0f, 1.0f, 0.0f),	XMVectorSet(0.5f,1.0f,0.0f,0.0f),    XMVectorSet(0.0f,0.0f,1.0f,0.0f),	// �l�p�`�̒��_�i�������j
	};

	vertexNum_ = sizeof(vertices) / sizeof(VERTEX);
	vertices_ = new VERTEX[vertexNum_];
	memcpy(vertices_, vertices, sizeof(vertices));
	
}
void Dice::InitIndex()
{
	//�C���f�b�N�X
	int index[] = { 0,1,2, 0,2,3, 4,5,6, 4,6,7, 8,9,10, 8,10,11, 12,13,14, 12,14,15, 16,17,18, 16,18,19, 20,21,22, 20,22,23 };

	indexNum_ = sizeof(index) / sizeof(int);
	index_ = new int[indexNum_];
	memcpy(index_, index, sizeof(index));
}