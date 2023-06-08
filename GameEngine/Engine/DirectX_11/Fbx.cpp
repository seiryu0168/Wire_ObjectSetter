#include "Fbx.h"
#include"FbxParts.h"
#include"../GameObject/Camera.h"
#include"Math.h"
#include"Texture.h"
#ifdef _DEBUG
#define _CRTDBG_MAP_ALLOC
#include<memory>
#define DEBUG_NEW new(_NORMAL_BLOCK, __FILE__, __LINE__)
#include <crtdbg.h>
#else
#define DEBUG_NEW	
#endif
#ifdef _DEBUG
#define new DEBUG_NEW
#endif // _DEBUG

Fbx::Fbx()
{
	_CrtSetDbgFlag(_CrtSetDbgFlag(_CRTDBG_REPORT_FLAG) | _CRTDBG_ALLOC_MEM_DF | _CRTDBG_LEAK_CHECK_DF);
	animSpeed_ = 0;
	pFbxManager_ = nullptr;
	pFbxScene_ = nullptr;
}
Fbx::~Fbx()
{
	Release();
	_CrtDumpMemoryLeaks();
}

//FBX���[�h
HRESULT Fbx::Load(std::string fileName)
{
	//�}�l�[�W���𐶐�
	 pFbxManager_ = FbxManager::Create();

	//�C���|�[�^�[�𐶐�
	FbxImporter* fbxImporter = FbxImporter::Create(pFbxManager_, "imp");
	fbxImporter->Initialize(fileName.c_str(), -1, pFbxManager_->GetIOSettings());

	//�V�[���I�u�W�F�N�g��FBX�t�@�C���̏��𗬂�����
	pFbxScene_ = FbxScene::Create(pFbxManager_, "fbxscene");
	fbxImporter->Import(pFbxScene_);
	fbxImporter->Destroy();

	//�A�j���[�V�����̃^�C�����[�h�擾
	frameRate_ = pFbxScene_->GetGlobalSettings().GetTimeMode();

	//���b�V�������擾
	FbxNode* rootNode = pFbxScene_->GetRootNode();
	FbxNode* pNode = rootNode->GetChild(0);
	if (pNode == nullptr)
	{
		MessageBox(nullptr, L"�t�@�C�������݂��܂���", L"�G���[", MB_OK);
		return E_FAIL;
	}
	


	//���݂̃J�����g�f�B���N�g����ޔ����Ă���
	WCHAR defaultDirectory[MAX_PATH];
	GetCurrentDirectory(MAX_PATH, defaultDirectory);
	//fileName����f�B���N�g�����擾
	wchar_t wtext[FILENAME_MAX];
	size_t ret;
	CHAR dir[MAX_PATH];
	WCHAR wDir[MAX_PATH];
	CHAR fName[FILENAME_MAX];
	
	//������𕪊����ăf�B���N�g���ƃt�@�C�����𕪂���
	_splitpath_s(fileName.c_str(), nullptr, 0, dir, MAX_PATH, fName, FILENAME_MAX, nullptr, 0);
	mbstowcs_s(&ret, wDir, dir, MAX_PATH);
	//�t�@�C����
	modelName_ = fName;
	

	//�f�B���N�g���ύX
	SetCurrentDirectory(wDir);
	
	CheckNode(pNode, &parts_);
	
	//�f�B���N�g�������ɖ߂�
	SetCurrentDirectory(defaultDirectory);

	pFbxScene_->Destroy();
	pFbxManager_->Destroy();
	return S_OK;
}

HRESULT Fbx::CheckNode(FbxNode* pNode, std::vector<FbxParts*>* pPartsList)
{
	HRESULT hr;
	FbxNodeAttribute* attr = pNode->GetNodeAttribute();
	if (attr != nullptr && attr->GetAttributeType() == FbxNodeAttribute::eMesh)
	{
		//�p�[�c�C���X�^���X����Ēǉ�
		FbxParts* pParts = new FbxParts;
		hr = pParts->Init(pNode);
		if (FAILED(hr))
		{
			return hr;
		}
		pPartsList->push_back(pParts);
	}

	//�q�m�[�h�����ׂē�������������
	{
		int childCount = pNode->GetChildCount();

		for (int i = 0; i < childCount; i++)
		{
			hr=CheckNode(pNode->GetChild(i), pPartsList);
			if (FAILED(hr))
			{
				return hr;
			}
		}
	}
	return S_OK;
}

///////////////////////////////////////////���_//////////////////////////////////////////
//HRESULT Fbx::InitVertex(fbxsdk::FbxMesh* mesh)
//{
//	//���_��������z��
//	pVertices_ = new VERTEX[vertexCount_];
//
//	//�S�|���S��
//	for (DWORD poly = 0; poly < polygonCount_; poly++)
//	{
//		//3���_��
//		for (int vertex = 0; vertex < 3; vertex++)
//		{
//			//���ׂ钸�_�̔ԍ�
//			int index = mesh->GetPolygonVertex(poly, vertex);
//
//			//���_�̈ʒu
//			FbxVector4 pos = mesh->GetControlPointAt(index);
//			pVertices_[index].position = XMVectorSet((float)pos[0], (float)pos[1], (float)pos[2], 0.0f);
//
//			//���_��UV
//			FbxLayerElementUV* pUV = mesh->GetLayer(0)->GetUVs();
//			int uvIndex = mesh->GetTextureUVIndex(poly, vertex, FbxLayerElement::eTextureDiffuse);
//			FbxVector2  uv = pUV->GetDirectArray().GetAt(uvIndex);
//			pVertices_[index].uv = XMVectorSet((float)uv.mData[0], (float)(1.0f - uv.mData[1]), 0.0f, 0.0f);
//
//			//���_�̖@��
//			FbxVector4 Normal;
//			mesh->GetPolygonVertexNormal(poly, vertex, Normal);	//���Ԗڂ̃|���S���́A���Ԗڂ̒��_�̖@�����Q�b�g
//			pVertices_[index].normal = XMVectorSet((float)Normal[0], (float)Normal[1], (float)Normal[2], 0.0f);
//		
//			
//		}
//#if 1
//		if (mesh->GetElementTangentCount() > 0)
//		{
//			for (int vertex = 0; vertex < 3; vertex++)
//			{
//				int index = mesh->GetPolygonVertex(poly, vertex);
//					//�ڐ�
//					FbxGeometryElementTangent * t = mesh->GetElementTangent(0);
//				FbxVector4 tangent = t->GetDirectArray().GetAt(index).mData;
//				pVertices_[index].tangent = XMVectorSet((float)tangent[0], (float)tangent[1], (float)tangent[2], 0.0f);
//			}
//		}
//		else
//		{
//			for (int vertex = 0; vertex < 3; vertex++)
//			{
//				int index = mesh->GetPolygonVertex(poly, vertex);
//				pVertices_[index].tangent = XMVectorSet(0.0f,0.0f,0.0f, 0.0f);
//			}
//		}
//#endif
//	}
//
//	D3D11_BUFFER_DESC bd_vertex;
//	bd_vertex.ByteWidth = sizeof(VERTEX) * vertexCount_;
//	bd_vertex.Usage = D3D11_USAGE_DEFAULT;
//	bd_vertex.BindFlags = D3D11_BIND_VERTEX_BUFFER;
//	bd_vertex.CPUAccessFlags = 0;
//	bd_vertex.MiscFlags = 0;
//	bd_vertex.StructureByteStride = 0;
//	D3D11_SUBRESOURCE_DATA data_vertex;
//	data_vertex.pSysMem = pVertices_;
//	HRESULT hr = Direct3D::pDevice->CreateBuffer(&bd_vertex, &data_vertex, &pVertexBuffer_);
//	if (FAILED(hr))
//	{
//		MessageBox(nullptr, L"���_�f�[�^�p�o�b�t�@�̐ݒ�Ɏ��s", L"�G���[", MB_OK);
//		return hr;
//	}
//
//	return S_OK;
//}
//
///////////////////////////////�C���f�b�N�X///////////////////////////////////
//HRESULT Fbx::InitIndex(fbxsdk::FbxMesh* mesh)
//{
//	ppIndex_ = new int*[materialCount_];
//	pIndexBuffer_ = new ID3D11Buffer * [materialCount_];
//	indexCount_ = new int[materialCount_];
//
//	for (int i = 0; i < materialCount_; i++)
//	{
//		int count = 0;
//		ppIndex_[i] = new int[polygonCount_ * 3];
//		//�S�|���S��
//		for (DWORD poly = 0; poly < polygonCount_; poly++)
//		{
//			FbxLayerElementMaterial* mtl = mesh->GetLayer(0)->GetMaterials();
//			int mtlId = mtl->GetIndexArray().GetAt(poly);
//
//			if (mtlId == i)
//			{
//				//3���_��
//				for (DWORD vertex = 0; vertex < 3; vertex++)
//				{
//					ppIndex_[i][count] = mesh->GetPolygonVertex(poly, vertex);
//					count++;
//				}
//			}
//		}
//		indexCount_[i] = count;
//
//		D3D11_BUFFER_DESC   bd;
//		bd.Usage = D3D11_USAGE_DEFAULT;
//		bd.ByteWidth = sizeof(int) * polygonCount_ * 3;
//		bd.BindFlags = D3D11_BIND_INDEX_BUFFER;
//		bd.CPUAccessFlags = 0;
//		bd.MiscFlags = 0;
//
//		D3D11_SUBRESOURCE_DATA InitData;
//		InitData.pSysMem = ppIndex_[i];
//		InitData.SysMemPitch = 0;
//		InitData.SysMemSlicePitch = 0;
//		HRESULT hr = Direct3D::pDevice->CreateBuffer(&bd, &InitData, &pIndexBuffer_[i]);
//		if (FAILED(hr))
//		{
//			MessageBox(nullptr, L"�C���f�b�N�X�o�b�t�@�̍쐬�Ɏ��s", L"�G���[", MB_OK);
//			return hr;
//		}
//	}
//	return S_OK;
//}
//
////////////////////////////////////�R���X�^���g�o�b�t�@�쐬///////////////////////////////////	
//HRESULT Fbx::CreateConstantBuffer()
//{
//	D3D11_BUFFER_DESC cb;
//	cb.ByteWidth = sizeof(CONSTANT_BUFFER);
//	cb.Usage = D3D11_USAGE_DYNAMIC;
//	cb.BindFlags = D3D11_BIND_CONSTANT_BUFFER;
//	cb.CPUAccessFlags = D3D11_CPU_ACCESS_WRITE;
//	cb.MiscFlags = 0;
//	cb.StructureByteStride = 0;
//
//	// �R���X�^���g�o�b�t�@�̍쐬
//	HRESULT hr = Direct3D::pDevice->CreateBuffer(&cb, nullptr, &pConstantBuffer_);
//	if (FAILED(hr))
//	{
//		MessageBox(nullptr, L"�R���X�^���g�o�b�t�@�̍쐬�Ɏ��s", L"�G���[", MB_OK);
//		return hr;
//	}
//	return S_OK;
//}
//
//void Fbx::InitMaterial(fbxsdk::FbxNode* pNode)
//{
//	pMaterialList_ = new MATERIAL[materialCount_];
//
//	for (int i = 0; i < materialCount_; i++)
//	{
//		//i�Ԗڂ̃}�e���A�������擾
//		FbxSurfaceMaterial* pMaterial = pNode->GetMaterial(i);
//		//�t�H���V�F�[�f�B���O�ׂ̈Ƀ}�e���A���������o��
//		FbxSurfacePhong* pPhong = (FbxSurfacePhong*)pMaterial;
//
//		//�e�N�X�`�����
//		FbxProperty  lProperty = pMaterial->FindProperty(FbxSurfaceMaterial::sDiffuse);
//
//		//�󂯎��p�̊e�}�e���A���p�����[�^�̏�����
//		FbxDouble3 diffuse = FbxDouble3(0, 0, 0);
//		FbxDouble3 ambient = FbxDouble3(0, 0, 0);
//		FbxDouble3 speculer = FbxDouble3(0, 0, 0);
//
//		diffuse = pPhong->Diffuse;
//		ambient = pPhong->Ambient;
//		//diffuse��ambient������
//		pMaterialList_[i].diffuse = XMFLOAT4((float)diffuse[0], (float)diffuse[1], (float)diffuse[2], 1.0f);
//		pMaterialList_[i].ambient = XMFLOAT4((float)ambient[0], (float)ambient[1], (float)ambient[2], 1.0f);
//		pMaterialList_[i].speculer = XMFLOAT4(0, 0, 0, 0);
//		pMaterialList_[i].shininess = 0;
//		
//		//Phong�̃N���X�Ȃ̂�Lambert�̃N���X�Ȃ̂��𔻕�(Lambert���ƃX�y�L�����[�̒l�������Ȃ�)
//		if (pPhong->GetClassId().Is(FbxSurfacePhong::ClassId))
//		{
//			speculer = pPhong->Specular;
//			pMaterialList_[i].speculer = XMFLOAT4((float)speculer[0], (float)speculer[1], (float)speculer[2], 1.0f);
//			//blender����1-�e���̒lx100�ɂ�����
//			pMaterialList_[i].shininess = pPhong->Shininess;
//		}
//
//		//�e�N�X�`���̖���
//		//pMaterialList_[i].pTexture = lProperty.GetSrcObjectCount<FbxFileTexture>();
//		int count = lProperty.GetSrcObjectCount<FbxFileTexture>();
//		if(lProperty.GetSrcObjectCount<FbxFileTexture>()>0)
//		{
//			FbxFileTexture* textureInfo = lProperty.GetSrcObject<FbxFileTexture>(0);
//			const char* textureFilePath = textureInfo->GetRelativeFileName();
//
//			//�p�X�����t�@�C�����Ɗg���q�����ɂ���
//			char name[_MAX_FNAME];	//�t�@�C����
//			char ext[_MAX_EXT];		//�g���q
//			_splitpath_s(textureFilePath, nullptr, 0, nullptr, 0, name, _MAX_FNAME, ext, _MAX_EXT);
//			sprintf_s(name, "%s%s", name, ext);
//
//			wchar_t wtext[FILENAME_MAX];
//			size_t ret;
//			mbstowcs_s(&ret, wtext, name, strlen(name));
//
//			//�t�@�C������e�N�X�`���쐬
//			pMaterialList_[i].pTexture = new Texture;
//			pMaterialList_[i].pTexture->Load(wtext);
//		}
//
//		//�e�N�X�`������
//		else
//		{
//			pMaterialList_[i].pTexture = nullptr;
//			//�}�e���A���̐F
//			FbxSurfaceLambert* pMaterial = (FbxSurfaceLambert*)pNode->GetMaterial(i);
//			FbxDouble3  diffuse = pMaterial->Diffuse;
//			pMaterialList_[i].diffuse = XMFLOAT4((float)diffuse[0], (float)diffuse[1], (float)diffuse[2], 1.0f);
//		}
//
//		/////////////////////////////////�m�[�}���}�b�v//////////////////////////
//		{
//			FbxProperty IPropaty = pMaterial->FindProperty(FbxSurfaceMaterial::sBump);
//			int normalMapCount = IPropaty.GetSrcObjectCount<FbxFileTexture>();
//
//			if (normalMapCount != 0)
//			{
//				FbxFileTexture* textureInfo = IPropaty.GetSrcObject<FbxFileTexture>(0);
//				const char* textureFilePath = textureInfo->GetRelativeFileName();
//
//				//�t�@�C����+�g�������ɂ���
//				char name[_MAX_FNAME];	//�t�@�C����
//				char ext[_MAX_EXT];	//�g���q
//				_splitpath_s(textureFilePath, nullptr, 0, nullptr, 0, name, _MAX_FNAME, ext, _MAX_EXT);
//
//				sprintf_s(name, "%s%s", name, ext);
//
//				//�t�@�C������e�N�X�`���쐬
//				wchar_t wtext[FILENAME_MAX];
//				size_t ret;
//				mbstowcs_s(&ret, wtext, name, strlen(name));
//
//				pMaterialList_[i].pNormalMap = new Texture;
//				pMaterialList_[i].pNormalMap->Load(wtext);
//			}
//			else
//			{
//				pMaterialList_[i].pNormalMap = nullptr;
//			}
//		}
//	}
//}

void Fbx::Draw(Transform& transform, SHADER_TYPE shaderType,int frame)
{
	Direct3D::SetShader(shaderType);
	Direct3D::SetBlendMode(BLEND_MODE::BLEND_DEFAULT);
	for (int i = 0; i<parts_.size(); i++)
	{
		FbxTime time;
		time.SetTime(0, 0, 0, frame,0, 0, frameRate_);

		if (parts_[i]->GetSkinInfo())
		{
			parts_[i]->DrawSkinAnime(transform, frame);
		}
		else
		{
			parts_[i]->Draw(transform);
		}
	}
}

void Fbx::DrawOutLine(Transform& transform, int frame, XMFLOAT4 lineColor)
{
	Direct3D::SetBlendMode(BLEND_MODE::BLEND_DEFAULT);
	Direct3D::SetShader(SHADER_TYPE::SHADER_OUTLINE);
	for (int i = 0; i < parts_.size(); i++)
	{
		FbxTime time;
		time.SetTime(0, 0, 0, frame, 0, 0, frameRate_);

		if (parts_[i]->GetSkinInfo())
		{
			parts_[i]->DrawSkinAnime(transform, frame,lineColor);
		}
		else
		{

			parts_[i]->Draw(transform,lineColor);
		}
	}

	Direct3D::SetShader(SHADER_TYPE::SHADER_3D);
	for (int i = 0; i < parts_.size(); i++)
	{
		FbxTime time;
		time.SetTime(0, 0, 0, frame, 0, 0, frameRate_);

		if (parts_[i]->GetSkinInfo())
		{
			parts_[i]->DrawSkinAnime(transform, frame);
		}
		else
		{
			parts_[i]->Draw(transform);
		}
	}
}

void Fbx::DrawToon(Transform& transform, bool isOutLine, int frame)
{
	Direct3D::SetBlendMode(BLEND_MODE::BLEND_DEFAULT);
	if (isOutLine)
	{
		Direct3D::SetShader(SHADER_TYPE::SHADER_OUTLINE);

		for (int i = 0; i < parts_.size(); i++)
		{
			FbxTime time;
			time.SetTime(0, 0, 0, frame, 0, 0, frameRate_);

			if (parts_[i]->GetSkinInfo())
			{
				parts_[i]->DrawSkinAnime(transform, frame);
			}
			else
			{
				parts_[i]->Draw(transform);
			}
		}
	}
	Direct3D::SetShader(SHADER_TYPE::SHADER_TOON);
	for (int i = 0; i < parts_.size(); i++)
	{
		FbxTime time;
		time.SetTime(0, 0, 0, frame, 0, 0, frameRate_);

		if (parts_[i]->GetSkinInfo())
		{
			parts_[i]->DrawSkinAnime(transform, frame);
		}
		else
		{
			parts_[i]->Draw(transform);
		}
	}
}

void Fbx::RayCast(RayCastData& ray,Transform& transform)
{
	for (int i = 0; i < parts_.size(); i++)
	{
		parts_[i]->RayCast(ray,transform);
	}

	//for (int material = 0; material < materialCount_; material++)
	//{
	//	XMVECTOR vDir = XMLoadFloat3(&ray.dir);
	//	vDir = XMVector3Normalize(vDir);
	//	XMStoreFloat3(&ray.dir, vDir);
	//	//float prev = 9999.0f;
	//	XMVECTOR nmlVec1;
	//	XMVECTOR nmlVec2;
	//	for (int poly = 0; poly < indexCount_[material]; poly++)
	//	{
	//		XMFLOAT3 v0 = {0,0,0};
	//		XMStoreFloat3(&v0, pVertices_[ppIndex_[material][poly]].position);
	//		XMFLOAT3 v1 = { 0,0,0 };
	//		XMStoreFloat3(&v1, pVertices_[ppIndex_[material][poly+1]].position);
	//		XMFLOAT3 v2 = { 0,0,0 };
	//		XMStoreFloat3(&v2, pVertices_[ppIndex_[material][poly+2]].position);
	//		nmlVec1 = pVertices_[ppIndex_[material][poly + 1]].position - pVertices_[ppIndex_[material][poly]].position;
	//		nmlVec2 = pVertices_[ppIndex_[material][poly + 2]].position - pVertices_[ppIndex_[material][poly]].position;
	//		poly += 2;
	//		XMVECTOR hitPosition;
	//		float dist;
	//		XMVECTOR normal= XMVector3Normalize(XMVector3Cross(nmlVec1, nmlVec2));
	//		if (Math::Intersect(ray.start,ray.dir, v0, v1, v2,dist,hitPosition)&&dist<ray.dist&& dist<ray.distLimit&&Math::IsFrontSurface(normal,XMLoadFloat3(&ray.dir)))
	//		{
	//			ray.normal		= normal;
	//			ray.hitPos      = XMVector3TransformCoord(hitPosition, transform.GetWorldMatrix());
	//			ray.dist		= dist;
	//			ray.hit			= true;
	//		}
	//	}
	//}
}

XMFLOAT3 Fbx::GetBonePosition(std::string boneName)
{
	XMFLOAT3 position = { 0,0,0 };
	for (int i = 0; i < parts_.size(); i++)
	{
		if (parts_[i]->GetBonePosition(boneName, &position))
		{
			break;
		}
	}
	return position;
}

std::string Fbx::GetModelName()
{
	return modelName_;
}

void Fbx::Release()
{
	for (int i = 0; i < parts_.size(); i++)
	{
		SAFE_DELETE(parts_[i]);
	}
	parts_.clear();
	_CrtDumpMemoryLeaks();
}