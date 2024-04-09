#include "Blast.h"
#include "GameObject.h"

void Blast::Initialize(Player* player, Vector3 position, int level) {
	player_ = player;
	worldTransform_.Initialize();
	worldTransform_.translate = position;
	velocity_ = { 0,0,0 };
	model_.CreateFromObj("ball", {}, worldTransform_);
	isAlive_ = true;
	isFeed_ = false;
	velocityXZ_ = 0;
	cntAhead_ = kTurnInterval_;
	rad_ = 0;
	t_ = 0;
	flame_ = 0;
	tmpPos_ = {};

	ParameterInitialize(level);
}

void Blast::ParameterInitialize(int level) {
	level;
	//パラメーター
	param.speed = 0.5f + level_ / 15.0f;
	param.hp = 15 + level_ * 2;
	param.power = level_ + 1;
	param.searchDistance = 15.0f + (float)level_ * 3;

	worldTransform_.scale = { (float)level_ / 2 + 1,(float)level_ / 2 + 1,(float)level_ / 2 + 1 };
}

void Blast::Update() {
	flame_++;

	if (!isFeed_) {
		Wander();

		CalculateDetectionRange();

		Chace();



		worldTransform_.UpdateMatrix();

		ImGui::Begin("enemy");
		ImGui::DragFloat3("Translate", &worldTransform_.translate.x, 0.005f);
		ImGui::DragFloat3("Rotate", &worldTransform_.rotate.x, 0.005f);
		ImGui::End();

				if (param.hp <= 0) {
			worldTransform_.scale *= 1.3f;
			model_.SetColor({ 0.0f,0.0f,255.0f,255.0f });
			isFeed_ = true;
		}
	}
	else {
		worldTransform_.UpdateMatrix();
	}
}

void Blast::Draw(Camera* camera) {
	model_.Draw(worldTransform_, camera);
}

void Blast::OnCollision() {
}

std::unique_ptr<Blast> Blast::Spawn(Player* player, Vector3 position, int level) {
	std::unique_ptr<Blast> newEnemy = std::make_unique<Blast>();
	newEnemy->Initialize(player, position, level);

	return newEnemy;
}

void Blast::OnCollision(uint32_t id)
{
	if (id == ObjectId::PlayerBullet && !isFeed_) {
		Log("Hit!!\n");
		param.hp -= 10;
		//param.hp -= player_->GetAttackLevel();
	}
	if (id == ObjectId::Player && isFeed_) {
		player_->AddEnergy(10);
		isAlive_ = false;
	}
}

void Blast::OnCollisionWithPlayer(Player* player)
{
	if (isFeed_) {
		player->AddEnergy(10);
		isAlive_ = false;
	}
}

void Blast::OnCollisionWithPlayerBullet(IPlayerBullet* bullet)
{
	if (!isFeed_) {
		Log("Hit!!\n");
		param.hp -= bullet->GetDamage();
		param.speed -= 0.02f;

		/*float color = 0;

		color += 30.0f;

		model_.SetColor({ 0.0f,0.0f,color,255.0f });*/

		//param.hp -= player_->GetAttackLevel();
	}
}

void Blast::SettingColliderBit()
{
	OBBCollider::SetCollosionAttribute(ObjectAttribute::Enemy);
	OBBCollider::SetCollisionMask(ObjectMask::Enemy);
	OBBCollider::SetID(ObjectId::Enemy);
}

void Blast::SettingCollider()
{
	// OBBのサイズの設定
	this->size_ = 2.0f * worldTransform_.scale;
	OBBCollider::SetSize(this->size_);
	OBBCollider::SetRotate(worldTransform_.rotate);
}

void Blast::CalculateDetectionRange() {
	distanceToPlayer_ = CalculateEuclideanDistance(player_->GetWorldPos(), worldTransform_.translate);

	//もし索敵範囲内なら追いかける
	if (distanceToPlayer_ <= kSearchRange_) {
		isTrace_ = true;
	}
	else {
		isTrace_ = false;
	}
}

void Blast::Chace() {
	if (isTrace_) {
		// 座標を移動させる（1フレーム分の移動量を足しこむ)
		Vector3 toPlayer = player_->GetWorldPos() - worldTransform_.translate;

		//ベクトルを正規化する
		toPlayer = Normalize(toPlayer);
		//velocity_ = Normalize(velocity_);

		velocity_ = SLerp(velocity_, toPlayer, kChaseIntensity_) * param.speed;

		worldTransform_.translate = worldTransform_.translate + velocity_;

		// Y軸周り角度(0y)
		worldTransform_.rotate.y = std::atan2(velocity_.x, velocity_.z);

		velocityXZ_ = std::sqrt(velocity_.x * velocity_.x + velocity_.z * velocity_.z);
		// X軸周り角度(0x)
		worldTransform_.rotate.x = std::atan2(-velocity_.y, velocityXZ_);

		t_ = 0;
	}
}


void Blast::Wander() {
	if (!isTrace_) {
		if (t_ == 0) {
			float rad = 10;

			Scope scope = { rad / 2,rad };
			rad_ = RandomGenerator::getRandom(scope);
			tmpPos_ = CalculateNextPosition(worldTransform_.translate, rad_);
		}


		Scope scope = { 80.0f,180.0f };

		float currentTime = 1.0f / RandomGenerator::getRandom(scope);

		if (t_ <= 1.0f) {
			t_ += currentTime;
		}if (t_ > 1.0f) {
			t_ = 0;
		}

		float easedValue = easeInOutQuad(t_);

		worldTransform_.translate = MoveWithEasing(worldTransform_.translate, tmpPos_, easedValue);

		// Y軸周り角度(0y)
		worldTransform_.rotate.y = std::atan2(velocity_.x, velocity_.z);

		velocityXZ_ = std::sqrt(velocity_.x * velocity_.x + velocity_.z * velocity_.z);
		// X軸周り角度(0x)
		worldTransform_.rotate.x = std::atan2(-velocity_.y, velocityXZ_);
	}
}


float Blast::CalculateEuclideanDistance(const Vector3& point1, const Vector3& point2) {
	float deltaX = point2.x - point1.x;
	float deltaY = point2.y - point1.y;
	float deltaZ = point2.z - point1.z;

	return std::sqrt(deltaX * deltaX + deltaY * deltaY + deltaZ * deltaZ);
}

Vector3 Blast::GetFrontVector() {
	Matrix4x4 wM = MakeRotateXYZMatrix(worldTransform_.rotate);
	Vector3 dir = { 0.0f,0.0f,1.0f };
	return Normalize(Multiply(dir, wM));
}

Vector3 Blast::Multiply(const Vector3& v, const Matrix4x4& m) {
	Vector3 result{};
	result.x = v.x * m.m[0][0] + v.y * m.m[1][0] + v.z * m.m[2][0] + m.m[3][0];
	result.y = v.x * m.m[0][1] + v.y * m.m[1][1] + v.z * m.m[2][1] + m.m[3][1];
	result.z = v.x * m.m[0][2] + v.y * m.m[1][2] + v.z * m.m[2][2] + m.m[3][2];
	return result;
}

Vector3 Blast::CalculateNextPosition(const Vector3& position, float radius) {
	// 乱数生成器の準備
	static std::random_device rd;
	static std::mt19937 gen(rd());
	static std::uniform_real_distribution<float> dis(-1.0f, 1.0f);

	// ランダムな方向ベクトルを生成し、長さをradiusにスケーリングする
	float dx = dis(gen) * radius;
	float dy = dis(gen) * radius;

	// 予測移動先座標を計算する
	float nextX = position.x + dx;
	float nextZ = position.z + dy;

	return { nextX, 0,nextZ };
}

Vector3 Blast::MoveWithEasing(const Vector3& currentPosition, const Vector3& targetPosition, float t) {
	// 現在位置から目標位置までの距離と方向を計算する
	float deltaX = targetPosition.x - currentPosition.x;
	float deltaY = targetPosition.z - currentPosition.z;

	// イージングを適用した移動量を計算する
	float newX = currentPosition.x + deltaX * t;
	float newZ = currentPosition.z + deltaY * t;

	// Y軸周り角度(0y)
	worldTransform_.rotate.y = std::atan2(newX, newZ);

	velocityXZ_ = std::sqrt(newX * newX + newZ * newZ);
	// X軸周り角度(0x)
	worldTransform_.rotate.x = std::atan2(-velocity_.y, velocityXZ_);

	return { newX, 0,newZ };
}

float Blast::easeInOutQuad(float startValue, float movement, float currentTime) {
	float duration = 1.0;  // イージングにかかる時間（1秒と仮定）

	currentTime /= duration / 2.0f;
	if (currentTime < 1.0f) {
		return movement / 2.0f * currentTime * currentTime + startValue;
	}

	currentTime--;
	return -movement / 2.0f * (currentTime * (currentTime - 2.0f) - 1.0f) + startValue;
}

Vector3 Blast::easeInOutQuad(Vector3 startValue, Vector3 movement, float currentTime) {
	float duration = 1.0;  // イージングにかかる時間（1秒と仮定）

	currentTime /= duration / 2.0f;
	if (currentTime < 1.0f) {
		return movement / 2.0f * currentTime * currentTime + startValue;
	}

	currentTime--;
	return -movement / 2.0f * (currentTime * (currentTime - 2.0f) - 1.0f) + startValue;
}

float Blast::easeInOutQuad(float x) {
	return x < 0.5f ? 2.0f * x * x : 1.0f - std::pow(-2.0f * x + 2.0f, 2.0f) / 2.0f;
}