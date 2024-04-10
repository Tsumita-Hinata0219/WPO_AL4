#include "Player.h"
#include "GameManager.h"

void Player::Init()
{
	model_ = make_unique<Model>();
	model_->CreateFromObj("Player");
	bulletModel_ = make_unique<Model>();
	bulletModel_->CreateFromObj("PlayerBullet");

	wt_.Initialize();
	
	velocity_ = Vector3::zero;

	move_ = Vector3::zero;
	moveSpeed_ = 0.5f;

	this->size_ = 2.0f * wt_.scale;

	OBBCollider::SetID(ObjectId::Player);
	OBBCollider::SetCollosionAttribute(ObjectAttribute::Player);
	OBBCollider::SetCollisionMask(ObjectMask::Player);

	reticle_ = make_unique<PlayerReticle>();
	reticle_->SetPlayer(this);
	reticle_->SetParent(&wt_);
}

void Player::Update(BaseCamera* camera)
{
	wt_.UpdateMatrix();

	SettingOBB();

	Move();

	Attack();

	ReticleUpdate(camera);
}

void Player::Draw3D(BaseCamera* camera)
{
	model_->Draw(wt_, camera);
}

void Player::Draw2DF(BaseCamera* camera)
{
	camera;
}

void Player::OnCollision(uint32_t id)
{
	id;
}

void Player::onCollisionWithEnemy()
{
}

void Player::onCollisionWithEnemyBullet()
{
}

void Player::Move()
{
	move_ = Vector3::zero;
	velocity_ = Vector3::zero;

	if (KeysInput::PressKeys(DIK_W))
	{
		velocity_.y = moveSpeed_;
	};
	if (KeysInput::PressKeys(DIK_A))
	{
		velocity_.x = -moveSpeed_;
	};
	if (KeysInput::PressKeys(DIK_S))
	{
		velocity_.y = -moveSpeed_;
	};
	if (KeysInput::PressKeys(DIK_D))
	{
		velocity_.x = moveSpeed_;
	};

	if (GamePadInput::GetLStick().x <= -0.3f)
	{
		velocity_.x = -1.0f;
	}
	if (GamePadInput::GetLStick().x >= 0.3f)
	{
		velocity_.x = 1.0f;
	}
	if (GamePadInput::GetLStick().y <= -0.3f)
	{
		velocity_.y = -1.0f;
	}
	if (GamePadInput::GetLStick().y >= 0.3f)
	{
		velocity_.y = 1.0f;
	}

	if (velocity_.x != 0.0f || velocity_.y != 0.0f) {
		float length = Length({ velocity_.x, velocity_.y });
		velocity_.x /= length;
		velocity_.y /= length;	}

	wt_.translate += (velocity_ * moveSpeed_);
}

void Player::Attack()
{
	if (KeysInput::TriggerKey(DIK_SPACE)) {
		PushBackBulletList();
	}
	if (GamePadInput::TriggerButton(PadData::A)) {
		PushBackBulletList();
	}
}

void Player::SettingOBB()
{
	OBBCollider::SetSize(this->size_);
	OBBCollider::SetRotate(this->wt_.rotate);
}

void Player::PushBackBulletList()
{
	shared_ptr<PlayerBullet> newBullet = make_shared<PlayerBullet>();
	Vector3 newPos = wt_.GetWorldPos();
	Vector3 newVel = CalcDirection();
	newBullet->Init((*bulletModel_), newPos, newVel);
	gameScene_->AddPlayerBullets(newBullet);
}

Vector3 Player::CalcDirection()
{
	Vector3 reticlePos = reticle_->GetWorldPosition();
	Vector3 PlaToRet = Normalize(reticlePos - wt_.GetWorldPos());
	return PlaToRet * kBulletSpeed_;
}

void Player::ReticleUpdate(BaseCamera* camera)
{
	reticle_->Update(camera);
}
