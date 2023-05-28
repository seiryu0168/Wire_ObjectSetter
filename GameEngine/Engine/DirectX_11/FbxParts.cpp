#include "FbxParts.h"
#include"Math.h"
#include"../ResourceManager/Model.h"
#include"../GameObject/Camera.h"
#include"Direct3D.h"

FbxParts::FbxParts()
{
	boneNum_ = 0;
	materialCount_ = 0;
	polygonCount_ = 0;
	vertexCount_ = 0;

	indexCount_ = nullptr;
	pBoneArray_ = nullptr;
	pMaterialList_ = nullptr;
	pSkinInfo_ = nullptr;
	pVertices_ = nullptr;
	pWeightArray_ = nullptr;
	ppCluster_ = nullptr;
	ppIndex_ = nullptr;

	pVertexBuffer_ = nullptr;
	ppIndexBuffer_ = nullptr;
	pConstantBuffer_ = nullptr;
	pToonTexture_ = nullptr;

}

FbxParts::~FbxParts()
{
	SAFE_RELEASE(pToonTexture_);
	SAFE_DELETE(pVertices_);
	SAFE_RELEASE(pVertexBuffer_);

	for (int i = 0; i < materialCount_; i++)
	{
		SAFE_RELEASE(ppIndexBuffer_[i]);
		SAFE_DELETE(pMaterialList_[i].pTexture);
		SAFE_DELETE_ARRAY(ppIndex_);
	}
	SAFE_DELETE_ARRAY(ppIndexBuffer_);
	SAFE_RELEASE(pConstantBuffer_);

	//SAFE_DELETE(pSkinInfo_);
	SAFE_DELETE(ppCluster_);
	if (pWeightArray_ != nullptr)
	{
		for (int i = 0; i < vertexCount_; i++)
		{
			SAFE_DELETE_ARRAY(pWeightArray_);
			SAFE_DELETE_ARRAY(pBoneArray_);
		}
	}
	SAFE_DELETE(indexCount_);
}

HRESULT FbxParts::Init(FbxNode* pNode)
{
	FbxMesh* mesh = pNode->GetMesh();
	pToonTexture_ = new Texture;
	pToonTexture_->Load("../Image\\ToonTexture.jpg");
	
	//�e���̌����擾
	vertexCount_ = mesh->GetControlPointsCount();	//���_�̐�
	polygonCount_ = mesh->GetPolygonCount();	//�|���S���̐�
	materialCount_ = pNode->GetMaterialCount();//�}�e���A���̐�

	InitIndex(mesh);
	InitVertex(mesh);
	CreateConstantBuffer();
	InitMaterial(pNode);
	InitSkelton(mesh);
	return S_OK;
}

void FbxParts::Draw(Transform& transform,XMFLOAT4 lineColor)
{
	transform.Calclation();
	float factor[4] = { D3D11_BLEND_ZERO,D3D11_BLEND_ZERO, D3D11_BLEND_ZERO, D3D11_BLEND_ZERO };

	
	//�R���X�^���g�o�b�t�@�ɏ���n��
	for (int i = 0; i < materialCount_; i++)
	{
		CONSTANT_BUFFER cb;
		cb.matWVP = XMMatrixTranspose(transform.GetWorldMatrix() * Camera::GetViewMatrix() * Camera::GetProjectionMatrix());
		cb.matW = XMMatrixTranspose(transform.GetWorldMatrix());
		cb.matNormal = XMMatrixTranspose(transform.GetNormalMatrix());
		cb.lightDirection = XMFLOAT4(0, 1, 0, 0);
		cb.cameraPosition = XMFLOAT4(Camera::GetPosition().x, Camera::GetPosition().y, Camera::GetPosition().z, 0);

		cb.isTexture = pMaterialList_[i].pTexture != nullptr;
		cb.isNormal = pMaterialList_[i].pNormalMap != nullptr;
		cb.diffuseColor = pMaterialList_[i].diffuse;
		cb.ambient = pMaterialList_[i].ambient;
		cb.speculer = pMaterialList_[i].speculer;
		cb.shininess = pMaterialList_[i].shininess;
		cb.customColor = lineColor;
		XMVECTOR a = pVertices_[i].tangent;
		D3D11_MAPPED_SUBRESOURCE pdata;
		Direct3D::pContext->Map(pConstantBuffer_, 0, D3D11_MAP_WRITE_DISCARD, 0, &pdata); //GPU����̃f�[�^�A�N�Z�X���~�߂�
		memcpy_s(pdata.pData, pdata.RowPitch, (void*)(&cb), sizeof(cb));			      //�f�[�^��l�𑗂�
		if (cb.isTexture)
		{
			ID3D11SamplerState* pSampler = pMaterialList_[i].pTexture->GetSampler();
			Direct3D::pContext->PSSetSamplers(0, 1, &pSampler);
			ID3D11ShaderResourceView* pSRV1 = pMaterialList_[i].pTexture->GetSRV();

			Direct3D::pContext->PSSetShaderResources(0, 1, &pSRV1);
		}
		if (cb.isNormal)
		{
			ID3D11ShaderResourceView* pNormalSRV = pMaterialList_[i].pNormalMap->GetSRV();
			Direct3D::pContext->PSSetShaderResources(2, 1, &pNormalSRV);
		}
		Direct3D::pContext->Unmap(pConstantBuffer_, 0);//�ĊJ

		Direct3D::pContext->Map(pConstantBuffer_, 0, D3D11_MAP_WRITE_DISCARD, 0, &pdata); //GPU����̃f�[�^�A�N�Z�X���~�߂�
		memcpy_s(pdata.pData, pdata.RowPitch, (void*)(&cb), sizeof(cb));			      //�f�[�^��l�𑗂�
		
		ID3D11SamplerState* pToonSampler = pToonTexture_->GetSampler();
		Direct3D::pContext->PSSetSamplers(1, 1, &pToonSampler);
		ID3D11ShaderResourceView* pToonSRV = pToonTexture_->GetSRV();
		Direct3D::pContext->PSSetShaderResources(1, 1, &pToonSRV);
		Direct3D::pContext->Unmap(pConstantBuffer_, 0);//�ĊJ

		//���_�o�b�t�@
		UINT stride = sizeof(VERTEX);
		UINT offset = 0;
		Direct3D::pContext->IASetVertexBuffers(0, 1, &pVertexBuffer_, &stride, &offset);

		// �C���f�b�N�X�o�b�t�@�[���Z�b�g
		stride = sizeof(int);
		offset = 0;
		Direct3D::pContext->IASetIndexBuffer(ppIndexBuffer_[i], DXGI_FORMAT_R32_UINT, 0);

		//�R���X�^���g�o�b�t�@
		Direct3D::pContext->VSSetConstantBuffers(0, 1, &pConstantBuffer_);							//���_�V�F�[�_�[�p	
		Direct3D::pContext->PSSetConstantBuffers(0, 1, &pConstantBuffer_);							//�s�N�Z���V�F�[�_�[�p
		Direct3D::pContext->UpdateSubresource(pConstantBuffer_, 0, nullptr, &cb, 0, 0);
		Direct3D::pContext->DrawIndexed(indexCount_[i], 0, 0);
	}
}

void FbxParts::DrawSkinAnime(Transform& transform, FbxTime time, XMFLOAT4 lineColor)
{
	for (int i = 0; i < boneNum_; i++)
	{
		//�e�{�[���̌��݂̍s����擾
		FbxAnimEvaluator* evaluator = ppCluster_[i]->GetLink()->GetScene()->GetAnimationEvaluator();
		FbxMatrix mCurrentOrentation = evaluator->GetNodeGlobalTransform(ppCluster_[i]->GetLink(), time);

		//�s��R�s�[
		XMFLOAT4X4 pose;
		for (int x = 0; x < 4; x++)
		{
			for (int y = 0; y < 4; y++) 
			{
				pose(x, y) = (float)mCurrentOrentation.Get(x, y);
			}
		}

		//�I�t�Z�b�g���Ƃ̃|�[�Y�̍������v�Z
		pBoneArray_[i].newPose = XMLoadFloat4x4(&pose);
		pBoneArray_[i].diffPose = XMMatrixInverse(nullptr, pBoneArray_[i].bindPose);
		pBoneArray_[i].diffPose *= pBoneArray_[i].newPose;
	}
	//�e�{�[���ɑΉ����钸�_�̕ό`�𐧌�
	for (int i = 0; i < vertexCount_; i++)
	{
		//�e���_���Ƃɉe������{�[���~�E�F�C�g��ɉh�������s����쐬
		XMMATRIX matrix;
		ZeroMemory(&matrix, sizeof(matrix));
		for (int j = 0; j < boneNum_; j++)
		{
			if (pWeightArray_[i].pBoneIndex[j] < 0)
				break;
			matrix += pBoneArray_[pWeightArray_[i].pBoneIndex[j]].diffPose * pWeightArray_[i].pBoneWeight[j];
		}
		//�쐬�����s��Œ��_��ό`
		XMVECTOR pos = pWeightArray_[i].originPos;
		XMVECTOR normal = pWeightArray_[i].originNormal;
		pVertices_[i].position = XMVector3TransformCoord(pos, matrix);
		pVertices_[i].normal = XMVector3TransformCoord(normal, matrix);
	}

	//���_�o�b�t�@�����b�N���A�ό`��̒��_���ŏ㏑��
	D3D11_MAPPED_SUBRESOURCE msr = {};
	Direct3D::pContext->Map(pVertexBuffer_, 0, D3D11_MAP_WRITE_DISCARD, 0, &msr);
	if (msr.pData)
	{
		memcpy_s(msr.pData, msr.RowPitch, pVertices_, sizeof(VERTEX) * vertexCount_);
		Direct3D::pContext->Unmap(pVertexBuffer_, 0);
	}
	Draw(transform,lineColor);
}


HRESULT FbxParts::InitVertex(fbxsdk::FbxMesh* mesh)
{
	//���_��������z��
	pVertices_ = new VERTEX[vertexCount_];

	//�S�|���S��
	for (DWORD poly = 0; poly < (DWORD)polygonCount_; poly++)
	{
		//3���_��
		for (int vertex = 0; vertex < 3; vertex++)
		{
			//���ׂ钸�_�̔ԍ�
			int index = mesh->GetPolygonVertex(poly, vertex);

			//���_�̈ʒu
			FbxVector4 pos = mesh->GetControlPointAt(index);
			pVertices_[index].position = XMVectorSet((float)pos[0], (float)pos[1], (float)pos[2], 0.0f);

			//���_��UV
			FbxLayerElementUV* pUV = mesh->GetLayer(0)->GetUVs();
			int uvIndex = mesh->GetTextureUVIndex(poly, vertex, FbxLayerElement::eTextureDiffuse);
			FbxVector2  uv = pUV->GetDirectArray().GetAt(uvIndex);
			pVertices_[index].uv = XMVectorSet((float)uv.mData[0], (float)(1.0f - uv.mData[1]), 0.0f, 0.0f);

			//���_�̖@��
			FbxVector4 Normal;
			mesh->GetPolygonVertexNormal(poly, vertex, Normal);	//���Ԗڂ̃|���S���́A���Ԗڂ̒��_�̖@�����Q�b�g
			pVertices_[index].normal = XMVectorSet((float)Normal[0], (float)Normal[1], (float)Normal[2], 0.0f);
		}
			int index0 = mesh->GetPolygonVertex(poly, 0);
			int index1 = mesh->GetPolygonVertex(poly, 1);
			int index2 = mesh->GetPolygonVertex(poly, 2);

		if (mesh->GetElementTangentCount() > 0)
		{
			int cnt = mesh->GetElementTangentCount();
			for (int vertex = 0; vertex < 3; vertex++)
			{
				int index = mesh->GetPolygonVertex(poly, vertex);
				//�ڐ�
				FbxGeometryElementTangent* t = mesh->GetElementTangent(0);
				FbxVector4 tangent = t->GetDirectArray().GetAt(index).mData;
				pVertices_[index].tangent = XMVectorSet((float)tangent[0], (float)tangent[1], (float)tangent[2], 0.0f);

			}
		}
		else
		{
			CalcTangent(pVertices_[index0], pVertices_[index1], pVertices_[index2]);
			CalcTangent(pVertices_[index1], pVertices_[index2], pVertices_[index0]);
			CalcTangent(pVertices_[index2], pVertices_[index0], pVertices_[index1]);
		}

#if 0
			if (mesh->GetElementTangentCount() > 0)
			{
				int cnt = mesh->GetElementTangentCount();
				for (int vertex = 0; vertex < 3; vertex++)
				{
					int index = mesh->GetPolygonVertex(poly, vertex);
					//�ڐ�
					FbxGeometryElementTangent* t = mesh->GetElementTangent(0);
					FbxVector4 tangent = t->GetDirectArray().GetAt(index).mData;
					pVertices_[index].tangent = XMVectorSet((float)tangent[0], (float)tangent[1], (float)tangent[2], 0.0f);

				}
			}
		else
		{
			//for (int vertex = 0; vertex < 3; vertex++)
			//{
			//	int index = mesh->GetPolygonVertex(poly, vertex);
			//	pVertices_[index].tangent = XMVectorSet(0.0f, 0.0f, 0.0f, 0.0f);
			//}
		}
#endif
	}
	FbxLayerElementUV* pUV = mesh->GetLayer(0)->GetUVs();
	int UVNum = pUV->GetDirectArray().GetCount();
	int UVCount = mesh->GetTextureUVCount();
	int indexNum = pUV->GetIndexArray().GetCount();

	//��̒��_�ɕ�����UV���W�����蓖�Ă��Ă���\��������̂ł�����ӂǂ��ɂ����Ȃ��Ƃ����Ȃ�
	if (vertexCount_ <= UVCount)
	{
		UVCount = vertexCount_;
	}
	if (pUV->GetMappingMode() == FbxLayerElement::eByControlPoint)
	{
		for (int i = 0; i < UVCount; i++)
		{
			FbxVector2  uv = pUV->GetDirectArray().GetAt(i);
			pVertices_[i].uv = XMVectorSet((float)uv.mData[0], 1.0f-(float)(uv.mData[1]), 0.0f, 0.0f);;
		}
	}
	if (pUV->GetMappingMode() == FbxLayerElement::eByPolygonVertex)
	{
		if (((FbxLayerElement*)pUV)->GetReferenceMode() == FbxLayerElement::eIndexToDirect)
		{
			for (int i = 0; i < UVCount; i++)
			{
				int ind= pUV->GetIndexArray().GetAt(i);
				FbxVector2  uv = pUV->GetDirectArray().GetAt(ind);
				XMVECTOR UV = XMVectorZero();
				UV= XMVectorSet((float)uv.mData[0], 1.0f-(float)(uv.mData[1]), 0.0f, 0.0f);
				UV.m128_i8[0] = 0;
				UV.m128_u8[0] = 0;
				pVertices_[i].uv = UV;
				if (sizeof(UV.m128_i8) > sizeof(pVertices_[i].uv.m128_i8))
				{
					int a = 0;
				}
			}
		}
	}

	D3D11_BUFFER_DESC bd_vertex;
	bd_vertex.ByteWidth = sizeof(VERTEX) * vertexCount_;
	bd_vertex.Usage = D3D11_USAGE_DEFAULT;
	bd_vertex.BindFlags = D3D11_BIND_VERTEX_BUFFER;
	bd_vertex.CPUAccessFlags = 0;
	bd_vertex.MiscFlags = 0;
	bd_vertex.StructureByteStride = 0;
	D3D11_SUBRESOURCE_DATA data_vertex;
	data_vertex.pSysMem = pVertices_;
	HRESULT hr = Direct3D::pDevice->CreateBuffer(&bd_vertex, &data_vertex, &pVertexBuffer_);
	if (FAILED(hr))
	{
		MessageBox(nullptr, L"���_�f�[�^�p�o�b�t�@�̐ݒ�Ɏ��s", L"�G���[", MB_OK);
		return hr;
	}

	return S_OK;
}

/////////////////////////////�C���f�b�N�X///////////////////////////////////
HRESULT FbxParts::InitIndex(fbxsdk::FbxMesh* mesh)
{
	ppIndex_ = new int* [materialCount_];
	ppIndexBuffer_ = new ID3D11Buffer * [materialCount_];
	indexCount_ = new int[materialCount_];

	for (int i = 0; i < materialCount_; i++)
	{
		int count = 0;
		ppIndex_[i] = new int[polygonCount_ * 3];
		//�S�|���S��
		for (DWORD poly = 0; poly < (DWORD)polygonCount_; poly++)
		{
			FbxLayerElementMaterial* mtl = mesh->GetLayer(0)->GetMaterials();
			int mtlId = mtl->GetIndexArray().GetAt(poly);

			if (mtlId == i)
			{
				//3���_��
				for (DWORD vertex = 0; vertex < 3; vertex++)
				{
					ppIndex_[i][count] = mesh->GetPolygonVertex(poly, vertex);
					count++;
				}
			}
		}
		indexCount_[i] = count;

		D3D11_BUFFER_DESC   bd;
		bd.Usage = D3D11_USAGE_DEFAULT;
		bd.ByteWidth = sizeof(int) * polygonCount_ * 3;
		bd.BindFlags = D3D11_BIND_INDEX_BUFFER;
		bd.CPUAccessFlags = 0;
		bd.MiscFlags = 0;

		D3D11_SUBRESOURCE_DATA InitData;
		InitData.pSysMem = ppIndex_[i];
		InitData.SysMemPitch = 0;
		InitData.SysMemSlicePitch = 0;
		HRESULT hr = Direct3D::pDevice->CreateBuffer(&bd, &InitData, &ppIndexBuffer_[i]);
		if (FAILED(hr))
		{
			MessageBox(nullptr, L"�C���f�b�N�X�o�b�t�@�̍쐬�Ɏ��s", L"�G���[", MB_OK);
			return hr;
		}
	}
	return S_OK;
}

//////////////////////////////////�R���X�^���g�o�b�t�@�쐬///////////////////////////////////	
HRESULT FbxParts::CreateConstantBuffer()
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

HRESULT FbxParts::InitSkelton(FbxMesh* pMesh)
{
	FbxDeformer* pDeformer = pMesh->GetDeformer(0);
	if (pDeformer == nullptr)
	{
		return S_OK;
	}
	pSkinInfo_ = (FbxSkin*)pDeformer;

	struct POLY_INDEX
	{
		int* polyIndex;
		int* vertexIndex;
		int refNum;
	};

	POLY_INDEX* polyTable = new POLY_INDEX[vertexCount_];
	for (int i = 0; i < vertexCount_; i++)
	{
		polyTable[i].polyIndex = new int[polygonCount_ * 3];
		polyTable[i].vertexIndex = new int[polygonCount_ * 3];
		polyTable[i].refNum = 0;
		ZeroMemory(polyTable[i].polyIndex, sizeof(int)* polygonCount_ * 3);
		ZeroMemory(polyTable[i].vertexIndex, sizeof(int)* polygonCount_ * 3);

		for (int j = 0; j < polygonCount_; j++)
		{
			for (int k = 0; k < 3; k++)
			{
				if (pMesh->GetPolygonVertex(j, k) == i)
				{
					polyTable[i].polyIndex[polyTable[i].refNum] = j;
					polyTable[i].vertexIndex[polyTable[i].refNum] = k;
					polyTable[i].refNum++;
				}
			}
		}
	}

	//�{�[�����擾
	boneNum_ = pSkinInfo_->GetClusterCount();
	ppCluster_ = new FbxCluster * [boneNum_];
	for (int i = 0; i < boneNum_; i++)
	{
		ppCluster_[i] = pSkinInfo_->GetCluster(i);
	}

	//�{�[���̐��ɍ��킹�ăE�F�C�g��������
	pWeightArray_ = new FbxParts::WEIGHT[vertexCount_];
	for (int i = 0; i < vertexCount_; i++)
	{
		pWeightArray_[i].originPos = pVertices_[i].position;
		pWeightArray_[i].originNormal = pVertices_[i].normal;
		pWeightArray_[i].pBoneIndex = new int[boneNum_];
		pWeightArray_[i].pBoneWeight = new float[boneNum_];
		for (int j = 0; j < boneNum_; j++)
		{
			pWeightArray_[i].pBoneIndex[j] = -1;
			pWeightArray_[i].pBoneWeight[j] = 0.0f;
		}
	}

	//�e�{�[������e�����󂯂钸�_�𒲂ׁA���_�x�[�X��boneIndex�AWeight�𐮓ڂ���
	for (int i = 0; i < boneNum_; i++)
	{
		int indexNum = ppCluster_[i]->GetControlPointIndicesCount();
		int* piIndex = ppCluster_[i]->GetControlPointIndices();
		double* pdWeight = ppCluster_[i]->GetControlPointWeights();
		
		//���_�Ɋ֘A�t����ꂽ�E�F�C�g���5�ȏ�̏ꍇ�͏d�݂̑傫������4�{�ɍi��
		for (int j = 0; j < indexNum; j++)
		{
			for (int k = 0; k < 4; k++)
			{
				if (k >= boneNum_)
					break;
				if (pdWeight[j] > pWeightArray_[piIndex[j]].pBoneWeight[k])
				{
					for (int l = boneNum_ - 1; l > k; l--)
					{
						pWeightArray_[piIndex[j]].pBoneIndex[l] = pWeightArray_[piIndex[j]].pBoneIndex[l - 1];
						pWeightArray_[piIndex[j]].pBoneWeight[l] = pWeightArray_[piIndex[j]].pBoneWeight[l - 1];
					}
					pWeightArray_[piIndex[j]].pBoneIndex[k] = i;
					pWeightArray_[piIndex[j]].pBoneWeight[k] = (float)pdWeight[j];
				}
			}
		}
	}
	
	//�{�[���̐���
	pBoneArray_ = new FbxParts::BONE[boneNum_];
	for (int i = 0; i < boneNum_; i++)
	{
		//�{�[���̃f�t�H���g�ʒu���擾
		FbxAMatrix matrix;
		ppCluster_[i]->GetTransformLinkMatrix(matrix);

		//�s����R�s�[
		XMFLOAT4X4 pose;
		for (int x = 0; x < 4; x++)
		{
			for (int y = 0; y < 4; y++)
			{
				pose(x, y) = (float)matrix.Get(x, y);
			}
		}
	}

	//�ꎞ�I�Ɏ���Ă������������J��
	for (int i = 0; i < vertexCount_; i++)
	{
		SAFE_DELETE_ARRAY(polyTable[i].polyIndex);
		SAFE_DELETE_ARRAY(polyTable[i].vertexIndex);
	}
	SAFE_DELETE_ARRAY(polyTable);
	return E_NOTIMPL;
}

void FbxParts::InitMaterial(fbxsdk::FbxNode* pNode)
{
	pMaterialList_ = new MATERIAL[materialCount_];

	for (int i = 0; i < materialCount_; i++)
	{
		//i�Ԗڂ̃}�e���A�������擾
		FbxSurfaceMaterial* pMaterial = pNode->GetMaterial(i);
		//�t�H���V�F�[�f�B���O�ׂ̈Ƀ}�e���A���������o��
		FbxSurfacePhong* pPhong = (FbxSurfacePhong*)pMaterial;

		//�e�N�X�`�����
		FbxProperty  lProperty = pMaterial->FindProperty(FbxSurfaceMaterial::sDiffuse);

		//�󂯎��p�̊e�}�e���A���p�����[�^�̏�����
		FbxDouble3 diffuse = FbxDouble3(0, 0, 0);
		FbxDouble3 ambient = FbxDouble3(0, 0, 0);
		FbxDouble3 speculer = FbxDouble3(0, 0, 0);

		diffuse = pPhong->Diffuse;
		ambient = pPhong->Ambient;
		//diffuse��ambient������
		pMaterialList_[i].diffuse = XMFLOAT4((float)diffuse[0], (float)diffuse[1], (float)diffuse[2], 1.0f);
		pMaterialList_[i].ambient = XMFLOAT4((float)ambient[0], (float)ambient[1], (float)ambient[2], 1.0f);
		pMaterialList_[i].speculer = XMFLOAT4(0, 0, 0, 0);
		pMaterialList_[i].shininess = 0;

		//Phong�̃N���X�Ȃ̂�Lambert�̃N���X�Ȃ̂��𔻕�(Lambert���ƃX�y�L�����[�̒l�������Ȃ�)
		if (pPhong->GetClassId().Is(FbxSurfacePhong::ClassId))
		{
			speculer = pPhong->Specular;
			pMaterialList_[i].speculer = XMFLOAT4((float)speculer[0], (float)speculer[1], (float)speculer[2], 1.0f);
			//blender����1-�e���̒lx100�ɂ�����
			pMaterialList_[i].shininess = (float)pPhong->Shininess;
		}

		//�e�N�X�`���̖���
		//pMaterialList_[i].pTexture = lProperty.GetSrcObjectCount<FbxFileTexture>();
		int count = lProperty.GetSrcObjectCount<FbxFileTexture>();
		if (lProperty.GetSrcObjectCount<FbxFileTexture>() > 0)
		{
			FbxFileTexture* textureInfo = lProperty.GetSrcObject<FbxFileTexture>(0);
			const char* textureFilePath = textureInfo->GetRelativeFileName();

			//�p�X�����t�@�C�����Ɗg���q�����ɂ���
			char name[_MAX_FNAME];	//�t�@�C����
			char ext[_MAX_EXT];		//�g���q
			_splitpath_s(textureFilePath, nullptr, 0, nullptr, 0, name, _MAX_FNAME, ext, _MAX_EXT);
			sprintf_s(name, "%s%s", name, ext);

			wchar_t wtext[FILENAME_MAX];
			size_t ret;
			mbstowcs_s(&ret, wtext, name, strlen(name));

			std::wstring fName = wtext;
			fName = L"../Image\\" + fName;
			//�t�@�C������e�N�X�`���쐬
			pMaterialList_[i].pTexture = new Texture;
			pMaterialList_[i].pTexture->Load(fName.c_str());
		}

		//�e�N�X�`������
		else
		{
			pMaterialList_[i].pTexture = nullptr;
			//�}�e���A���̐F
			FbxSurfaceLambert* pMaterial = (FbxSurfaceLambert*)pNode->GetMaterial(i);
			FbxDouble3  diffuse = pMaterial->Diffuse;
			pMaterialList_[i].diffuse = XMFLOAT4((float)diffuse[0], (float)diffuse[1], (float)diffuse[2], 1.0f);
		}

		/////////////////////////////////�m�[�}���}�b�v//////////////////////////
		{
			FbxProperty IPropaty = pMaterial->FindProperty(FbxSurfaceMaterial::sBump);
			int normalMapCount = IPropaty.GetSrcObjectCount<FbxFileTexture>();

			pMaterialList_[i].pNormalMap = new Texture;
			if (normalMapCount != 0)
			{
				FbxFileTexture* textureInfo = IPropaty.GetSrcObject<FbxFileTexture>(0);
				const char* textureFilePath = textureInfo->GetRelativeFileName();

				//�t�@�C����+�g�������ɂ���
				char name[_MAX_FNAME];	//�t�@�C����
				char ext[_MAX_EXT];	//�g���q
				_splitpath_s(textureFilePath, nullptr, 0, nullptr, 0, name, _MAX_FNAME, ext, _MAX_EXT);

				sprintf_s(name, "%s%s", name, ext);

				//�t�@�C������e�N�X�`���쐬
				wchar_t wtext[FILENAME_MAX];
				size_t ret;
				mbstowcs_s(&ret, wtext, name, strlen(name));

				pMaterialList_[i].pNormalMap->Load(wtext);
			}
			else
			{
				pMaterialList_[i].pNormalMap->Load(L"../Image\\DefaultNormalMap.jpg");
			}
		}
	}
}

void FbxParts::CalcTangent(VERTEX& vertex0, const VERTEX& vertex1, const VERTEX& vertex2)
{
	//3���_�̕K�v�ȏ��
	XMFLOAT3 localPos[3] = { StoreFloat3(vertex0.position),
							 StoreFloat3(vertex1.position),
							 StoreFloat3(vertex2.position) };

	XMFLOAT3 uv[3] =	   { StoreFloat3(vertex0.uv),	
							 StoreFloat3(vertex1.uv),
							 StoreFloat3(vertex2.uv) };
	XMVECTOR vPos0[3] = {
		XMVectorSet(localPos[0].x,uv[0].x,uv[0].y,0.0f),
		XMVectorSet(localPos[0].y,uv[0].x,uv[0].y,0.0f),
		XMVectorSet(localPos[0].z,uv[0].x,uv[0].y,0.0f)
	};
	XMVECTOR vPos1[3] = {
		XMVectorSet(localPos[1].x,uv[1].x,uv[1].y,0.0f),
		XMVectorSet(localPos[1].y,uv[1].x,uv[1].y,0.0f),
		XMVectorSet(localPos[1].z,uv[1].x,uv[1].y,0.0f)
	};
	XMVECTOR vPos2[3] = {
		XMVectorSet(localPos[2].x,uv[2].x,uv[2].y,0.0f),
		XMVectorSet(localPos[2].y,uv[2].x,uv[2].y,0.0f),
		XMVectorSet(localPos[2].z,uv[2].x,uv[2].y,0.0f)
	};

	float u[3];
	float v[3];
	for (int i = 0; i < 3; i++)
	{
		XMVECTOR V1 = vPos1[i] - vPos0[i];
		XMVECTOR V2 = vPos2[i] - vPos1[i];
		XMFLOAT3 normal;

		normal=StoreFloat3(XMVector3Normalize(XMVector3Cross(V1, V2)));
		bool degnerate = max(normal.x, 0.0f);

		if (normal.x==0.0f)
		{
			normal.x = 1.0f;
			//���_���W��UV���W�����S�ɏd�Ȃ��Ă���̂ŏk�ނ��Ă���
			//�v�Z�����藧���Ȃ�
			//assert(false);
			//XMVECTOR tangent = vertex1.position - vertex0.position;
			//tangent = tangent - vertex0.normal * VectorDot(tangent, vertex0.normal);
			//vertex0.tangent = XMVector3Normalize(tangent);
			//return;
		}

		u[i] = -normal.y / normal.x;
		v[i] = -normal.z / normal.x;
	}
	XMVECTOR tan = XMVectorSet(u[0], u[1], u[2], 0);
	//tan -= vertex0.normal * VectorDot(tan, vertex0.normal);
	vertex0.tangent = XMVector3Normalize(tan);
}

bool FbxParts::GetBonePosition(std::string boneName, XMFLOAT3* position)
{
	for (int i = 0; i < boneNum_; i++)
	{
		if (boneName == ppCluster_[i]->GetLink()->GetName())
		{
			FbxAMatrix matrix;
			ppCluster_[i]->GetTransformLinkMatrix(matrix);
			position->x = (float)matrix[3][0];
			position->y = (float)matrix[3][1];
			position->z = (float)matrix[3][2];
			return true;
		}
	}
	return false;
}

void FbxParts::RayCast(RayCastData& rayData,Transform& transform)
{

	for (int material = 0; material < materialCount_; material++)
	{
		XMVECTOR vDir = XMLoadFloat3(&rayData.dir);
		vDir = XMVector3Normalize(vDir);
		XMStoreFloat3(&rayData.dir, vDir);
		//float prev = 9999.0f;

		XMVECTOR nmlVec1;
		XMVECTOR nmlVec2;
		for (int poly = 0; poly < indexCount_[material]; poly++)
		{
			//3�̒��_�̃x�N�g�����g���ē������Ă��邩�ǂ����̔��ʂ�����
			XMFLOAT3 v0 = { 0,0,0 };
			XMStoreFloat3(&v0, pVertices_[ppIndex_[material][poly]].position);

			XMFLOAT3 v1 = { 0,0,0 };
			XMStoreFloat3(&v1, pVertices_[ppIndex_[material][poly + 1]].position);

			XMFLOAT3 v2 = { 0,0,0 };
			XMStoreFloat3(&v2, pVertices_[ppIndex_[material][poly + 2]].position);

			nmlVec1 = pVertices_[ppIndex_[material][poly + 1]].position - pVertices_[ppIndex_[material][poly]].position;
			nmlVec2 = pVertices_[ppIndex_[material][poly + 2]].position - pVertices_[ppIndex_[material][poly]].position;
			poly += 2;

			XMVECTOR hitPosition;
			float dist;
			XMVECTOR normal = XMVector3Normalize(XMVector3Cross(nmlVec1, nmlVec2));
			float angle = acosf(Clamp<float>(VectorDot(normal, -vDir),-1,1));
			if (Math::Intersect(rayData.start, rayData.dir, v0, v1, v2, dist, hitPosition) && dist < rayData.dist && dist < rayData.distLimit && Math::IsFrontSurface(normal, XMLoadFloat3(&rayData.dir)))
			{
				rayData.normal = normal;
				rayData.hitPos = XMVector3TransformCoord(hitPosition, transform.GetWorldMatrix());
				rayData.dist = dist;
				rayData.angle = angle;
				rayData.hit = true;
			}

		}
	}
}