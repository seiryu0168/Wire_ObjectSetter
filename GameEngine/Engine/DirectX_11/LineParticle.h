#pragma once
#include"Direct3D.h"
#include"Texture.h"
#include"DirectXMath.h"
#include"../GameObject/Transform.h"
#include<list>

class LineParticle
{
	enum class LineMode
	{
		LINE_DEFAULT=0,
		LINE_CROSS,
	};

private:
	struct CONSTANT_BUFFER
	{
		XMMATRIX	matWVP;
		XMFLOAT4	color;
	};

	//頂点情報
	struct VERTEX
	{
		XMFLOAT3 position;
		XMFLOAT3 uv;
	};

	float WIDTH_;//ラインパーティクルの幅
	UINT LENGTH_;//ラインパーティクルのポジションを記憶する量
	float tipWidth_;
	XMFLOAT4 color_;
	//int* index_;

	ID3D11Buffer* pVertexBuffer_;
	ID3D11Buffer* pIndexBuffer_;
	ID3D11Buffer* pConstantBuffer_;

	Texture* pTexture_;
	std::vector<int> indexList;
	std::list<XMFLOAT3> positionList_;
public:
	LineParticle();
	LineParticle(float width,int length,float tipWidth);
	/// <summary>
	/// ラインの位置保存
	/// 位置をポジションリストに追加
	/// </summary>
	/// <param name="pos">この関数が呼ばれた時の位置</param>
	void AddPosition(XMFLOAT3 pos);
	/// <summary>
	/// パイプ状のラインパーティクル生成
	/// </summary>
	/// <param name="pList"></param>
	/// <returns></returns>
	HRESULT CreateMeshPype(std::list<XMFLOAT3>* pList);
	HRESULT CreateMeshPlate(std::list<XMFLOAT3>* pList);
	/// <summary>
	/// ラインパーティクルに使う画像のロード
	/// </summary>
	/// <param name="fileName"></param>
	/// <returns></returns>
	HRESULT Load(std::string fileName);
	/// <summary>
	/// ラインパーティクルの色設定
	/// </summary>
	/// <param name="col">色</param>
	void SetColor(XMFLOAT4 col);
	/// <summary>
	/// インデックス生成
	/// </summary>
	void SetIndex();
	/// <summary>
	/// 描画
	/// </summary>
	/// <param name="transform">呼び出し元のtransform</param>
	void Draw(Transform* transform);
	/// <summary>
	/// ラインパーティクルの各パラメータ設定
	/// </summary>
	/// <param name="width">横幅</param>
	/// <param name="length">ポジションの保存数</param>
	/// <param name="tipWidth">終端の幅</param>
	void SetLineParameter(float width, int length,float tipWidth=0);
	/// <summary>
	/// ポジションリストの削除
	/// </summary>
	void DeleteLine();
	void Release();



};

