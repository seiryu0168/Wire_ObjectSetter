#include "Quad.h"
#include"Camera.h"
#include"Transform.h"


Quad::Quad()
{
	pVertexBuffer_ = nullptr;
	pIndexBuffer_ = nullptr;
	pConstantBuffer_ = nullptr;
	pTexture_ = nullptr;

	vertices_ = nullptr;
	index_ = nullptr;
}

Quad::~Quad()
{
	Release();
}

HRESULT Quad::Initialize()
{
	InitVertex();
	InitIndex();
	;
	if (FAILED(CreateVertexBuffer()))
	{
		return E_FAIL;
	}

#pragma region Vertex

	/*D3D11_BUFFER_DESC bd_vertex;
	bd_vertex.ByteWidth = sizeof(vertices);
	bd_vertex.Usage = D3D11_USAGE_DEFAULT;
	bd_vertex.BindFlags = D3D11_BIND_VERTEX_BUFFER;
	bd_vertex.CPUAccessFlags = 0;
	bd_vertex.MiscFlags = 0;
	bd_vertex.StructureByteStride = 0;
	D3D11_SUBRESOURCE_DATA data_vertex;
	data_vertex.pSysMem = vertices;
	hr = Direct3D::pDevice->CreateBuffer(&bd_vertex, &data_vertex, &pVertexBuffer_);
	if (FAILED(hr))
	{
		MessageBox(nullptr, L"頂点データ用バッファの設定に失敗", L"エラー", MB_OK);
		return hr;
	}*/
#pragma endregion 

	//インデックス情報
	//int index[] = { 0,1,2, 0,2,3, 4,5,6, 4,6,7, 8,9,10, 8,10,11, 12,13,14, 12,14,15, 16,17,18, 16,18,19, 20,21,22, 20,22,23 };

	// インデックスバッファを生成する

	if (FAILED(CreateIndexBuffer()))
	{
		return E_FAIL;
	}

#pragma region Index

	/*D3D11_BUFFER_DESC   bd;
	bd.Usage = D3D11_USAGE_DEFAULT;
	bd.ByteWidth = sizeof(index);
	bd.BindFlags = D3D11_BIND_INDEX_BUFFER;
	bd.CPUAccessFlags = 0;
	bd.MiscFlags = 0;

	D3D11_SUBRESOURCE_DATA InitData;
	InitData.pSysMem = index;
	InitData.SysMemPitch = 0;
	InitData.SysMemSlicePitch = 0;
	hr = Direct3D::pDevice->CreateBuffer(&bd, &InitData, &pIndexBuffer_);
	if (FAILED(hr))
	{
		MessageBox(nullptr, L"インデックスバッファの作成に失敗", L"エラー", MB_OK);
		return hr;
	}*/
#pragma endregion

	//コンスタントバッファ作成
	if (FAILED(CreateConstantBuffer()))
	{
		return E_FAIL;
	}

#pragma region Constant
	//D3D11_BUFFER_DESC cb;
	//cb.ByteWidth = sizeof(CONSTANT_BUFFER);
	//cb.Usage = D3D11_USAGE_DYNAMIC;
	//cb.BindFlags = D3D11_BIND_CONSTANT_BUFFER;
	//cb.CPUAccessFlags = D3D11_CPU_ACCESS_WRITE;
	//cb.MiscFlags = 0;
	//cb.StructureByteStride = 0;

	//// コンスタントバッファの作成
	//hr = Direct3D::pDevice->CreateBuffer(&cb, nullptr, &pConstantBuffer_);
	//if (FAILED(hr))
	//{
	//	MessageBox(nullptr, L"コンスタントバッファの作成に失敗", L"エラー", MB_OK);
	//	return hr;
	//}
#pragma endregion constant
	if (FAILED(TextureSet(L"Assets\\dice.png")))
	{
		return E_FAIL;
	}
	

	return S_OK;
}

void Quad::Draw(Transform& transform)
{
	Direct3D::SetShader(SHADER_3D);
	//コンスタントバッファに情報を渡す
	transform.Calclation();
	ToPipeLine(transform);

	
	
	//頂点、インデックス、コンスタントバッファをセット
	bufferSet();

	


}

// 頂点データ用バッファの設定
HRESULT Quad::CreateVertexBuffer()
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
		MessageBox(nullptr, L"頂点データ用バッファの設定に失敗", L"エラー", MB_OK);
		return hr;
	}

	return S_OK;
}

// インデックスバッファを生成する
HRESULT Quad::CreateIndexBuffer()
{
	D3D11_BUFFER_DESC   bd;
	bd.Usage = D3D11_USAGE_DEFAULT;
	bd.ByteWidth = sizeof(int)*indexNum_;
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
		MessageBox(nullptr, L"インデックスバッファの作成に失敗", L"エラー", MB_OK);
		return hr;
	}
	return S_OK;
}

//コンスタントバッファ作成
HRESULT Quad::CreateConstantBuffer()
{
	D3D11_BUFFER_DESC cb;
	cb.ByteWidth = sizeof(CONSTANT_BUFFER);
	cb.Usage = D3D11_USAGE_DYNAMIC;
	cb.BindFlags = D3D11_BIND_CONSTANT_BUFFER;
	cb.CPUAccessFlags = D3D11_CPU_ACCESS_WRITE;
	cb.MiscFlags = 0;
	cb.StructureByteStride = 0;

	// コンスタントバッファの作成
	HRESULT hr = Direct3D::pDevice->CreateBuffer(&cb, nullptr, &pConstantBuffer_);
	if (FAILED(hr))
	{
		MessageBox(nullptr, L"コンスタントバッファの作成に失敗", L"エラー", MB_OK);
		return hr;
	}
	return S_OK;
}

HRESULT Quad::TextureSet(LPCWSTR fileName)
{
	pTexture_ = new Texture;
	if (FAILED(pTexture_->Load(fileName)))
	{
		MessageBox(nullptr, L"画像ロードに失敗しました", L"エラー",MB_OK);
		return E_FAIL;
	}
	return S_OK;
}
void Quad::InitVertex()
{
	// 頂点情報
	VERTEX vertices[] =
	{
		//1
		XMVectorSet(-1.0f,  1.0f, 0.0f, 0.0f),	XMVectorSet(0.0f,0.0f,0.0f,0.0f),	// 四角形の頂点（左上）
		XMVectorSet(1.0f,  1.0f, 0.0f, 0.0f),	XMVectorSet(0.25f,0.0f,0.0f,0.0f),	// 四角形の頂点（右上）
		XMVectorSet(1.0f, -1.0f, 0.0f, 0.0f),	XMVectorSet(0.25f,0.5f,0.0f,0.0f),	// 四角形の頂点（右下）
		XMVectorSet(-1.0f, -1.0f, 0.0f, 0.0f),	XMVectorSet(0.0f,0.5f,0.0f,0.0f),   // 四角形の頂点（左下）
		//2
		XMVectorSet(-1.0f,  1.0f, 2.0f, 0.0f),	XMVectorSet(0.25f,0.0f,0.0f,0.0f),	// 四角形の頂点（左上奥）
		XMVectorSet(1.0f,  1.0f, 2.0f, 0.0f),	XMVectorSet(0.5f,0.0f,0.0f,0.0f),	// 四角形の頂点（右上奥）
		XMVectorSet(1.0f, 1.0f, 0.0f, 0.0f),	XMVectorSet(0.5f,0.5f,0.0f,0.0f),	// 四角形の頂点（右上）
		XMVectorSet(-1.0f, 1.0f, 0.0f, 0.0f),	XMVectorSet(0.25f,0.5f,0.0f,0.0f),   // 四角形の頂点（左上）
		//3
		XMVectorSet(-1.0f,  1.0f, 2.0f, 0.0f),	XMVectorSet(0.5f,0.0f,0.0f,0.0f),	// 四角形の頂点（左上奥）
		XMVectorSet(-1.0f,  1.0f, 0.0f, 0.0f),	XMVectorSet(0.75f,0.0f,0.0f,0.0f),	// 四角形の頂点（左上）
		XMVectorSet(-1.0f, -1.0f, 0.0f, 0.0f),	XMVectorSet(0.75f,0.5f,0.0f,0.0f),	// 四角形の頂点（右下）
		XMVectorSet(-1.0f, -1.0f, 2.0f, 0.0f),	XMVectorSet(0.5f,0.5f,0.0f,0.0f),   // 四角形の頂点（右下奥）
		//4
		XMVectorSet(1.0f,  1.0f, 0.0f, 0.0f),	XMVectorSet(0.75f,0.0f,0.0f,0.0f),	// 四角形の頂点（右上）
		XMVectorSet(1.0f,  1.0f, 2.0f, 0.0f),	XMVectorSet(1.0f,0.0f,0.0f,0.0f),	// 四角形の頂点（右上奥）
		XMVectorSet(1.0f, -1.0f, 2.0f, 0.0f),	XMVectorSet(1.0f,0.5f,0.0f,0.0f),	// 四角形の頂点（右下奥）
		XMVectorSet(1.0f, -1.0f, 0.0f, 0.0f),	XMVectorSet(0.75f,0.5f,0.0f,0.0f),   // 四角形の頂点（右下）
		//5
		XMVectorSet(-1.0f,  -1.0f, 0.0f, 0.0f),	XMVectorSet(0.0f,0.5f,0.0f,0.0f),	// 四角形の頂点（左下）
		XMVectorSet(1.0f,  -1.0f, 0.0f, 0.0f),	XMVectorSet(0.25f,0.5f,0.0f,0.0f),	// 四角形の頂点（右下）
		XMVectorSet(1.0f, -1.0f, 2.0f, 0.0f),	XMVectorSet(0.25f,1.0f,0.0f,0.0f),	// 四角形の頂点（右下奥）
		XMVectorSet(-1.0f, -1.0f, 2.0f, 0.0f),	XMVectorSet(0.0f,1.0f,0.0f,0.0f),   // 四角形の頂点（左下奥）
		//6
		XMVectorSet(1.0f, 1.0f, 2.0f, 0.0f),	XMVectorSet(0.5f,0.5f,0.0f,0.0f),   // 四角形の頂点（左上奥）
		XMVectorSet(-1.0f, 1.0f, 2.0f, 0.0f),	XMVectorSet(0.25f,0.5f,0.0f,0.0f),   // 四角形の頂点（右上奥）
		XMVectorSet(-1.0f, -1.0f, 2.0f, 0.0f),	XMVectorSet(0.25f,1.0f,0.0f,0.0f),   // 四角形の頂点（右下奥）
		XMVectorSet(1.0f, -1.0f, 2.0f, 0.0f),	XMVectorSet(0.5f,1.0f,0.0f,0.0f),   // 四角形の頂点（左下奥）
	};
	vertexNum_ = sizeof(vertices) / sizeof(VERTEX);
	vertices_ = new VERTEX[vertexNum_];
	memcpy(vertices_, vertices, sizeof(vertices));

}
void Quad::InitIndex()
{
	//インデックス情報
	int index[] = { 0,1,2, 0,2,3, 4,5,6, 4,6,7, 8,9,10, 8,10,11, 12,13,14, 12,14,15, 16,17,18, 16,18,19, 20,21,22, 20,22,23 };

	indexNum_ = sizeof(index) / sizeof(int);
	index_ = new int[indexNum_];
	memcpy(index_, index, sizeof(index));

}

//コンスタントバッファに情報を渡す
void Quad::ToPipeLine(Transform& transform)
{
	CONSTANT_BUFFER cb;
	cb.matWVP = XMMatrixTranspose(transform.GetWorldMatrix() * Camera::GetViewMatrix() * Camera::GetProjectionMatrix());
	cb.matNormal = XMMatrixTranspose(transform.GetNormalMatrix());
	D3D11_MAPPED_SUBRESOURCE pdata;
	Direct3D::pContext->Map(pConstantBuffer_, 0, D3D11_MAP_WRITE_DISCARD, 0, &pdata);	// GPUからのデータアクセスを止める
	memcpy_s(pdata.pData, pdata.RowPitch, (void*)(&cb), sizeof(cb));					// データを値を送る

	ID3D11SamplerState* pSampler = pTexture_->GetSampler();
	Direct3D::pContext->PSSetSamplers(0, 1, &pSampler);
	ID3D11ShaderResourceView* pSRV = pTexture_->GetSRV();
	Direct3D::pContext->PSSetShaderResources(0, 1, &pSRV);

	Direct3D::pContext->Unmap(pConstantBuffer_, 0);//再開
}

//頂点、インデックス、コンスタントバッファをセット
void Quad::bufferSet()
{
	//頂点バッファ
	UINT stride = sizeof(VERTEX);
	UINT offset = 0;
	Direct3D::pContext->IASetVertexBuffers(0, 1, &pVertexBuffer_, &stride, &offset);

	// インデックスバッファーをセット
	stride = sizeof(int);
	offset = 0;
	Direct3D::pContext->IASetIndexBuffer(pIndexBuffer_, DXGI_FORMAT_R32_UINT, 0);

	//コンスタントバッファ
	Direct3D::pContext->VSSetConstantBuffers(0, 1, &pConstantBuffer_);	//頂点シェーダー用	
	Direct3D::pContext->PSSetConstantBuffers(0, 1, &pConstantBuffer_);	//ピクセルシェーダー用

	Direct3D::pContext->DrawIndexed(indexNum_, 0, 0);
}

void Quad::Release()
{
	//->Release();
	//SAFE_DELETE(pTexture_);
	SAFE_DELETE_ARRAY(index_);
	SAFE_DELETE_ARRAY(vertices_);
	SAFE_RELEASE(pTexture_);
	SAFE_RELEASE(pConstantBuffer_);
	SAFE_RELEASE(pIndexBuffer_);
	SAFE_RELEASE(pVertexBuffer_);
}