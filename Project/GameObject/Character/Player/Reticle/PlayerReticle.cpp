#include "PlayerReticle.h"
#include "Player/Player.h"

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

	moveSpeed_ = 0.3f;
}

void PlayerReticle::Update(BaseCamera* camera)
{
	camera;
	wt3D_.UpdateMatrix();
	wt2D_.UpdateMatrix();

	Move();

	/*offsetVec_ = Vector3::oneZ;
	offsetVec_ = TransformNormal(offsetVec_, player_->GetWorldTransform().matWorld);
	offsetVec_ = Normalize(offsetVec_) * kDistReticle_;*/

}

void PlayerReticle::Draw3D(BaseCamera* camera)
{
	model_->Draw(wt3D_, camera);
}

void PlayerReticle::Draw2DF(BaseCamera* camera)
{
	sprite_->Draw(reticleTexHD_, wt2D_, camera);
}

void PlayerReticle::Move()
{
	move_ = Vector3::zero;
	velocity_ = Vector3::zero;

	if (KeysInput::PressKeys(DIK_UP))
	{
		velocity_.y = moveSpeed_;
	};
	if (KeysInput::PressKeys(DIK_LEFT))
	{
		velocity_.x = -moveSpeed_;
	};
	if (KeysInput::PressKeys(DIK_DOWN))
	{
		velocity_.y = -moveSpeed_;
	};
	if (KeysInput::PressKeys(DIK_RIGHT))
	{
		velocity_.x = moveSpeed_;
	};

	if (GamePadInput::GetRStick().x <= -0.3f)
	{
		velocity_.x = -1.0f;
	}
	if (GamePadInput::GetRStick().x >= 0.3f)
	{
		velocity_.x = 1.0f;
	}
	if (GamePadInput::GetRStick().y <= -0.3f)
	{
		velocity_.y = -1.0f;
	}
	if (GamePadInput::GetRStick().y >= 0.3f)
	{
		velocity_.y = 1.0f;
	}

	if (velocity_.x != 0.0f || velocity_.y != 0.0f) {
		float length = Length({ velocity_.x, velocity_.y });
		velocity_.x /= length;
		velocity_.y /= length;
	}

	wt3D_.translate += (velocity_ * moveSpeed_);
}
