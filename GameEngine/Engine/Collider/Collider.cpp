#include "Collider.h"
#include"BoxCollider.h"
#include"SphereCollider.h"
#include"OBBCollider.h"
#include"PolygonCollider.h"
#include"../GameObject/GameObject.h"
#include"../../DebugUI.h"
//#include"../DirectX_11/Fbx.h"
#include"../ResourceManager/Model.h"

Collider::Collider()
{
	pColObject_ = nullptr;
}
Collider::~Collider()
{

}

bool Collider::IsHitBox_Box(BoxCollider* boxB, BoxCollider* boxA)
{
	//BoxPosにFloat3Addの戻り値を入れる
	//引数
	//自分の座標
	//相手の座標

	XMFLOAT3 boxPos1 = Transform::Float3Add(boxA->pColObject_->GetWorldPosition(), boxA->center_);
	XMFLOAT3 boxPos2 = Transform::Float3Add(boxB->pColObject_->GetWorldPosition(), boxB->center_);

	if ((boxPos1.x + boxA->size_.x/ 2) > (boxPos2.x - boxB->size_.x / 2) &&
		(boxPos1.x - boxA->size_.x / 2) < (boxPos2.x + boxB->size_.x / 2) &&
		(boxPos1.y + boxA->size_.y / 2) > (boxPos2.y - boxB->size_.y / 2) &&
		(boxPos1.y - boxA->size_.y / 2) < (boxPos2.y + boxB->size_.y / 2) &&
		(boxPos1.z + boxA->size_.z / 2) > (boxPos2.z - boxB->size_.z / 2) &&
		(boxPos1.z - boxA->size_.z / 2) < (boxPos2.z + boxB->size_.z / 2))
	{
		return true;
	}

	return false;

}

bool Collider::IsHitBox_Sphere(BoxCollider* box, SphereCollider* sphere)
{
	XMFLOAT3 boxPos = Transform::Float3Add(box->pColObject_->GetWorldPosition(), box->center_);
	XMFLOAT3 spherePos= Transform::Float3Add(sphere->pColObject_->GetWorldPosition(), sphere->center_);

	if (spherePos.x > boxPos.x - box->size_.x/2 - sphere->size_.x &&
		spherePos.x < boxPos.x + box->size_.x/2 + sphere->size_.x &&
		spherePos.y > boxPos.y - box->size_.y/2 - sphere->size_.x &&
		spherePos.y < boxPos.y + box->size_.y/2 + sphere->size_.x &&
		spherePos.z > boxPos.z - box->size_.z/2 - sphere->size_.x &&
		spherePos.z < boxPos.z + box->size_.z/2 + sphere->size_.x)
	{
		return true;
	}

	return false;
}

bool Collider::IsHitSphere_Sphere(SphereCollider* sphereA, SphereCollider* sphereB)
{
	XMFLOAT3 spherePosA = sphereA->center_;
	XMFLOAT3 positionA = sphereA->pColObject_->GetWorldPosition();
	XMFLOAT3 spherePosB = sphereB->center_;
	XMFLOAT3 positionB = sphereB->pColObject_->GetWorldPosition();

	XMVECTOR vec = (XMLoadFloat3(&spherePosA) + XMLoadFloat3(&positionA)) - 
				   (XMLoadFloat3(&spherePosB) + XMLoadFloat3(&positionB));

	if (XMVector3Length(vec).m128_f32[0] <= sphereA->size_.x + sphereB->size_.x)
	{
		return true;
	}
	return false;

}

//bool Collider::IsHitSphere_Polygon(SphereCollider* sphereA, std::vector<XMVECTOR> polyList)
//{
//	return false;
//}

bool Collider::IsHitOBB_OBB(OBBCollider* obbA, OBBCollider* obbB)
{
	XMFLOAT3 obbAPos = obbA->pColObject_->GetTransform().position_;
	XMFLOAT3 obbBPos = obbB->pColObject_->GetTransform().position_;
	
	XMVECTOR betweenCenterPoint = XMLoadFloat3(&obbBPos) - XMLoadFloat3(&obbAPos);

	obbA->CalcAxisVec();
	obbB->CalcAxisVec();

	
	
	float rA = XMVectorGetX(XMVector3Length(obbA->OBB_X));								    //obbAの中心点からの長さ
	float rB = obbA->prjLine(&obbA->OBB_X, &obbB->OBB_X, &obbB->OBB_Y, &obbB->OBB_Z);		//obbBの中心点からの長さ
	float  L  = fabsf(XMVectorGetX(XMVector3Dot(betweenCenterPoint, XMVector3Normalize(obbA->OBB_X))));//中心点間の長さ
	if (L > rA + rB)
	{
		return false;
	}

	rA = XMVectorGetX(XMVector3Length(obbA->OBB_Y));								    //obbAの中心点からの長さ
	rB = obbA->prjLine(&obbA->OBB_Y, &obbB->OBB_X, &obbB->OBB_Y, &obbB->OBB_Z);			//obbBの中心点からの長さ
	L  = fabsf(XMVectorGetX(XMVector3Dot(betweenCenterPoint, XMVector3Normalize(obbA->OBB_Y))));//中心点間の長さ
	if (L > rA + rB)
	{
		return false;
	}

	rA = XMVectorGetX(XMVector3Length(obbA->OBB_Z));								    //obbAの中心点からの長さ
	rB = obbA->prjLine(&obbA->OBB_Z, &obbB->OBB_X, &obbB->OBB_Y, &obbB->OBB_Z);			//obbBの中心点からの長さ
	L  = fabsf(XMVectorGetX(XMVector3Dot(betweenCenterPoint, XMVector3Normalize(obbA->OBB_Z))));//中心点間の長さ
	if (L > rA + rB)
	{
		return false;
	}

	rA = XMVectorGetX(XMVector3Length(obbB->OBB_X));								    //obbAの中心点からの長さ
	rB = obbA->prjLine(&obbB->OBB_X, &obbA->OBB_X, &obbA->OBB_Y, &obbA->OBB_Z);			//obbBの中心点からの長さ
	L  = fabsf(XMVectorGetX(XMVector3Dot(betweenCenterPoint, XMVector3Normalize(obbB->OBB_X))));//中心点間の長さ
	if (L > rA + rB)
	{
		return false;
	}

	rA = XMVectorGetX(XMVector3Length(obbB->OBB_Y));								    //obbAの中心点からの長さ
	rB = obbA->prjLine(&obbB->OBB_Y, &obbA->OBB_X, &obbA->OBB_Y, &obbA->OBB_Z);			//obbBの中心点からの長さ
	L  = fabsf(XMVectorGetX(XMVector3Dot(betweenCenterPoint, XMVector3Normalize(obbB->OBB_Y))));//中心点間の長さ
	if (L > rA + rB)
	{
		return false;
	}

	rA = XMVectorGetX(XMVector3Length(obbB->OBB_Z));								    //obbAの中心点からの長さ
	rB = obbA->prjLine(&obbB->OBB_Z, &obbA->OBB_X, &obbA->OBB_Y, &obbA->OBB_Z);			//obbBの中心点からの長さ
	L  = fabsf(XMVectorGetX(XMVector3Dot(betweenCenterPoint, XMVector3Normalize(obbB->OBB_Z))));//中心点間の長さ
	if (L > rA + rB)
	{
		return false;
	}

	///////////////////////////////ここから外積軸による衝突判定//////////////////////////////////////

	XMVECTOR cross;
	//obbAのX軸とobbBのX軸の外積 : この外積を分離軸として計算を行う
	cross = XMVector3Normalize(XMVector3Cross(obbA->nX, obbB->nX));
	rA = obbA->prjLine(&cross, &obbA->OBB_Y, &obbA->OBB_Z);				//obbAの長さ
	rB = obbA->prjLine(&cross, &obbB->OBB_Y, &obbB->OBB_Z);				//obbBの長さ
	L  = fabsf(XMVectorGetX(XMVector3Dot(betweenCenterPoint, cross)));	//分離軸に投影した中心点間の距離
	if (L > rA + rB)
	{
		return false;
	}

	//obbAのX軸とobbBのY軸の外積 : この外積を分離軸として計算を行う
	cross = XMVector3Normalize(XMVector3Cross(obbA->nX, obbB->nY));
	rA = obbA->prjLine(&cross, &obbA->OBB_Y, &obbA->OBB_Z);				//obbAの長さ
	rB = obbA->prjLine(&cross, &obbB->OBB_X, &obbB->OBB_Z);				//obbBの長さ
	L  = fabsf(XMVectorGetX(XMVector3Dot(betweenCenterPoint, cross)));	//分離軸に投影した中心点間の距離
	if (L > rA + rB)
	{
		return false;
	}

	//obbAのX軸とobbBのZ軸の外積 : この外積を分離軸として計算を行う
	cross = XMVector3Normalize(XMVector3Cross(obbA->nX, obbB->nZ));
	rA = obbA->prjLine(&cross, &obbA->OBB_Y, &obbA->OBB_Z);				//obbAの長さ
	rB = obbA->prjLine(&cross, &obbB->OBB_X, &obbB->OBB_Y);				//obbBの長さ
	L  = fabsf(XMVectorGetX(XMVector3Dot(betweenCenterPoint, cross)));//分離軸に投影した中心点間の距離
	if (L > rA + rB)
	{
		return false;
	}

	//obbAのY軸とobbBのX軸の外積 : この外積を分離軸として計算を行う
	cross = XMVector3Normalize(XMVector3Cross(obbA->nY, obbB->nX));
	rA = obbA->prjLine(&cross, &obbA->OBB_X, &obbA->OBB_Z);				//obbAの長さ
	rB = obbA->prjLine(&cross, &obbB->OBB_Y, &obbB->OBB_Z);				//obbBの長さ
	L  = fabsf(XMVectorGetX(XMVector3Dot(betweenCenterPoint, cross)));//分離軸に投影した中心点間の距離
	if (L > rA + rB)
	{
		return false;
	}

	//obbAのY軸とobbBのY軸の外積 : この外積を分離軸として計算を行う
	cross = XMVector3Normalize(XMVector3Cross(obbA->nY, obbB->nY));
	rA = obbA->prjLine(&cross, &obbA->OBB_X, &obbA->OBB_Z);				//obbAの長さ
	rB = obbA->prjLine(&cross, &obbB->OBB_X, &obbB->OBB_Z);				//obbBの長さ
	L  = fabsf(XMVectorGetX(XMVector3Dot(betweenCenterPoint, cross)));	//分離軸に投影した中心点間の距離
	if (L > rA + rB)
	{
		return false;
	}

	//obbAのY軸とobbBのZ軸の外積 : この外積を分離軸として計算を行う
	cross = XMVector3Normalize(XMVector3Cross(obbA->nY, obbB->nZ));
	rA = obbA->prjLine(&cross, &obbA->OBB_X, &obbA->OBB_Z);				//obbAの長さ
	rB = obbA->prjLine(&cross, &obbB->OBB_X, &obbB->OBB_Y);				//obbBの長さ
	L  = fabsf(XMVectorGetX(XMVector3Dot(betweenCenterPoint, cross)));	//分離軸に投影した中心点間の距離
	if (L > rA + rB)
	{
		return false;
	}

	//obbAのZ軸とobbBのX軸の外積 : この外積を分離軸として計算を行う
	cross = XMVector3Normalize(XMVector3Cross(obbA->nZ, obbB->nX));
	rA = obbA->prjLine(&cross, &obbA->OBB_X, &obbA->OBB_Y);				//obbAの長さ
	rB = obbA->prjLine(&cross, &obbB->OBB_Y, &obbB->OBB_Z);				//obbBの長さ
	L  = fabsf(XMVectorGetX(XMVector3Dot(betweenCenterPoint, cross)));	//分離軸に投影した中心点間の距離
	if (L > rA + rB)
	{
		return false;
	}

	//obbAのZ軸とobbBのY軸の外積 : この外積を分離軸として計算を行う
	cross = XMVector3Normalize(XMVector3Cross(obbA->nZ, obbB->nY));
	rA = obbA->prjLine(&cross, &obbA->OBB_X, &obbA->OBB_Y);				//obbAの長さ
	rB = obbA->prjLine(&cross, &obbB->OBB_X, &obbB->OBB_Z);				//obbBの長さ
	L  = fabsf(XMVectorGetX(XMVector3Dot(betweenCenterPoint, cross)));	//分離軸に投影した中心点間の距離
	if (L > rA + rB)
	{
		return false;
	}

	//obbAのZ軸とobbBのZ軸の外積 : この外積を分離軸として計算を行う
	cross = XMVector3Normalize(XMVector3Cross(obbA->nZ, obbB->nZ));
	rA = obbA->prjLine(&cross, &obbA->OBB_X, &obbA->OBB_Y);
	rB = obbA->prjLine(&cross, &obbB->OBB_X, &obbB->OBB_Y);
	L  = fabsf(XMVectorGetX(XMVector3Dot(betweenCenterPoint, cross)));//分離軸に投影した中心点間の距離
	if (L > rA + rB)
	{
		return false;
	}

	//中心点間の距離が一回もrA + rB以上にならなかったので当たっている
	return true;
}

bool Collider::IsHitOBB_Sphere(OBBCollider* obb, SphereCollider* sphere)
{
	XMFLOAT3 obbPos = obb->pColObject_->GetTransform().position_;
	XMFLOAT3 spherePos = sphere->pColObject_->GetTransform().position_;

	XMVECTOR betweenCenterPoint = XMLoadFloat3(&spherePos) - XMLoadFloat3(&obbPos);

	obb->CalcAxisVec();
	
	float L;
	double rA = XMVectorGetX(XMVector3Length(obb->OBB_X));								    //obbの中心点からの長さ
	double rB = sphere->size_.x;															//sphereの中心点からの長さ
	L = fabsf(XMVectorGetX(XMVector3Dot(betweenCenterPoint, XMVector3Normalize(obb->OBB_X))));//中心点間の長さ
	if (L > rA + rB)
	{
		return false;
	}

	rA = XMVectorGetX(XMVector3Length(obb->OBB_Y));								    //obbAの中心点からの長さ
	rB = sphere->size_.x;															//obbBの中心点からの長さ
	L = fabsf(XMVectorGetX(XMVector3Dot(betweenCenterPoint, XMVector3Normalize(obb->OBB_Y))));//中心点間の長さ
	if (L > rA + rB)
	{
		return false;
	}

	rA = XMVectorGetX(XMVector3Length(obb->OBB_Z));								    //obbAの中心点からの長さ
	rB = sphere->size_.x;															//obbBの中心点からの長さ
	L = fabsf(XMVectorGetX(XMVector3Dot(betweenCenterPoint, XMVector3Normalize(obb->OBB_Z))));//中心点間の長さ
	if (L > rA + rB)
	{
		return false;
	}

#if false
	rA = XMVectorGetX(XMVector3Length(obbB->OBB_X));								    //obbAの中心点からの長さ
	rB = sphere->size_.x;														//obbBの中心点からの長さ
	L = fabs(XMVectorGetX(XMVector3Dot(betweenCenterPoint, XMVector3Normalize(obbB->OBB_X))));//中心点間の長さ
	if (L > rA + rB)
	{
		return false;
	}

	rA = XMVectorGetX(XMVector3Length(obbB->OBB_Y));								    //obbAの中心点からの長さ
	rB = obbA->prjLine(&obbB->OBB_Y, &obbA->OBB_X, &obbA->OBB_Y, &obbA->OBB_Z);			//obbBの中心点からの長さ
	L = fabs(XMVectorGetX(XMVector3Dot(betweenCenterPoint, XMVector3Normalize(obbB->OBB_Y))));//中心点間の長さ
	if (L > rA + rB)
	{
		return false;
	}

	rA = XMVectorGetX(XMVector3Length(obbB->OBB_Z));								    //obbAの中心点からの長さ
	rB = obbA->prjLine(&obbB->OBB_Z, &obbA->OBB_X, &obbA->OBB_Y, &obbA->OBB_Z);			//obbBの中心点からの長さ
	L = fabs(XMVectorGetX(XMVector3Dot(betweenCenterPoint, XMVector3Normalize(obbB->OBB_Z))));//中心点間の長さ
	if (L > rA + rB)
	{
		return false;
	}

	///////////////////////////////ここから外積軸による衝突判定//////////////////////////////////////

	XMVECTOR cross;
	//obbAのX軸とobbBのX軸の外積 : この外積を分離軸として計算を行う
	cross = XMVector3Normalize(XMVector3Cross(obbA->nX, obbB->nX));
	rA = obbA->prjLine(&cross, &obbA->OBB_Y, &obbA->OBB_Z);				//obbAの長さ
	rB = obbA->prjLine(&cross, &obbB->OBB_Y, &obbB->OBB_Z);				//obbBの長さ
	L = fabs(XMVectorGetX(XMVector3Dot(betweenCenterPoint, cross)));	//分離軸に投影した中心点間の距離
	if (L > rA + rB)
	{
		return false;
	}

	//obbAのX軸とobbBのY軸の外積 : この外積を分離軸として計算を行う
	cross = XMVector3Normalize(XMVector3Cross(obbA->nX, obbB->nY));
	rA = obbA->prjLine(&cross, &obbA->OBB_Y, &obbA->OBB_Z);				//obbAの長さ
	rB = obbA->prjLine(&cross, &obbB->OBB_X, &obbB->OBB_Z);				//obbBの長さ
	L = fabs(XMVectorGetX(XMVector3Dot(betweenCenterPoint, cross)));	//分離軸に投影した中心点間の距離
	if (L > rA + rB)
	{
		return false;
	}

	//obbAのX軸とobbBのZ軸の外積 : この外積を分離軸として計算を行う
	cross = XMVector3Normalize(XMVector3Cross(obbA->nX, obbB->nZ));
	rA = obbA->prjLine(&cross, &obbA->OBB_Y, &obbA->OBB_Z);				//obbAの長さ
	rB = obbA->prjLine(&cross, &obbB->OBB_X, &obbB->OBB_Y);				//obbBの長さ
	L = fabs(XMVectorGetX(XMVector3Dot(betweenCenterPoint, cross)));//分離軸に投影した中心点間の距離
	if (L > rA + rB)
	{
		return false;
	}

	//obbAのY軸とobbBのX軸の外積 : この外積を分離軸として計算を行う
	cross = XMVector3Normalize(XMVector3Cross(obbA->nY, obbB->nX));
	rA = obbA->prjLine(&cross, &obbA->OBB_X, &obbA->OBB_Z);				//obbAの長さ
	rB = obbA->prjLine(&cross, &obbB->OBB_Y, &obbB->OBB_Z);				//obbBの長さ
	L = fabs(XMVectorGetX(XMVector3Dot(betweenCenterPoint, cross)));//分離軸に投影した中心点間の距離
	if (L > rA + rB)
	{
		return false;
	}

	//obbAのY軸とobbBのY軸の外積 : この外積を分離軸として計算を行う
	cross = XMVector3Normalize(XMVector3Cross(obbA->nY, obbB->nY));
	rA = obbA->prjLine(&cross, &obbA->OBB_X, &obbA->OBB_Z);				//obbAの長さ
	rB = obbA->prjLine(&cross, &obbB->OBB_X, &obbB->OBB_Z);				//obbBの長さ
	L = fabs(XMVectorGetX(XMVector3Dot(betweenCenterPoint, cross)));	//分離軸に投影した中心点間の距離
	if (L > rA + rB)
	{
		return false;
	}

	//obbAのY軸とobbBのZ軸の外積 : この外積を分離軸として計算を行う
	cross = XMVector3Normalize(XMVector3Cross(obbA->nY, obbB->nZ));
	rA = obbA->prjLine(&cross, &obbA->OBB_X, &obbA->OBB_Z);				//obbAの長さ
	rB = obbA->prjLine(&cross, &obbB->OBB_X, &obbB->OBB_Y);				//obbBの長さ
	L = fabs(XMVectorGetX(XMVector3Dot(betweenCenterPoint, cross)));	//分離軸に投影した中心点間の距離
	if (L > rA + rB)
	{
		return false;
	}

	//obbAのZ軸とobbBのX軸の外積 : この外積を分離軸として計算を行う
	cross = XMVector3Normalize(XMVector3Cross(obbA->nZ, obbB->nX));
	rA = obbA->prjLine(&cross, &obbA->OBB_X, &obbA->OBB_Y);				//obbAの長さ
	rB = obbA->prjLine(&cross, &obbB->OBB_Y, &obbB->OBB_Z);				//obbBの長さ
	L = fabs(XMVectorGetX(XMVector3Dot(betweenCenterPoint, cross)));	//分離軸に投影した中心点間の距離
	if (L > rA + rB)
	{
		return false;
	}

	//obbAのZ軸とobbBのY軸の外積 : この外積を分離軸として計算を行う
	cross = XMVector3Normalize(XMVector3Cross(obbA->nZ, obbB->nY));
	rA = obbA->prjLine(&cross, &obbA->OBB_X, &obbA->OBB_Y);				//obbAの長さ
	rB = obbA->prjLine(&cross, &obbB->OBB_X, &obbB->OBB_Z);				//obbBの長さ
	L = fabs(XMVectorGetX(XMVector3Dot(betweenCenterPoint, cross)));	//分離軸に投影した中心点間の距離
	if (L > rA + rB)
	{
		return false;
	}

	//obbAのZ軸とobbBのZ軸の外積 : この外積を分離軸として計算を行う
	cross = XMVector3Normalize(XMVector3Cross(obbA->nZ, obbB->nZ));
	rA = obbA->prjLine(&cross, &obbA->OBB_X, &obbA->OBB_Y);
	rB = obbA->prjLine(&cross, &obbB->OBB_X, &obbB->OBB_Y);
	L = fabs(XMVectorGetX(XMVector3Dot(betweenCenterPoint, cross)));//分離軸に投影した中心点間の距離
	if (L > rA + rB)
	{
		return false;
	}

	//中心点間の距離が一回もrA + rB以上にならなかったので当たっている
#endif
	return true;
}
