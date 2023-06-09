#pragma once
#include"Component.h"
#include"Engine/DirectX_11/Math.h"
class TransformComponent : public Component
{
private:
	XMVECTOR RotationQuaternion_;

	XMMATRIX matTranslate_;	//移動行列
	XMMATRIX matRotate_;	//回転行列	
	XMMATRIX matScale_;	//拡大行列
public:
	XMFLOAT3 position_;	//位置
	XMFLOAT3 rotate_;	//向き
	XMFLOAT3 scale_;	//拡大率
	XMVECTOR baseVec_;	//基準となるベクトル
		
	TransformComponent* pParent_;//親の情報
	TransformComponent(GameObject* atcObj);
	~TransformComponent();

	//各行列の計算
	void Calclation();

	//回転行列と拡縮の逆行列の計算
	XMMATRIX GetNormalMatrix();

	//ワールド行列を取得
	XMMATRIX GetWorldMatrix();
	XMMATRIX GetLocalMatrix();
	XMMATRIX GetLocalTranslateMatrix();
	XMMATRIX GetLocalRotateMatrix();
	XMMATRIX GetLocalScaleMatrix();

	XMMATRIX GetWorldTranslateMatrix();
	XMMATRIX GetWorldRotateMatrix();
	XMMATRIX GetWorldScaleMatrix();
	void SetParentTransformComponent(TransformComponent* parent);

	static XMFLOAT3 Float3Add(XMFLOAT3 add1, XMFLOAT3 add2)
	{
		return XMFLOAT3(add1.x + add2.x, add1.y + add2.y, add1.z + add2.z);
	}


};

