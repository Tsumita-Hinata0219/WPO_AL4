#include "Mortar.h"
#include "GameScene/GameScene.h"

void Mortar::Initialize(Player* player, Vector3 position, int level) {
	level;
	player_ = player;
	model_.CreateFromObj("mortar", {}, worldTransform_);

	worldTransform_.Initialize();
	worldTransform_.translate = position;
	worldTransform_.scale *= 2;

	nowCT = bulletCoolTime;
	param.searchDistance = 15.0f;

	ParameterInitialize();

	isAlive_ = true;
	isFeed_ = false;
}

void Mortar::ParameterInitialize() {
	//パラメーター
	param.speed = 0.5f + level_ / 15.0f;
	param.hp = 15 + level_ * 2;
	param.power = level_ + 1;
	param.searchDistance = 15.0f + (float)level_ * 3;

	worldTransform_.scale = { (float)level_ / 2 + 1,(float)level_ / 2 + 1,(float)level_ / 2 + 1 };
}

void Mortar::Update() {
	if (!isFeed_) {
		Shoot();
		worldTransform_.UpdateMatrix();

		if (param.hp <= 0) {
			//worldTransform_.scale *= 1.3f;
			//model_.SetColor({ 0.0f,0.0f,255.0f,255.0f });
		//	isFeed_ = true;
		}
	}
	else {
		worldTransform_.UpdateMatrix();
	}

	for (EnemyBullet* bullet : bullet_) {
		bullet->UpdateMortar();
	}

	bullet_.remove_if([](EnemyBullet* bullet) {
		if (bullet->IsDead()) {
			delete bullet;
			return true;
		}
		return false;
		});
}

void Mortar::Draw(Camera* camera) {
	model_.Draw(worldTransform_, camera);

	for (EnemyBullet* bullet : bullet_) {
		bullet->Draw(camera);
	}
}

void Mortar::Shoot() {
	//CT減算
	nowCT--;

	//プレイヤーとの距離を計算
	distance = player_->GetWorldPos() - worldTransform_.translate;
	distanceToPlayer_ = Length(distance);

	//射程範囲内かつCTが過ぎてる時
	if (param.searchDistance >= distanceToPlayer_ && nowCT <= 0) {
		nowCT = bulletCoolTime;

		EnemyBullet* newBullet;
		newBullet = new EnemyBullet();
		newBullet->InitializeMortar(worldTransform_.translate, player_->GetWorldPos());
		bullet_.push_back(newBullet);
		//gameScene->AddEnemyBulletM(newBullet);
	}
}

void Mortar::OnCollision(uint32_t id)
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

void Mortar::OnCollisionWithPlayer(Player* player)
{
	if (isFeed_) {
		player->AddEnergy(10);
		isAlive_ = false;
	}
}

void Mortar::OnCollisionWithPlayerBullet(IPlayerBullet* bullet)
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

void Mortar::SettingColliderBit()
{
	OBBCollider::SetCollosionAttribute(ObjectAttribute::Enemy);
	OBBCollider::SetCollisionMask(ObjectMask::Enemy);
	OBBCollider::SetID(ObjectId::Enemy);
}

void Mortar::SettingCollider()
{
	// OBBのサイズの設定
	this->size_ = 2.0f * worldTransform_.scale;
	OBBCollider::SetSize(this->size_);
	OBBCollider::SetRotate(worldTransform_.rotate);
}