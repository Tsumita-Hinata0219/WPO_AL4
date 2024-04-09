#include "PlayerReticle.h"



// 初期化処理
void PlayerReticle::Initialize()
{
	reticleModel_ = make_unique<Model>();
	reticleModel_->CreateFromObj("PlayerReticle", "Player");
	offset_ = { 0.0f, 0.0f, 10.0f };
	reticleWt_.Initialize();
	reticleWt_.translate = offset_;
	theta1_ = { 0.0f, 0.0f };
	theta1_ = { 0.0f, 1.0f };
	velocity_ = Vector2::zero;
	thetaMove_ = 0.01f;
}


// 更新処理
void PlayerReticle::Update(Camera* camera)
{
	// ワールドトランスフォームの初期化
	reticleWt_.UpdateMatrix();

	// 移動処理
	Move();

	// カメラに向ける
	CalcRotateToCamera(camera);


	if (KeysInput::PressKeys(DIK_LEFT)) {
		reticleWt_.translate.x -= 0.2f;
	}
	if (KeysInput::PressKeys(DIK_RIGHT)) {
		reticleWt_.translate.x += 0.2f;
	}

	Vector2 cont = GamePadInput::GetRStick();

#ifdef _DEBUG

	if (ImGui::TreeNode("PlayerReticle")) {

		ImGui::Text("WorldTransform");
		ImGui::DragFloat3("Rotate", &reticleWt_.rotate.x, 0.01f);
		ImGui::DragFloat3("Translate", &reticleWt_.translate.x, 0.01f);

		ImGui::Text("Controller = { %.2f, %.2f }", cont.x, cont.y);
		ImGui::Text("");
		ImGui::Text("Theta1 = { %.2f, %.2f }", theta1_.x, theta1_.y);
		ImGui::Text("Theta2 = { %.2f, %.2f }", theta2_.x, theta2_.y);
		ImGui::Text("Hr = { %.2f, %.2f }", hr.x, hr.y);
		ImGui::Text("Move = %.2f", thetaMove_);

		ImGui::TreePop();
	}

#endif // _DEBUG


}


// 描画処理
void PlayerReticle::Draw3D(Camera* camera)
{
	reticleModel_->Draw(reticleWt_, camera);
}


// 移動処理
void PlayerReticle::Move()
{
	// velocityは毎フレーム0で初期化
	velocity_ = Vector2::zero;

	// スティックを動かしたら
	//theta1_ = GamePadInput::GetRStick();
	if (GamePadInput::GetRStick().x <= 0.2f && GamePadInput::GetRStick().x >= -0.2f) {
		theta1_.x = 0.0f;
	}
	else {
		theta1_.x = GamePadInput::GetRStick().x;
	}

	if (GamePadInput::GetRStick().y <= 0.2f && GamePadInput::GetRStick().y >= -0.2f) {
		theta1_.y = 0.02f;
	}
	else {
		theta1_.y = GamePadInput::GetRStick().y;
	}

	// レティクルの座標の設定
	offset_.x = (sinf(theta1_.x));
	offset_.z = (sinf(theta1_.y));
	reticleWt_.translate = offset_ * 10.0f;
}


// カメラに向く処理
void PlayerReticle::CalcRotateToCamera(Camera* camera)
{
	// カメラとの差分
	Vector3 diffRotate = Normalize(reticleWt_.GetWorldPos() - camera->GetWorldPos());

	// 姿勢をカメラに向ける
	reticleWt_.rotate.y = atan2(diffRotate.x, diffRotate.z);
	float vecZ = sqrt((diffRotate.x * diffRotate.x) + (diffRotate.z * diffRotate.z));
	float height = -diffRotate.y;
	reticleWt_.rotate.x = atan2(height, vecZ);
}

