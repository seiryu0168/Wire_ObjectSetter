#pragma once
#include<DirectXMath.h>
#include<algorithm>
//#include"Fbx.h"
using namespace DirectX;

namespace Math
{
	float Det(XMFLOAT3 a, XMFLOAT3 b, XMFLOAT3 c);
	/// <summary>
	/// �����Ɩʂ̔���
	/// </summary>
	/// <param name="start">�����̊J�n�ʒu</param>
	/// <param name="dir">�����̕���</param>
	/// <param name="v0">�ʂ��\�����钸�_1</param>
	/// <param name="v1">�ʂ��\�����钸�_2</param>
	/// <param name="v2">�ʂ��\�����钸�_3</param>
	/// <param name="dist">�J�n�ʒu����̋���</param>
	/// <param name="hitPos">�����Ƃ̏Փˈʒu(���[�J�����W)</param>
	/// <returns>�����������ǂ���</returns>
	bool Intersect(XMFLOAT3 start, XMFLOAT3 dir, XMFLOAT3 v0, XMFLOAT3 v1, XMFLOAT3 v2,float& dist, XMVECTOR& hitPos);
	
	/// <summary>
	/// �����Ɩʂ̔���
	/// </summary>
	/// <param name="rayCast">���C�̃f�[�^</param>
	/// <param name="v0">�ʂ��\�����钸�_1</param>
	/// <param name="v1">�ʂ��\�����钸�_2</param>
	/// <param name="v2">�ʂ��\�����钸�_3</param>
	/// <returns>�����������ǂ���</returns>
	//bool Intersect(RayCastData& rayCast, XMFLOAT3 v0, XMFLOAT3 v1, XMFLOAT3 v2);
	/// <summary>
	/// �@���Ƃ̊p�x�𒲂ׂ�
	/// </summary>
	/// <param name="vNormal">���ׂ����@��</param>
	/// <param name="vDir">����</param>
	/// <returns>�\���ǂ���</returns>
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
	//f1��m1�ŕό`������
	f1 = XMVector3TransformCoord(f1, m1);
	return f1;
}
//----------------------------------------------------------------------------------------
inline XMVECTOR operator*(XMVECTOR f1, XMMATRIX m1)
{
	//f1��m1�ŕό`
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
