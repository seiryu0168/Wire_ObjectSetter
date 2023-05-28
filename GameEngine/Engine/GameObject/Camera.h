#pragma once
#include "../DirectX_11/Direct3D.h"
#include <DirectXMath.h>

using namespace DirectX;

//-----------------------------------------------------------
//カメラ
//-----------------------------------------------------------
namespace Camera
{
	//初期化（プロジェクション行列作成）
	void Initialize(float width, float height);

	//更新（ビュー行列作成）
	void Update();

	//視点（カメラの位置）を設定
	void SetPosition(XMVECTOR position);

	//焦点（見る位置）を設定
	void SetTarget(XMVECTOR target);

	//カメラの上方向ベクトルを設定
	void SetUpVector(XMVECTOR upVector);

	/// <summary>
	/// プロジェクション行列を設定
	/// </summary>
	/// <param name="aov">画角</param>
	/// <param name="aspectRadio">アスペクト比</param>
	/// <param name="nearZ">ニアクリッピング</param>
	/// <param name="farZ">ファークリッピング</param>
	void SetProjection(float aov, float aspectRadio, float nearZ, float farZ);

	/// <summary>
	/// 画角設定
	/// </summary>
	/// <param name="aov">画角</param>
	void SetAOV(float aov);

	/// <summary>
	/// アスペクト比設定
	/// </summary>
	/// <param name="aspect">アスペクト比</param>
	void SetAspectRadio(float aspect);

	/// <summary>
	/// クリッピング範囲設定
	/// </summary>
	/// <param name="nearZ">ニアクリッピング</param>
	/// <param name="farZ">ファークリッピング</param>
	void SetClipping(float nearZ, float farZ);

	//視点(見える位置)を取得
	XMFLOAT3 GetPosition();

	//焦点取得
	XMVECTOR GetTarget();

	//ビュー行列を取得
	XMMATRIX GetViewMatrix();

	//ビルボード用回転行列
	XMMATRIX GetBillBoardMatrix();

	//プロジェクション行列を取得
	XMMATRIX GetProjectionMatrix();

};