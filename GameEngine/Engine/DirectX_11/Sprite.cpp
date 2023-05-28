#include "Sprite.h"
#include"../ResourceManager/TextureManager.h"
Sprite::Sprite()
{
	indexNum_ = 0;
	size_ = { 0,0,0 };
	vertexNum_ = 0;
	hPict_			 = -1;
	pVertexBuffer_	 = nullptr;
	pIndexBuffer_	 = nullptr;
	pConstantBuffer_ = nullptr;
	vertices_		 = nullptr;
	index_			 = nullptr;
}

Sprite::~Sprite()
{
	Release();
}

HRESULT Sprite::Initialize()
{
	InitVertex();
	InitIndex();
	if (FAILED(CreateVertexBuffer()))
	{
		return E_FAIL;
	}

	// �C���f�b�N�X�o�b�t�@�𐶐�����
	if (FAILED(CreateIndexBuffer()))
	{
		return E_FAIL;
	}

	//�R���X�^���g�o�b�t�@�쐬
	if (FAILED(CreateConstantBuffer()))
	{
		return E_FAIL;
	}

	SetSize((float)TextureManager::GetTexture(hPict_)->GetWidth(), (float)TextureManager::GetTexture(hPict_)->GetHeight());
	return S_OK;
}

void Sprite::Draw(Transform& transform, RECT rect, XMFLOAT4 changeColor,float alpha)
{
	Direct3D::SetShader(SHADER_TYPE::SHADER_2D);
	Direct3D::SetBlendMode(BLEND_MODE::BLEND_DEFAULT);
	Direct3D::SetDepthBufferWriteEnable(false);
	//�R���X�^���g�o�b�t�@�ɏ���n��
	transform.Calclation();

	//��ʂ̃T�C�Y�ɍ��킹��s��
	XMMATRIX matImageSize = XMMatrixScaling((float)(1.0f / Direct3D::GetScreenWidth()), (float)(1.0f / Direct3D::GetScreenHeight()), 1.0f);
	//�؂蔲���T�C�Y�ɍ��킹��s��
	XMMATRIX matCut = XMMatrixScaling((float)rect.right, (float)rect.bottom, 1.0f);
	CONSTANT_BUFFER cb;
	
	//�ŏI�I�ȍs��
	cb.matWorld = XMMatrixTranspose(matCut*transform.GetWorldScaleMatrix() * matImageSize * transform.GetWorldRotateMatrix() * transform.GetWorldTranslateMatrix());
	
	XMMATRIX matTexTrans = XMMatrixTranslation((float)rect.left / size_.x, (float)rect.top / size_.y, 1.0f);
	XMMATRIX matTexScale = XMMatrixScaling((float)rect.right / size_.x, (float)rect.bottom / size_.y, 1.0f);

	cb.matUVTrans = XMMatrixTranspose(matTexScale * matTexTrans);
	cb.color = XMFLOAT4(1, 1, 1, alpha);
	cb.ChangeColor = changeColor;

	D3D11_MAPPED_SUBRESOURCE pdata;
	Direct3D::pContext->Map(pConstantBuffer_, 0, D3D11_MAP_WRITE_DISCARD, 0, &pdata);	// GPU����̃f�[�^�A�N�Z�X���~�߂�
	memcpy_s(pdata.pData, pdata.RowPitch, (void*)(&cb), sizeof(cb));					// �f�[�^��l�𑗂�

	ID3D11SamplerState* pSampler = TextureManager::GetTexture(hPict_)->GetSampler();
	Direct3D::pContext->PSSetSamplers(0, 1, &pSampler);
	ID3D11ShaderResourceView* pSRV = TextureManager::GetTexture(hPict_)->GetSRV();
	Direct3D::pContext->PSSetShaderResources(0, 1, &pSRV);

	Direct3D::pContext->Unmap(pConstantBuffer_, 0);//�ĊJ
	
	//���_�A�C���f�b�N�X�A�R���X�^���g�o�b�t�@���Z�b�g
	
	//���_�o�b�t�@
	UINT stride = sizeof(VERTEX);
	UINT offset = 0;
	Direct3D::pContext->IASetVertexBuffers(0, 1, &pVertexBuffer_, &stride, &offset);

	// �C���f�b�N�X�o�b�t�@�[���Z�b�g
	stride = sizeof(int);
	offset = 0;
	Direct3D::pContext->IASetIndexBuffer(pIndexBuffer_, DXGI_FORMAT_R32_UINT, 0);

	//�R���X�^���g�o�b�t�@
	Direct3D::pContext->VSSetConstantBuffers(0, 1, &pConstantBuffer_);	//���_�V�F�[�_�[�p
	Direct3D::pContext->PSSetConstantBuffers(0, 1, &pConstantBuffer_);	//�s�N�Z���V�F�[�_�[�p

	Direct3D::pContext->DrawIndexed(indexNum_, 0, 0);

	Direct3D::SetDepthBufferWriteEnable(true);
}

// ���_�f�[�^�p�o�b�t�@�̐ݒ�
HRESULT Sprite::CreateVertexBuffer()
{
	D3D11_BUFFER_DESC bd_vertex;
	bd_vertex.ByteWidth = sizeof(VERTEX) * vertexNum_;
	bd_vertex.Usage = D3D11_USAGE_DEFAULT;
	bd_vertex.BindFlags = D3D11_BIND_VERTEX_BUFFER;
	bd_vertex.CPUAccessFlags = 0;
	bd_vertex.MiscFlags = 0;
	bd_vertex.StructureByteStride = 0;
	D3D11_SUBRESOURCE_DATA data_vertex;
	data_vertex.pSysMem = vertices_;
	HRESULT hr = Direct3D::pDevice->CreateBuffer(&bd_vertex, &data_vertex, &pVertexBuffer_);
	if (FAILED(hr))
	{
		MessageBox(nullptr, L"���_�f�[�^�p�o�b�t�@�̐ݒ�Ɏ��s", L"�G���[", MB_OK);
		return hr;
	}

	return S_OK;
}

// �C���f�b�N�X�o�b�t�@�𐶐�����
HRESULT Sprite::CreateIndexBuffer()
{
	D3D11_BUFFER_DESC   bd;
	bd.Usage = D3D11_USAGE_DEFAULT;
	bd.ByteWidth = sizeof(int) * indexNum_;
	bd.BindFlags = D3D11_BIND_INDEX_BUFFER;
	bd.CPUAccessFlags = 0;
	bd.MiscFlags = 0;

	D3D11_SUBRESOURCE_DATA InitData;
	InitData.pSysMem = index_;
	InitData.SysMemPitch = 0;
	InitData.SysMemSlicePitch = 0;
	HRESULT hr = Direct3D::pDevice->CreateBuffer(&bd, &InitData, &pIndexBuffer_);
	if (FAILED(hr))
	{
		MessageBox(nullptr, L"�C���f�b�N�X�o�b�t�@�̍쐬�Ɏ��s", L"�G���[", MB_OK);
		return hr;
	}
	return S_OK;
}

//�R���X�^���g�o�b�t�@�쐬
HRESULT Sprite::CreateConstantBuffer()
{
	D3D11_BUFFER_DESC cb;
	cb.ByteWidth = sizeof(CONSTANT_BUFFER);
	cb.Usage = D3D11_USAGE_DYNAMIC;
	cb.BindFlags = D3D11_BIND_CONSTANT_BUFFER;
	cb.CPUAccessFlags = D3D11_CPU_ACCESS_WRITE;
	cb.MiscFlags = 0;
	cb.StructureByteStride = 0;

	// �R���X�^���g�o�b�t�@�̍쐬
	HRESULT hr = Direct3D::pDevice->CreateBuffer(&cb, nullptr, &pConstantBuffer_);
	if (FAILED(hr))
	{
		MessageBox(nullptr, L"�R���X�^���g�o�b�t�@�̍쐬�Ɏ��s", L"�G���[", MB_OK);
		return hr;
	}
	return S_OK;
}

HRESULT Sprite::Load(std::string fileName)
{
	hPict_ = TextureManager::Load(fileName.c_str());
	if (hPict_<0)
	{
		MessageBox(nullptr, L"�摜���[�h�Ɏ��s���܂���", L"�G���[", MB_OK);
		return E_FAIL;
	}
	size_ = { (float)TextureManager::GetTexture(hPict_)->GetWidth(),(float)TextureManager::GetTexture(hPict_)->GetHeight(),1.0f };
	
	InitVertex();
	InitIndex();
	if (FAILED(CreateVertexBuffer()))
	{
		return E_FAIL;
	}

	// �C���f�b�N�X�o�b�t�@�𐶐�����
	if (FAILED(CreateIndexBuffer()))
	{
		return E_FAIL;
	}

	//�R���X�^���g�o�b�t�@�쐬
	if (FAILED(CreateConstantBuffer()))
	{
		return E_FAIL;
	}
	return S_OK;
}

void Sprite::InitVertex()
{
	// ���_���
	VERTEX vertices[] =
	{
		//1
		XMVectorSet( -1.0f,  1.0f, 0.0f, 0.0f),	XMVectorSet(0.0f,0.0f,0.0f,0.0f),	// �l�p�`�̒��_�i����j
		XMVectorSet(  1.0f,  1.0f, 0.0f, 0.0f),	XMVectorSet(1.0f,0.0f,0.0f,0.0f),	// �l�p�`�̒��_�i�E��j
		XMVectorSet(  1.0f, -1.0f, 0.0f, 0.0f),	XMVectorSet(1.0f,1.0f,0.0f,0.0f),	// �l�p�`�̒��_�i�E���j
		XMVectorSet( -1.0f, -1.0f, 0.0f, 0.0f),	XMVectorSet(0.0f,1.0f,0.0f,0.0f),   // �l�p�`�̒��_�i�����j
	};
	vertexNum_ = sizeof(vertices) / sizeof(VERTEX);
	vertices_ = new VERTEX[vertexNum_];
	memcpy(vertices_, vertices, sizeof(vertices));
}

void Sprite::InitIndex()
{
	//�C���f�b�N�X���
	int index[] = { 0,1,2, 0,2,3};

	indexNum_ = sizeof(index) / sizeof(int);
	index_ = new int[indexNum_];
	memcpy(index_, index, sizeof(index));
}

//�R���X�^���g�o�b�t�@�ɏ���n��
//void Sprite::ToPipeLine(Transform& transform)
//{
//	XMMATRIX matImageSize = XMMatrixScaling((float)(size_.x / Direct3D::GetScreenWidth()), (float)(size_.y / Direct3D::GetScreenHeight()), 1.0f);
//	CONSTANT_BUFFER cb;
//	cb.matPosition = XMMatrixTranspose(transform.GetWorldScaleMatrix()*matImageSize*transform.GetWorldRotateMatrix()*transform.GetWorldTranslateMatrix());
//	D3D11_MAPPED_SUBRESOURCE pdata;
//	Direct3D::pContext->Map(pConstantBuffer_, 0, D3D11_MAP_WRITE_DISCARD, 0, &pdata);	// GPU����̃f�[�^�A�N�Z�X���~�߂�
//	memcpy_s(pdata.pData, pdata.RowPitch, (void*)(&cb), sizeof(cb));					// �f�[�^��l�𑗂�
//
//	ID3D11SamplerState* pSampler = TextureManager::GetTexture(hPict_)->GetSampler();
//	Direct3D::pContext->PSSetSamplers(0, 1, &pSampler);
//	ID3D11ShaderResourceView* pSRV = TextureManager::GetTexture(hPict_)->GetSRV();
//	Direct3D::pContext->PSSetShaderResources(0, 1, &pSRV);
//
//	Direct3D::pContext->Unmap(pConstantBuffer_, 0);//�ĊJ
//}
//
////���_�A�C���f�b�N�X�A�R���X�^���g�o�b�t�@���Z�b�g
//void Sprite::bufferSet()
//{
//	//���_�o�b�t�@
//	UINT stride = sizeof(VERTEX);
//	UINT offset = 0;
//	Direct3D::pContext->IASetVertexBuffers(0, 1, &pVertexBuffer_, &stride, &offset);
//
//	// �C���f�b�N�X�o�b�t�@�[���Z�b�g
//	stride = sizeof(int);
//	offset = 0;
//	Direct3D::pContext->IASetIndexBuffer(pIndexBuffer_, DXGI_FORMAT_R32_UINT, 0);
//
//	//�R���X�^���g�o�b�t�@
//	Direct3D::pContext->VSSetConstantBuffers(0, 1, &pConstantBuffer_);	//���_�V�F�[�_�[�p
//	Direct3D::pContext->PSSetConstantBuffers(0, 1, &pConstantBuffer_);	//�s�N�Z���V�F�[�_�[�p
//
//	Direct3D::pContext->DrawIndexed(indexNum_, 0, 0);
//}

void Sprite::Release()
{
	SAFE_DELETE_ARRAY(index_);
	SAFE_DELETE_ARRAY(vertices_);
	SAFE_RELEASE(pConstantBuffer_);
	SAFE_RELEASE(pIndexBuffer_);
	SAFE_RELEASE(pVertexBuffer_);
}