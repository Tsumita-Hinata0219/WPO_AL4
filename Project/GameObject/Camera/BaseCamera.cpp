#include "BaseCamera.h"



// 初期化処理
void BaseCamera::Initialize(Vector3 initRotate, Vector3 initTranslate)
{
	worldTransform_.Initialize();
	worldTransform_.scale = Vector3::one;
	worldTransform_.rotate = initRotate;
	worldTransform_.translate = initTranslate;

	CreateBuffer();
	Map();
	UpdateMatrix();
}


// 行列の更新処理
void BaseCamera::UpdateMatrix()
{
	rotateMat = MakeRotateXYZMatrix(worldTransform_.rotate);
	translateMat = MakeTranslateMatrix(worldTransform_.translate);
	matWorld = MakeAffineMatrix(worldTransform_.scale, worldTransform_.rotate, worldTransform_.translate);

	matView = Inverse(translateMat) * Inverse(rotateMat);
	matProjection = MakePerspectiveFovMatrix(fov, aspectRatio, nearZ, farZ);

	orthoGraphicMat = MakeOrthographicMatrix(0.0f, 0.0f, float(WinApp::kWindowWidth), float(WinApp::kWindowHeight), 0.0f, 100.0f);

	matViewPort = MakeViewportMatrix(0.0f, 0.0f, float(WinApp::kWindowWidth), float(WinApp::kWindowHeight), 0.0f, 1.0f);
	matViewProjectionViewPort = matView * (matProjection * matViewPort);
	matInverseVPV = Inverse(matViewProjectionViewPort);

	TransferMatrix();
}


// ワールド座標の取得
Vector3 BaseCamera::GetWorldPos()
{
	return worldTransform_.GetWorldPos();
}


// 定数バッファの生成
void BaseCamera::CreateBuffer()
{
	CreateResource::CreateBufferResource(sizeof(TransformationViewMatrix), constBuffer);

	// constBuffer が nullptr の場合はエラーログを出力してアサーションでプログラムを停止させる
	if (!constBuffer) {
		assert(false && "Failed to create constBuffer.");
	}
}


// マッピング処理
void BaseCamera::Map()
{
	// constBuffer が nullptr の場合はエラーハンドリング
	if (!constBuffer) {
		// エラーログなどを出力するか、適切な対処を行う
		// この例ではエラーログを出力し、アサーションでプログラムを停止させる
		assert(false && "constBuff_ is nullptr. Make sure to create constBuffer before calling Map.");
		return;
	}

	HRESULT result = constBuffer->Map(0, nullptr, reinterpret_cast<void**>(&constMap));
	assert(SUCCEEDED(result));
}


// マッピング終了
void BaseCamera::UnMap()
{
	constBuffer->Unmap(0, nullptr);
}


// 行列の計算・転送
void BaseCamera::TransferMatrix()
{
	Map();

	constMap->view = matView;
	constMap->viewProjection = matProjection;
	constMap->orthoGraphic = orthoGraphicMat;
	constMap->cameraPosition = worldTransform_.translate;

	UnMap();
}
