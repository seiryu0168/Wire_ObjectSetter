#include "LineParticle.h"
#include"Direct3D.h"
#include"DirectXMath.h"
#include"../GameObject/Camera.h"
#include<vector>
LineParticle::LineParticle()
	:WIDTH_(0.5),
	LENGTH_(2),
	tipWidth_(0),
	color_(1,1,1,1),
	pVertexBuffer_(nullptr),
	pIndexBuffer_(nullptr),
	pConstantBuffer_(nullptr),
	pTexture_(nullptr)
{
	
}
LineParticle::LineParticle(float width, int length, float tipWidth)
	:WIDTH_(width),
	LENGTH_(length),
	tipWidth_(tipWidth),
	color_({1,1,1,1}),
	pVertexBuffer_(nullptr),
	pIndexBuffer_(nullptr),
	pConstantBuffer_(nullptr),
	pTexture_(nullptr)
{
	assert(LENGTH_ >= 1);
	tipWidth_ = max(tipWidth_, 0);
}

//���݈ʒu���L�� : pos
void LineParticle::AddPosition(XMFLOAT3 pos)
{
	//���X�g�̐�[�Ɍ��݈ʒu������
	positionList_.push_front(pos);
	
	//LENGTH�����Ă���I�[��position���폜
	if (positionList_.size() > (size_t)LENGTH_+2)
	{
		positionList_.pop_back();
	}

	//���_�o�b�t�@���
	SAFE_RELEASE(pVertexBuffer_);
	SAFE_RELEASE(pIndexBuffer_);
	indexList.clear();
	SetIndex();

	//�J�����̈ʒu�擾(�x�N�g����)
	XMFLOAT3 camPos = Camera::GetPosition();
	XMVECTOR vCamPos = XMLoadFloat3(&camPos);

	CreateMeshPype(&positionList_);
}

HRESULT LineParticle::CreateMeshPype(std::list<XMFLOAT3>* pList)
{
	//�J�����̈ʒu�擾(�x�N�g����)
	XMFLOAT3 camPos = Camera::GetPosition();
	XMVECTOR vCamPos = XMLoadFloat3(&camPos);


	//���_�f�[�^�쐬
	XMVECTOR upVec = XMVectorSet(0, 1, 0, 0);
	VERTEX* vertices = new VERTEX[pList->size()* 4];
	
		int index = 0;
		auto itr = pList->begin();
		for (UINT j = 0; j < LENGTH_+2; j++)
		{
			//�L�����Ă���ʒu�擾
			XMVECTOR vPos = XMLoadFloat3(&(*itr));

			itr++;
			if (itr == pList->end())
			{
				break;
			}

			//�������擾�����ʒu���玟�̈ʒu�Ɍ������x�N�g��
			XMVECTOR vLine = XMLoadFloat3(&(*itr)) - vPos;

			if (XMVectorGetX(XMVector3Length(vLine)) >= 0.01f)
			{
				//�p�[�e�B�N���̘r���񂷃N�I�[�^�j�I��
				XMVECTOR armRotate = XMQuaternionRotationAxis(vLine, (float)(M_PI / 2.0f));
				
				//�p�[�e�B�N���̘r�����
				XMVECTOR vArm = XMVector3Cross(vLine, vCamPos);
				
				//�����ɂ���đ����ς��
				vArm = XMVector3Normalize(vArm) * (float)(WIDTH_*(LENGTH_-j)/LENGTH_);
				
				//�r����]
				XMVECTOR vArm2 = XMVector3Rotate(vArm, armRotate);
				XMFLOAT3 pos;

				//�Ђ��`�ɂȂ�悤�ɒ��_��z�u
				XMStoreFloat3(&pos, vPos + -vArm);	
				VERTEX vertex0 = { pos,XMFLOAT3((float)j / LENGTH_,1.0f/4.0f,0) };

				XMStoreFloat3(&pos, vPos + vArm2);
				VERTEX vertex1 = { pos,XMFLOAT3((float)j / LENGTH_,2.0f / 4.0f,0) };

				XMStoreFloat3(&pos, vPos + vArm);
				VERTEX vertex2 = { pos,XMFLOAT3((float)j / LENGTH_,3.0f/4.0f,0) };

				XMStoreFloat3(&pos, vPos + -vArm2);
				VERTEX vertex3 = { pos,XMFLOAT3((float)j / LENGTH_,1,0) };
				
				vertices[index] = vertex0;
				index++;
				vertices[index] = vertex1;
				index++;
				vertices[index] = vertex2;
				index++;
				vertices[index] = vertex3;
				index++;
			} 
			else
			{
				XMFLOAT3 dumPos;
				XMStoreFloat3(&dumPos, vPos);
				VERTEX dummy = { dumPos,{0,0,0} };
				vertices[index] = dummy;
				index++;
				vertices[index] = dummy;
				index++;
				vertices[index] = dummy;
				index++;
				vertices[index] = dummy;
				index++;
			}
		}

		for (int i = 0; i < sizeof(*vertices) / sizeof(VERTEX); i++)
		{
			if (abs(vertices[i].position.x) <= 0.1f&& abs(vertices[i].position.y) <= 1.1f&& abs(vertices[i].position.z) <= 0.1f)
			{
				int a = 0;
			}
		}

	D3D11_BUFFER_DESC bd_vertex;
	bd_vertex.ByteWidth = sizeof(VERTEX) * pList->size()*4;
	bd_vertex.Usage = D3D11_USAGE_DEFAULT;
	bd_vertex.BindFlags = D3D11_BIND_VERTEX_BUFFER;
	bd_vertex.CPUAccessFlags = 0;
	bd_vertex.MiscFlags = 0;
	bd_vertex.StructureByteStride = 0;

	D3D11_SUBRESOURCE_DATA data_vertex;
	data_vertex.pSysMem = vertices;

	HRESULT hr = Direct3D::pDevice->CreateBuffer(&bd_vertex, &data_vertex, &pVertexBuffer_);
	if (FAILED(hr))
	{
		MessageBox(nullptr, L"���C���p�[�e�B�N���̃|�W�V�����X�V���s", L"�G���[", MB_OK);
		return hr;
	}
	hr = Direct3D::pDevice->GetDeviceRemovedReason();
	if (FAILED(hr))
	{
		MessageBox(nullptr, L"���C���p�[�e�B�N���̃|�W�V�����X�V���s", L"�G���[", MB_OK);
		return hr;
	}
	SAFE_DELETE_ARRAY(vertices);

	return hr;
}

HRESULT LineParticle::CreateMeshPlate(std::list<XMFLOAT3>* pList)
{
	//�J�����̈ʒu�擾(�x�N�g����)
	XMFLOAT3 camPos = Camera::GetPosition();
	XMVECTOR vCamPos = XMLoadFloat3(&camPos);
	int index = 0;


	auto itr = pList->begin();
	//���_�f�[�^�쐬
	VERTEX* vertices = new VERTEX[pList->size() * 2];
	for (UINT i = 0; i < LENGTH_+2; i++)
	{
		//�L�����Ă���ʒu�擾
		XMVECTOR vPos = XMLoadFloat3(&(*itr));

		itr++;
		if (itr == pList->end())
		{
			break;
		}

		//�������擾�����ʒu���玟�̈ʒu�Ɍ������x�N�g��
		XMVECTOR vLine = XMLoadFloat3(&(*itr)) - vPos;

		XMVECTOR vArm = XMVector3Cross(vLine, vCamPos);
		vArm = XMVector3Normalize(vArm) * WIDTH_;

			XMFLOAT3 pos;
			XMStoreFloat3(&pos, vPos + vArm);
			VERTEX vertex1 = { pos,XMFLOAT3((float)i / LENGTH_ + tipWidth_,0,0) };

			XMStoreFloat3(&pos, vPos - vArm);
			VERTEX vertex2 = { pos,XMFLOAT3((float)i / LENGTH_ + tipWidth_,1,0) };

			int s = sizeof(VERTEX);

			vertices[index] = vertex1;
			index++;
			vertices[index] = vertex2;
			index++;


	}
	//���_�o�b�t�@�p��
	D3D11_BUFFER_DESC bd_vertex;
	bd_vertex.ByteWidth = sizeof(VERTEX) * ((size_t)LENGTH_+2) * 4;
	bd_vertex.Usage = D3D11_USAGE_DEFAULT;
	bd_vertex.BindFlags = D3D11_BIND_VERTEX_BUFFER;
	bd_vertex.CPUAccessFlags = 0;
	bd_vertex.MiscFlags = 0;
	bd_vertex.StructureByteStride = 0;

	D3D11_SUBRESOURCE_DATA data_vertex;
	data_vertex.pSysMem = vertices;
	HRESULT hr = Direct3D::pDevice->CreateBuffer(&bd_vertex, &data_vertex, &pVertexBuffer_);//���܂ɗ�O�̃X���[���N����B�����s��
	if (FAILED(hr))
	{
		MessageBox(nullptr, L"���C���p�[�e�B�N���̃|�W�V�����X�V���s", L"�G���[", MB_OK);
		return hr;
	}
	hr = Direct3D::pDevice->GetDeviceRemovedReason();

	if (FAILED(hr))
	{
		MessageBox(nullptr, L"���C���p�[�e�B�N���̃|�W�V�����X�V���s", L"�G���[", MB_OK);
		return hr;
	}
	delete[] vertices;

	return hr;
}

HRESULT LineParticle::Load(std::string fileName)
{
	HRESULT hr;
	D3D11_BUFFER_DESC bd_constant;
	bd_constant.ByteWidth = sizeof(CONSTANT_BUFFER);
	bd_constant.Usage = D3D11_USAGE_DYNAMIC;
	bd_constant.BindFlags = D3D11_BIND_CONSTANT_BUFFER;
	bd_constant.CPUAccessFlags = D3D11_CPU_ACCESS_WRITE;
	bd_constant.MiscFlags = 0;
	bd_constant.StructureByteStride = 0;



	hr = Direct3D::pDevice->CreateBuffer(&bd_constant, nullptr, &pConstantBuffer_);
	if (FAILED(hr))
	{
		MessageBox(nullptr, L"���C���p�[�e�B�N���p�R���X�^���g�o�b�t�@�̍쐬�Ɏ��s", L"�G���[", MB_OK);
		return hr;
	}
	pTexture_ = new Texture();
	wchar_t name[FILENAME_MAX];
	size_t ret;
	mbstowcs_s(&ret, name, fileName.c_str(), fileName.length());

	hr = pTexture_->Load(name);
	if (FAILED(hr))
	{
		MessageBox(nullptr, L"���C���p�[�e�B�N���̃e�N�X�`���̃��[�h�Ɏ��s", L"�G���[", MB_OK);
		return hr;
	}

	SetIndex();
	return S_OK;
}

void LineParticle::SetColor(XMFLOAT4 col)
{
	color_ = col;
}

void LineParticle::SetIndex()
{
	//�C���f�b�N�X�̏����z��
	int fixedIndex[] = { 0,0,-3, 3,-1,1, 5,5,2,  8, 4, 6, 10,10, 7, 13,9, 11, 15,19,16, 18,18,16 };

	//�z����ꏄ�������ɑ�����l
	int indexOffset = 0;
	int indexdelta = 1;
	
	//�C���f�b�N�X����
	for (UINT i = 0; i < LENGTH_*3*8; i++)
	{
			indexList.push_back(i - (indexOffset + fixedIndex[i % 24]));
		if (i%(24*indexdelta-1) == 0&&i!=0)
		{
			indexOffset += 20;
			indexdelta++;
		}
	}

	//�C���f�b�N�X�̃f�[�^
	D3D11_BUFFER_DESC   bd;
	bd.Usage = D3D11_USAGE_DEFAULT;
	bd.ByteWidth = (int)(sizeof(int) * indexList.size());
	bd.BindFlags = D3D11_BIND_INDEX_BUFFER;
	bd.CPUAccessFlags = 0;
	bd.MiscFlags = 0;

	D3D11_SUBRESOURCE_DATA InitData;
	//�z�񕔕�����Ă���ɂ�.data()�B&vector[0]�Ɠ��l
	InitData.pSysMem = indexList.data();
	InitData.SysMemPitch = 0;
	InitData.SysMemSlicePitch = 0;

	//�C���f�b�N�X�o�b�t�@����
	HRESULT hr = Direct3D::pDevice->CreateBuffer(&bd, &InitData, &pIndexBuffer_);
	if (FAILED(hr))
	{
		MessageBox(nullptr, L"�C���f�b�N�X�o�b�t�@�̍쐬�Ɏ��s", L"�G���[", MB_OK);
		//return hr;
	}
	//return S_OK;
}

void LineParticle::Draw(Transform* transform)
{
	HRESULT hr;
	Direct3D::SetShader(SHADER_TYPE::SHADER_EFF);
	CONSTANT_BUFFER cb;
	cb.matWVP = XMMatrixTranspose(Camera::GetViewMatrix() * Camera::GetProjectionMatrix());
	cb.color = color_;

	D3D11_MAPPED_SUBRESOURCE pdata;

	//GPU����̃f�[�^�A�N�Z�X���~�߂�
	hr = Direct3D::pContext->Map(pConstantBuffer_, 0, D3D11_MAP_WRITE_DISCARD, 0, &pdata);
	if (FAILED(hr))
	{
		MessageBox(nullptr, L"�f�o�C�X�R���e�L�X�g���s", L"�G���[", MB_OK);
	}
	//�f�[�^�𑗂�
	memcpy_s(pdata.pData, pdata.RowPitch, (void*)(&cb), sizeof(cb));
	
	ID3D11SamplerState* pSampler = pTexture_->GetSampler();
	Direct3D::pContext->PSSetSamplers(0, 1, &pSampler);

	ID3D11ShaderResourceView* pSRV = pTexture_->GetSRV();
	Direct3D::pContext->PSSetShaderResources(0, 1, &pSRV);

	//�f�[�^�A�N�Z�X�ĊJ
	Direct3D::pContext->Unmap(pConstantBuffer_, 0);

	UINT stride = sizeof(VERTEX);
	UINT offset = 0;

	//���_�o�b�t�@
	Direct3D::pContext->IASetVertexBuffers(0, 1, &pVertexBuffer_, &stride, &offset);
	
	//�C���f�b�N�X�o�b�t�@
	Direct3D::pContext->IASetIndexBuffer(pIndexBuffer_, DXGI_FORMAT_R32_UINT, 0);
	
	//�R���X�^���g�o�b�t�@
	Direct3D::pContext->VSSetConstantBuffers(0, 1, &pConstantBuffer_);//���_�V�F�[�_�[�p
	Direct3D::pContext->PSSetConstantBuffers(0, 1, &pConstantBuffer_);//�s�N�Z���V�F�[�_�[�p

	//���_�̕��ѕ����w��
	Direct3D::pContext->DrawIndexed((UINT)indexList.size()-24,0,0);
	Direct3D::pContext->IASetPrimitiveTopology(D3D_PRIMITIVE_TOPOLOGY_TRIANGLELIST);
}

void LineParticle::SetLineParameter(float width, int length, float tipWidth)
{
	WIDTH_ = width;
	LENGTH_ = length;
	tipWidth_ = tipWidth;
	tipWidth_ = max(tipWidth_, 0);
	
}

void LineParticle::DeleteLine()
{
	positionList_.clear();
}

void LineParticle::Release()
{
	SAFE_RELEASE(pTexture_);
	SAFE_RELEASE(pConstantBuffer_);
	SAFE_RELEASE(pVertexBuffer_);
	positionList_.clear();
}
