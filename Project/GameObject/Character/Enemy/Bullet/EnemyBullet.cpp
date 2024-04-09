#include "EnemyBullet.h"

// 初期化処理
void EnemyBullet::Initialize(Vector3 pos, Vector3 vel)
{
	// モデルの初期化
	model_.CreateFromObj("bullet", {}, worldTransform_);

	// ワールドトランスフォームの初期化
	worldTransform_.Initialize();
	worldTransform_.translate = pos;

	// 加算速度
	velocity_ = Vector3::zero;

	// Y軸周り角度(θy)
	worldTransform_.rotate.y = std::atan2(vel.x, vel.z);
	float velZ = std::sqrt((vel.x * vel.x) + (vel.z * vel.z));
	float height = -vel.y;
	// X軸周り角度(θx)
	worldTransform_.rotate.x = std::atan2(height, velZ);

	// 移動量の設定
	move_ = vel;

	// サイズ
	this->size_ = 2.0f * worldTransform_.scale;

	float s = 0.2f;
	worldTransform_.scale.x = s;
	worldTransform_.scale.y = s;
	worldTransform_.scale.z = s;

	// 寿命
	kLifeTimer_ = 60 * 5;
	lifeTimer_ = kLifeTimer_;

	// コライダー
	//OBBCollider::SetID(ObjectBit::PlayerBullet);
	//OBBCollider::SetCollosionAttribute(ObjectFileter::Player);
	//OBBCollider::SetCollisionMask(ObjectFileter::Player);
}

// 初期化処理
void EnemyBullet::InitializeMortar(Vector3 pos, Vector3 pPos)
{
	// モデルの初期化
	model_.CreateFromObj("bullet", {}, worldTransform_);

	// ワールドトランスフォームの初期化
	worldTransform_.Initialize();
	worldTransform_.translate = pos;

	// ワールドトランスフォームの初期化
	tmpWorldTransform_.Initialize();
	tmpWorldTransform_.translate = pPos;
	
	flame = 0;

	// 加算速度
	velocity_ = Vector3::zero;

	distance_ = Vector3::zero;
	distance_ = pPos - pos;

	// サイズ
	this->size_ = 2.0f * worldTransform_.scale;

	// 寿命
	bombTimer_ = 30;
	kLifeTimer_ = 60 * 2 + bombTimer_;
	lifeTimer_ = kLifeTimer_;

	// コライダー
	OBBCollider::SetID(ObjectId::EnemyBullet);
	OBBCollider::SetCollosionAttribute(ObjectAttribute::Enemy);
	OBBCollider::SetCollisionMask(ObjectMask::Player);
}


// 更新処理
void EnemyBullet::Update()
{
	// OBBの設定
	SettingOBBProperties();

	// 移動処理
	Move();

	// 寿命の処理
	UpdateLifeTimer();

	// ワールド座標の更新
	worldTransform_.UpdateMatrix();
}

void EnemyBullet::UpdateMortar() {
	// OBBの設定
	SettingOBBProperties();

	// 移動処理
	MoveMortar();

	// 寿命の処理
	UpdateLifeTimer();

	// ワールド座標の更新
	worldTransform_.UpdateMatrix();
}


// 描画処理
void EnemyBullet::Draw(Camera* camera)
{
	model_.Draw(worldTransform_, camera);
}


// 衝突時コールバック関数
void EnemyBullet::OnCollision(uint32_t id)
{
	if (id == ObjectId::Player) {
		
		isDead_ = true;
	}
}

void EnemyBullet::OnCollisionWithPlayer()
{
	isDead_ = true;
}


// 移動処理
void EnemyBullet::Move()
{
	// 速度を常に加算
	velocity_ = move_;
	worldTransform_.translate += velocity_;
}

// 移動処理
void EnemyBullet::MoveMortar()
{
	//水平方面は等速直線運動
	if (flame <= 120) {
		worldTransform_.translate.x += distance_.x / float(kLifeTimer_ - bombTimer_);
		worldTransform_.translate.z += distance_.z / float(kLifeTimer_ - bombTimer_);
	}

	//Y方面は垂直投射の処理
	//worldTransform_.translate.y = (9.8f * (float(flame) / 60.0f)) - (0.5f * 9.8f * ((float(flame) / 60.0f)) * ((float(flame) / 60.0f)));
	//worldTransform_.translate.y = 9.8f * (float(flame) / 60.0f) - (0.5f * 9.8f * float(pow(flame / 60.0, 2)));
	if (flame <= 60) {
		worldTransform_.translate.y = easeOutCubic(float(flame) / 60.0f) * 10.0f;
	}
	else if (flame <= 120) {
	worldTransform_.translate.y = easeOutCubic(1.0f - float(flame - 60) / 60.0f) * 10.0f;
	}
	else {//着弾時に爆風を発生
		float bombSize = 3.0f;

		worldTransform_.scale.x = bombSize;
		worldTransform_.scale.y = bombSize;
		worldTransform_.scale.z = bombSize;
	}

	//flameの加算
	flame++;
}


// OBBの設定
void EnemyBullet::SettingOBBProperties()
{
	// OBBのサイズの設定
	this->size_ = {
		2.0f * worldTransform_.scale.x,
		1.5f * worldTransform_.scale.y,
		2.0f * worldTransform_.scale.z,
	};
	OBBCollider::SetSize(this->size_);
	OBBCollider::SetRotate(worldTransform_.rotate);
}



// 寿命の処理
void EnemyBullet::UpdateLifeTimer()
{
	lifeTimer_--;

	if (lifeTimer_ <= 0.0f) {
		lifeTimer_ = 0;
		isDead_ = true;
	}
}

float easeOutCubic(float x){
	return float(1 - pow(1.0 - x, 3));
}

float easeOutCirc(float x){
return float(sqrt(1 - pow(x - 1, 2)));
}