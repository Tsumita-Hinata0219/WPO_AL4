#include "Ground.h"



// インスタンスの取得
Ground* Ground::GetInstance() 
{
	static Ground instance;
	return&instance;
}


// 初期化処理
void Ground::Initialize() 
{
	this->normalMapTexHD_ = TextureManager::LoadTexture("normalMap.png");
	this->model_ = make_unique<Model>();
	this->model_->CreateFromObj("Ground");
	this->model_->SetModelDrawType(PhongNormalMap);
	this->model_->SetNormalMapTex(normalMapTexHD_);
	this->worldTransform_.Initialize();
	this->worldTransform_.scale = { 500.0f, 1.0f, 500.0f };
	this->worldTransform_.translate.y = -10.0f;
	this->color_ = { 0.5f, 0.5f, 0.5f, 1.0f };
	this->light_.color = { 1.0f, 1.0f, 1.0f, 1.0f };
	this->light_.direction = { 0.0f, 1.0f, 1.0f };
	this->light_.intensity = 1.0f;
	this->light_.sininess = 100.0f;
	this->light_.enableLightting = true;
}


// 更新処理
void Ground::Update() 
{
	// ワールド座標の更新
	this->worldTransform_.UpdateMatrix();

	// カラーの設定
	this->model_->SetColor(this->color_);

	// ライトの設定
	this->model_->SetDirectionalLight(this->light_);

	if (ImGui::TreeNode("Ground")) {

		ImGui::ColorEdit4("color", &color_.x);
		ImGui::ColorEdit4("LightColor", &light_.color.x);
		ImGui::DragFloat3("LightDirection", &light_.direction.x, 0.01f, -1.0f, 1.0f);
		ImGui::DragFloat("LightIntensity", &light_.intensity, 0.01f, -1.0f, 1.0f);
		ImGui::DragFloat("LightSininess", &light_.sininess, 0.1f, 0.0f, 200.0f);
		ImGui::TreePop();
	}
}


// 描画処理
void Ground::Draw(BaseCamera* camera)
{
	this->model_->Draw(this->worldTransform_, camera);
}

