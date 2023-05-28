#pragma once
#include<DirectXMath.h>
#include<fbxsdk.h>
#include<d3d11.h>
#include"Texture.h"
#include"../GameObject/Transform.h"
#include"Fbx.h"

class FbxParts
{
private:
	//マテリアル
	//質感
	struct MATERIAL
	{
		Texture* pTexture;    //テクスチャ
		Texture* pNormalMap;  //ノーマルマップ
		XMFLOAT4 diffuse;	  //ディフューズ(マテリアルの色)
		XMFLOAT4 ambient;	  //アンビエント
		XMFLOAT4 speculer;    //スペキュラー(ツルツルを表現する奴)
		float shininess;	  //ハイライトの強さ

	};

	//シェーダーに送る情報をまとめた構造体
	struct CONSTANT_BUFFER
	{
		XMMATRIX matWVP;			//ワールド、ビュー、プロジェクション行列の合成(頂点変換に使う)
		XMMATRIX matW;				//ワールド行列
		XMMATRIX matNormal;			//回転行列と拡大行列の合成(法線の変形に使う)
		XMFLOAT4 diffuseColor;		//ディフューズ(マテリアルの色)
		XMFLOAT4 ambient;			//アンビエント
		XMFLOAT4 speculer;			//スペキュラー
		XMFLOAT4 lightDirection;	//ライトの向き
		XMFLOAT4 cameraPosition;	//カメラの位置
		XMFLOAT4 customColor;       //プログラム側で色を変えたい場合
		FLOAT	 shininess;			//ハイライトの強さ
		BOOL	 isTexture;			//テクスチャが貼ってあるかどうか
		BOOL     isNormal;	//ノーマルマップがあるかどうか
	};

	//頂点に格納される情報
	//mayaだとfbx出力する時に頂点法線の設定をしないと表示がおかしくなる
	//blenderの場合は自動スムーズを切ると正しく表示される
	struct VERTEX
	{
		XMVECTOR position;
		XMVECTOR uv;
		XMVECTOR normal;//blenderではスムーズシェードにした上でノーマルの自動スムーズを切らないと法線のデータがおかしくなって正しく表示されない
		XMVECTOR tangent;//接線
	};

	struct BONE
	{
		XMMATRIX bindPose;		//初期ポーズ時のボーン変換行列
		XMMATRIX newPose;		//アニメーションで変化した時のボーン変換行列
		XMMATRIX diffPose;		//mBindPoseに対するmNowPoseの変化量
	};

	struct WEIGHT
	{
		XMVECTOR originPos;			//元の頂点座標
		XMVECTOR originNormal;		//元の法線ベクトル
		int*	 pBoneIndex;		//関連するボーンのID
		float*	 pBoneWeight;		//ボーンのウェイト
	};

	int vertexCount_;		//頂点数
	int polygonCount_;		//ポリゴン数
	int materialCount_;		//マテリアル数
	int boneNum_;			//ボーンの数


	ID3D11Buffer* pVertexBuffer_;	//頂点バッファ
	ID3D11Buffer** ppIndexBuffer_;	//インデックスバッファ
	ID3D11Buffer* pConstantBuffer_;	//コンスタントバッファ

	FbxSkin*	 pSkinInfo_;     //スキンメッシュの情報
	FbxCluster** ppCluster_;	 //クラスタ情報
	BONE*		 pBoneArray_;	 //各ボーンの情報
	WEIGHT*		 pWeightArray_;	 //ウェイトの情報
	
	MATERIAL* pMaterialList_;//マテリアルリスト
	int*	  indexCount_;

	int**	ppIndex_;
	VERTEX* pVertices_;
	Texture* pToonTexture_;

	HRESULT InitVertex(fbxsdk::FbxMesh* mesh);
	HRESULT InitIndex(fbxsdk::FbxMesh* mesh);
	HRESULT CreateConstantBuffer();
	HRESULT InitSkelton(FbxMesh* pMesh);
	void InitMaterial(fbxsdk::FbxNode* pNode);
	void CalcTangent(VERTEX& vertex0, const VERTEX& vertex1, const VERTEX& vertex2);
public:
	FbxParts();
	~FbxParts();

	HRESULT Init(FbxNode* pNode);
	void Draw(Transform& transform, XMFLOAT4 lineColor = { 0,0,0,1 });
	void DrawSkinAnime(Transform& transform, FbxTime time, XMFLOAT4 lineColor = { 0,0,0,0 });
	//void DrawMeshAnime(Transform& transform, FbxTime time, FbxScene* scene);
	FbxSkin* GetSkinInfo() { return pSkinInfo_; }
	bool GetBonePosition(std::string boneName, XMFLOAT3* position);
	void RayCast(RayCastData& rayData, Transform& transform);



};

