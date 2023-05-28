#include "BillBoard.h"
#include"../GameObject/Camera.h"
#include"../ResourceManager/TextureManager.h"
BillBoard::BillBoard()
	:hTexture_(-1)
{
	pConstantBuffer_ = nullptr;
	pIndexBuffer_ = nullptr;
	pVertexBuffer_ = nullptr;
}

BillBoard::~BillBoard()
{
	Release();
}

HRESULT BillBoard::Load(std::string fileName)
{
	HRESULT hr;

	VERTEX vertices[] = {
							{XMFLOAT3(-0.5,0.5,0),	XMFLOAT3(0,0,0)},//����
							{XMFLOAT3(0.5,0.5,0),	XMFLOAT3(1,0,0)},//�E��
							{XMFLOAT3(0.5,-0.5,0),	XMFLOAT3(1,1,0)},//�E��
							{XMFLOAT3(-0.5,-0.5,0),	XMFLOAT3(0,1,0)},//����
						};

	int index[]={ 0,1,2, 0,2,3 };

	
	///////////////////���_�o�b�t�@�쐬////////////////////
	D3D11_BUFFER_DESC bd_vertex;
	bd_vertex.ByteWidth = sizeof(vertices);
	bd_vertex.Usage = D3D11_USAGE_DEFAULT;
	bd_vertex.BindFlags = D3D11_BIND_VERTEX_BUFFER;
	bd_vertex.CPUAccessFlags = 0;
	bd_vertex.MiscFlags = 0;
	bd_vertex.StructureByteStride = 0;
	D3D11_SUBRESOURCE_DATA data_vertex;
	data_vertex.pSysMem = vertices;

	hr=Direct3D::pDevice->CreateBuffer(&bd_vertex, &data_vertex, &pVertexBuffer_);
	if (FALSE(hr))
	{
		MessageBox(nullptr, L"�r���{�[�h���_�o�b�t�@�̍쐬�Ɏ��s", L"�G���[", MB_OK);
		return hr;
	}
	
	
	////////////////�R���X�^���g�o�b�t�@�쐬///////////////
	D3D11_BUFFER_DESC cb;
	cb.ByteWidth = sizeof(CONSTANT_BUFFER);
	cb.Usage = D3D11_USAGE_DYNAMIC;
	cb.BindFlags = D3D11_BIND_CONSTANT_BUFFER;
	cb.CPUAccessFlags = D3D11_CPU_ACCESS_WRITE;
	cb.MiscFlags = 0;
	cb.StructureByteStride = 0;

	hr=Direct3D::pDevice->CreateBuffer(&cb, nullptr, &pConstantBuffer_);
	if (FALSE(hr))
	{
		MessageBox(nullptr, L"�r���{�[�h�R���X�^���g�o�b�t�@�̍쐬�Ɏ��s", L"�G���[", MB_OK);
		return hr;
	}

	////////////////�C���f�b�N�X�o�b�t�@//////////////////
	D3D11_BUFFER_DESC bd_index;
	bd_index.ByteWidth = sizeof(index);
	bd_index.Usage = D3D11_USAGE_DEFAULT;
	bd_index.BindFlags = D3D11_BIND_INDEX_BUFFER;
	bd_index.CPUAccessFlags = 0;
	bd_index.MiscFlags = 0;
	

	D3D11_SUBRESOURCE_DATA initData;
	initData.pSysMem = index;
	initData.SysMemPitch = 0;
	initData.SysMemSlicePitch = 0;

	hr = Direct3D::pDevice->CreateBuffer(&bd_vertex, &initData, &pIndexBuffer_);
	if (FALSE(hr))
	{
		MessageBox(nullptr, L"�r���{�[�h�C���f�b�N�X�o�b�t�@�̍쐬�Ɏ��s", L"�G���[", MB_OK);
		return hr;
	}
	hTexture_ = TextureManager::Load(fileName);

	if (hTexture_<0)
	{
		MessageBox(nullptr, L"�r���{�[�h�e�N�X�`���̃��[�h�Ɏ��s", L"�G���[", MB_OK);
		return hr;
	}
	return hr;
}

void BillBoard::Draw(XMMATRIX matW, XMFLOAT4 col)
{
	Direct3D::SetBlendMode(BLEND_MODE::BLEND_ADD);
	Direct3D::SetShader(SHADER_TYPE::SHADER_EFF);

	CONSTANT_BUFFER cb;
	cb.matWVP = XMMatrixTranspose(matW * Camera::GetViewMatrix() * Camera::GetProjectionMatrix());
	cb.color = col;

	D3D11_MAPPED_SUBRESOURCE pdata;
	Direct3D::pContext->Map(pConstantBuffer_, 0, D3D11_MAP_WRITE_DISCARD, 0, &pdata);			//GPU����̃f�[�^�A�N�Z�X���~�߂�
	memcpy_s(pdata.pData, pdata.RowPitch, (void*)(&cb), sizeof(cb));							//�f�[�^��l�𑗂�
	
		ID3D11SamplerState* pSampler = TextureManager::GetTexture(hTexture_)->GetSampler();
		Direct3D::pContext->PSSetSamplers(0, 1, &pSampler);

		ID3D11ShaderResourceView* pSRV1 = TextureManager::GetTexture(hTexture_)->GetSRV();
		Direct3D::pContext->PSSetShaderResources(0, 1, &pSRV1);

	Direct3D::pContext->Unmap(pConstantBuffer_, 0);//�ĊJ
	
	//���_�o�b�t�@
	UINT stride = sizeof(VERTEX);
	UINT offset = 0;
	Direct3D::pContext->IASetVertexBuffers(0, 1, &pVertexBuffer_, &stride, &offset);

	
	// �C���f�b�N�X�o�b�t�@�[���Z�b�g
	stride = sizeof(int);
	offset = 0;
	Direct3D::pContext->IASetIndexBuffer(pIndexBuffer_, DXGI_FORMAT_R32_UINT, 0);

	//�R���X�^���g�o�b�t�@
	Direct3D::pContext->VSSetConstantBuffers(0, 1, &pConstantBuffer_);							//���_�V�F�[�_�[�p	
	Direct3D::pContext->PSSetConstantBuffers(0, 1, &pConstantBuffer_);							//�s�N�Z���V�F�[�_�[�p

	Direct3D::pContext->DrawIndexed(6, 0, 0);
}

void BillBoard::Release()
{
	SAFE_RELEASE(pIndexBuffer_);
	SAFE_RELEASE(pConstantBuffer_);
	SAFE_RELEASE(pVertexBuffer_);

	SAFE_DELETE(pIndexBuffer_);
	SAFE_DELETE(pConstantBuffer_);
	SAFE_DELETE(pVertexBuffer_);

}