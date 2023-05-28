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

	// インデックスバッファを生成する
	if (FAILED(CreateIndexBuffer()))
	{
		return E_FAIL;
	}

	//コンスタントバッファ作成
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
	//コンスタントバッファに情報を渡す
	transform.Calclation();

	//画面のサイズに合わせる行列
	XMMATRIX matImageSize = XMMatrixScaling((float)(1.0f / Direct3D::GetScreenWidth()), (float)(1.0f / Direct3D::GetScreenHeight()), 1.0f);
	//切り抜きサイズに合わせる行列
	XMMATRIX matCut = XMMatrixScaling((float)rect.right, (float)rect.bottom, 1.0f);
	CONSTANT_BUFFER cb;
	
	//最終的な行列
	cb.matWorld = XMMatrixTranspose(matCut*transform.GetWorldScaleMatrix() * matImageSize * transform.GetWorldRotateMatrix() * transform.GetWorldTranslateMatrix());
	
	XMMATRIX matTexTrans = XMMatrixTranslation((float)rect.left / size_.x, (float)rect.top / size_.y, 1.0f);
	XMMATRIX matTexScale = XMMatrixScaling((float)rect.right / size_.x, (float)rect.bottom / size_.y, 1.0f);

	cb.matUVTrans = XMMatrixTranspose(matTexScale * matTexTrans);
	cb.color = XMFLOAT4(1, 1, 1, alpha);
	cb.ChangeColor = changeColor;

	D3D11_MAPPED_SUBRESOURCE pdata;
	Direct3D::pContext->Map(pConstantBuffer_, 0, D3D11_MAP_WRITE_DISCARD, 0, &pdata);	// GPUからのデータアクセスを止める
	memcpy_s(pdata.pData, pdata.RowPitch, (void*)(&cb), sizeof(cb));					// データを値を送る

	ID3D11SamplerState* pSampler = TextureManager::GetTexture(hPict_)->GetSampler();
	Direct3D::pContext->PSSetSamplers(0, 1, &pSampler);
	ID3D11ShaderResourceView* pSRV = TextureManager::GetTexture(hPict_)->GetSRV();
	Direct3D::pContext->PSSetShaderResources(0, 1, &pSRV);

	Direct3D::pContext->Unmap(pConstantBuffer_, 0);//再開
	
	//頂点、インデックス、コンスタントバッファをセット
	
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

	Direct3D::SetDepthBufferWriteEnable(true);
}

// 頂点データ用バッファの設定
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
		MessageBox(nullptr, L"頂点データ用バッファの設定に失敗", L"エラー", MB_OK);
		return hr;
	}

	return S_OK;
}

// インデックスバッファを生成する
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
		MessageBox(nullptr, L"インデックスバッファの作成に失敗", L"エラー", MB_OK);
		return hr;
	}
	return S_OK;
}

//コンスタントバッファ作成
HRESULT Sprite::CreateConstantBuffer()
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

HRESULT Sprite::Load(std::string fileName)
{
	hPict_ = TextureManager::Load(fileName.c_str());
	if (hPict_<0)
	{
		MessageBox(nullptr, L"画像ロードに失敗しました", L"エラー", MB_OK);
		return E_FAIL;
	}
	size_ = { (float)TextureManager::GetTexture(hPict_)->GetWidth(),(float)TextureManager::GetTexture(hPict_)->GetHeight(),1.0f };
	
	InitVertex();
	InitIndex();
	if (FAILED(CreateVertexBuffer()))
	{
		return E_FAIL;
	}

	// インデックスバッファを生成する
	if (FAILED(CreateIndexBuffer()))
	{
		return E_FAIL;
	}

	//コンスタントバッファ作成
	if (FAILED(CreateConstantBuffer()))
	{
		return E_FAIL;
	}
	return S_OK;
}

void Sprite::InitVertex()
{
	// 頂点情報
	VERTEX vertices[] =
	{
		//1
		XMVectorSet( -1.0f,  1.0f, 0.0f, 0.0f),	XMVectorSet(0.0f,0.0f,0.0f,0.0f),	// 四角形の頂点（左上）
		XMVectorSet(  1.0f,  1.0f, 0.0f, 0.0f),	XMVectorSet(1.0f,0.0f,0.0f,0.0f),	// 四角形の頂点（右上）
		XMVectorSet(  1.0f, -1.0f, 0.0f, 0.0f),	XMVectorSet(1.0f,1.0f,0.0f,0.0f),	// 四角形の頂点（右下）
		XMVectorSet( -1.0f, -1.0f, 0.0f, 0.0f),	XMVectorSet(0.0f,1.0f,0.0f,0.0f),   // 四角形の頂点（左下）
	};
	vertexNum_ = sizeof(vertices) / sizeof(VERTEX);
	vertices_ = new VERTEX[vertexNum_];
	memcpy(vertices_, vertices, sizeof(vertices));
}

void Sprite::InitIndex()
{
	//インデックス情報
	int index[] = { 0,1,2, 0,2,3};

	indexNum_ = sizeof(index) / sizeof(int);
	index_ = new int[indexNum_];
	memcpy(index_, index, sizeof(index));
}

//コンスタントバッファに情報を渡す
//void Sprite::ToPipeLine(Transform& transform)
//{
//	XMMATRIX matImageSize = XMMatrixScaling((float)(size_.x / Direct3D::GetScreenWidth()), (float)(size_.y / Direct3D::GetScreenHeight()), 1.0f);
//	CONSTANT_BUFFER cb;
//	cb.matPosition = XMMatrixTranspose(transform.GetWorldScaleMatrix()*matImageSize*transform.GetWorldRotateMatrix()*transform.GetWorldTranslateMatrix());
//	D3D11_MAPPED_SUBRESOURCE pdata;
//	Direct3D::pContext->Map(pConstantBuffer_, 0, D3D11_MAP_WRITE_DISCARD, 0, &pdata);	// GPUからのデータアクセスを止める
//	memcpy_s(pdata.pData, pdata.RowPitch, (void*)(&cb), sizeof(cb));					// データを値を送る
//
//	ID3D11SamplerState* pSampler = TextureManager::GetTexture(hPict_)->GetSampler();
//	Direct3D::pContext->PSSetSamplers(0, 1, &pSampler);
//	ID3D11ShaderResourceView* pSRV = TextureManager::GetTexture(hPict_)->GetSRV();
//	Direct3D::pContext->PSSetShaderResources(0, 1, &pSRV);
//
//	Direct3D::pContext->Unmap(pConstantBuffer_, 0);//再開
//}
//
////頂点、インデックス、コンスタントバッファをセット
//void Sprite::bufferSet()
//{
//	//頂点バッファ
//	UINT stride = sizeof(VERTEX);
//	UINT offset = 0;
//	Direct3D::pContext->IASetVertexBuffers(0, 1, &pVertexBuffer_, &stride, &offset);
//
//	// インデックスバッファーをセット
//	stride = sizeof(int);
//	offset = 0;
//	Direct3D::pContext->IASetIndexBuffer(pIndexBuffer_, DXGI_FORMAT_R32_UINT, 0);
//
//	//コンスタントバッファ
//	Direct3D::pContext->VSSetConstantBuffers(0, 1, &pConstantBuffer_);	//頂点シェーダー用
//	Direct3D::pContext->PSSetConstantBuffers(0, 1, &pConstantBuffer_);	//ピクセルシェーダー用
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