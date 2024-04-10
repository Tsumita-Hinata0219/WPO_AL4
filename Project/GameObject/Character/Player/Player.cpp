#include "Player.h"

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

void Player::Update()
{
	wt_.UpdateMatrix();

	SettingOBB();

	Move();

	Attack();

	ReticleUpdate();
}

void Player::Draw3D(BaseCamera* camera)
{
	model_->Draw(wt_, camera);
}

void Player::Draw2DF(BaseCamera* camera)
{
	camera;
}

void Player::onCollisionWithEnemy()
{
}

void Player::onCollisionWithEnemyBullet()
{
}

void Player::Move()
{
}

void Player::Attack()
{
}

void Player::SettingOBB()
{
}

void Player::PushBackBulletList()
{
}

Vector3 Player::CalcDirection()
{
	return Vector3();
}

void Player::ReticleUpdate()
{
}
