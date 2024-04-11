#pragma once

#include "WinApp.h"
#include "MyMath.h"
#include "CreateResource.h"
#include "WorldTransform.h"



// 定数バッファ
struct TransformationViewMatrix {
	Matrix4x4 view;
	Matrix4x4 viewProjection;
	Matrix4x4 orthoGraphic;
	Vector3 cameraPosition;
};


class BaseCamera {

public:

	/// <summary>
	/// コンストラクタ
	/// </summary>
	BaseCamera() {};

	/// <summary>
	/// デストラクタ
	/// </summary>
	~BaseCamera() {};

	/// <summary>
	/// 初期化処理
	/// </summary>
	void Initialize(Vector3 initRotate = { 0.0f, 0.0f, 0.0f }, Vector3 initTranslate = { 0.0f, 0.0f, -5.0f });

	/// <summary>
	/// 行列の更新処理
	/// </summary>
	void UpdateMatrix();

#pragma region Get 

	/// <summary>
	/// ワールド座標の取得
	/// </summary>
	Vector3 GetWorldPos();

#pragma endregion 


private:

	/// <summary>
	/// 定数バッファの生成
	/// </summary>
	void CreateBuffer();

	/// <summary>
	/// マッピング処理
	/// </summary>
	void Map();

	/// <summary>
	/// マッピング終了
	/// </summary>
	void UnMap();

	/// <summary>
	/// 行列の計算・転送
	/// </summary>
	void TransferMatrix();


public:

#pragma region ビュー行列の設定

	// ワールドトランスフォーム
	WorldTransform worldTransform_;

	Matrix4x4 rotateMat{};
	Matrix4x4 translateMat{};
	Matrix4x4 matWorld{};

#pragma endregion 

#pragma region 射影行列の設定

	// 垂直方向視野角
	float fov = 0.45f;

	// ビューポートのアスペクト比
	float aspectRatio = float(WinApp::kWindowWidth) / float(WinApp::kWindowHeight);

	// 深度限界 (手前側)
	float nearZ = 0.1f;

	// 深度限界 (奥側)
	float farZ = 1000.0f;

#pragma endregion 

	// ビュー行列
	Matrix4x4 matView{};

	// 射影行列
	Matrix4x4 matProjection{};

	// 正射影行列
	Matrix4x4 orthoGraphicMat{};

	// ビューポート行列
	Matrix4x4 matViewPort{};

	// ビュープロジェクションビューポート合成行列
	Matrix4x4 matViewProjectionViewPort{};

	// 合成行列の逆行列
	Matrix4x4 matInverseVPV{};

	// 定数バッファ
	ComPtr<ID3D12Resource> constBuffer;

	// マッピング済みアドレス
	TransformationViewMatrix* constMap = nullptr;
};

