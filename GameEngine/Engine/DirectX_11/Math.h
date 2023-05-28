#pragma once
#include<DirectXMath.h>
#include<algorithm>
//#include"Fbx.h"
using namespace DirectX;

namespace Math
{
	float Det(XMFLOAT3 a, XMFLOAT3 b, XMFLOAT3 c);
	/// <summary>
	/// 線分と面の判定
	/// </summary>
	/// <param name="start">線分の開始位置</param>
	/// <param name="dir">線分の方向</param>
	/// <param name="v0">面を構成する頂点1</param>
	/// <param name="v1">面を構成する頂点2</param>
	/// <param name="v2">面を構成する頂点3</param>
	/// <param name="dist">開始位置からの距離</param>
	/// <param name="hitPos">線分との衝突位置(ローカル座標)</param>
	/// <returns>当たったかどうか</returns>
	bool Intersect(XMFLOAT3 start, XMFLOAT3 dir, XMFLOAT3 v0, XMFLOAT3 v1, XMFLOAT3 v2,float& dist, XMVECTOR& hitPos);
	
	/// <summary>
	/// 線分と面の判定
	/// </summary>
	/// <param name="rayCast">レイのデータ</param>
	/// <param name="v0">面を構成する頂点1</param>
	/// <param name="v1">面を構成する頂点2</param>
	/// <param name="v2">面を構成する頂点3</param>
	/// <returns>当たったかどうか</returns>
	//bool Intersect(RayCastData& rayCast, XMFLOAT3 v0, XMFLOAT3 v1, XMFLOAT3 v2);
	/// <summary>
	/// 法線との角度を調べる
	/// </summary>
	/// <param name="vNormal">調べたい法線</param>
	/// <param name="vDir">方向</param>
	/// <returns>表かどうか</returns>
	bool IsFrontSurface(XMVECTOR vNormal, XMVECTOR vDir);

}

template <typename T>
inline T Clamp(const T& in, const T& low, const T& high)
{
	T out = std::min<T>(std::max<T>(in, low), high);
	return out;
}
//template<typename T>
//inline bool IsPositive(const T& in)
//{
//	if (in > 0)
//	{
//		return true;
//	}
//	return false;
//}
//template<typename T>
//inline bool IsNegative(const T& in)
//{
//	if (in < 0)
//	{
//		return true;
//	}
//	return false;
//}
//----------------------------------------------------------------------------------------
inline XMVECTOR operator*=(XMVECTOR& f1, const XMMATRIX& m1)
{
	//f1をm1で変形させる
	f1 = XMVector3TransformCoord(f1, m1);
	return f1;
}
//----------------------------------------------------------------------------------------
inline XMVECTOR operator*(XMVECTOR f1, XMMATRIX m1)
{
	//f1をm1で変形
	return XMVector3TransformCoord(f1, m1);
}
//----------------------------------------------------------------------------------------
inline XMVECTOR operator-(const XMFLOAT3& f1, const XMFLOAT3& f2)
{
	return XMLoadFloat3(&f1) - XMLoadFloat3(&f2);
}
//----------------------------------------------------------------------------------------
inline XMVECTOR operator+(const XMFLOAT3& f1, const XMFLOAT3& f2)
{
	return XMLoadFloat3(&f1) + XMLoadFloat3(&f2);
}
//----------------------------------------------------------------------------------------
inline XMFLOAT3 operator*(const XMFLOAT3& f1, const float f)
{
	return XMFLOAT3(f1.x * f, f1.y * f, f1.z * f);
}
//----------------------------------------------------------------------------------------
inline XMFLOAT3 operator*(const float f, const XMFLOAT3& f1)
{
	return XMFLOAT3(f1.x * f, f1.y * f, f1.z * f);
}
//----------------------------------------------------------------------------------------
inline XMFLOAT3 operator/(const XMFLOAT3& f1, const float f)
{
	return XMFLOAT3(f1.x / f, f1.y / f, f1.z / f);
}
//----------------------------------------------------------------------------------------
inline float VectorDot(const XMVECTOR& v1, const XMVECTOR& v2)
{
	return XMVectorGetX(XMVector3Dot(v1, v2));
}
//----------------------------------------------------------------------------------------
inline float VectorLength(const XMVECTOR& v1)
{
	return XMVectorGetX(XMVector3Length(v1));
}
//----------------------------------------------------------------------------------------
inline	XMFLOAT3 StoreFloat3(const XMVECTOR& v1)
{
	XMFLOAT3 ret;
	XMStoreFloat3(&ret, v1);
	return ret;
}
//----------------------------------------------------------------------------------------
