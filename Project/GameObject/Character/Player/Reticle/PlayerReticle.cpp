#include "PlayerReticle.h"

void PlayerReticle::Init(Model& modelHD, Vector3 initTranslate)
{
	model_ = make_unique<Model>();
	(*model_) = modelHD;

	wt3D_.Initialize();
	wt3D_.translate = initTranslate;

	offsetVec_ = Vector3::oneZ;

	reticleTexHD_ = TextureManager::LoadTexture("PlayerReticle.png");

	size_ = { 64.0f, 64.0f };

	sprite_ = make_unique<Sprite>();
	sprite_->Initialize(size_);

	wt2D_.Initialize();
	spritePos_ = WinApp::WindowSize() / 2;
	wt2D_.translate = { spritePos_.x, spritePos_.y, 0.0f };

	move_ = Vector3::zero;
	velocity_ = Vector3::zero;
}

void PlayerReticle::Update(BaseCamera* camera)
{
	camera;

}

void PlayerReticle::Draw3D(BaseCamera* camera)
{
	model_->Draw(wt3D_, camera);
}

void PlayerReticle::Draw2DF(BaseCamera* camera)
{
	sprite_->Draw(reticleTexHD_, wt2D_, camera);
}
