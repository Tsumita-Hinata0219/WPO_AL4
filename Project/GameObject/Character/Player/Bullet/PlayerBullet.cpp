#include "PlayerBullet.h"

void PlayerBullet::Init(Model& modelHD, Vector3 pos, Vector3 vel)
{
	model_ = make_unique<Model>();
	(*model_) = modelHD;

	wt_.Initialize();
	wt_.translate = pos;

	velocity_ = Vector3::zero;

	wt_.rotate.y = atan2(vel.x, vel.z);
	float velZ = sqrt((vel.x * vel.x) + (vel.z * vel.z));
	float height = -vel.y;
	wt_.rotate.x = atan2(height, velZ);

	move_ = vel;

	size_ = 2.0f * wt_.scale;

	kLifeTime_ = 60 * 5;
	lifeTimer_ = kLifeTime_;

	isDead_ = false;

	OBBCollider::SetID(ObjectId::PlayerBullet);
	OBBCollider::SetCollosionAttribute(ObjectAttribute::Player);
	OBBCollider::SetCollisionMask(ObjectMask::Player);
}

void PlayerBullet::Update()
{
	wt_.UpdateMatrix();

	SettingOBB();

	Move();

	UpdateLifeTimer();
}

void PlayerBullet::Draw(BaseCamera* camera)
{
	model_->Draw(wt_, camera);
}

void PlayerBullet::OnCollision(uint32_t id)
{
	id;
}

void PlayerBullet::onCollisionWithEnemy()
{
}

void PlayerBullet::onCollisionWithEnemyBullet()
{
}

void PlayerBullet::Move()
{
	velocity_ = move_;
	wt_.translate += velocity_;
}

void PlayerBullet::SettingOBB()
{
	OBBCollider::SetSize(this->size_);
	OBBCollider::SetRotate(this->wt_.rotate);
}

void PlayerBullet::UpdateLifeTimer()
{
	lifeTimer_--;
	if (lifeTimer_ <= 0.0f) {
		lifeTimer_ = 0;
		isDead_ = true;
	}
}
