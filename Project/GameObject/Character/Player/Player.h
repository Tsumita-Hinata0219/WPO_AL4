#pragma once

#include "GameObject.h"
#include "Player/Bullet/PlayerBullet.h"
#include "Player/Reticle/PlayerReticle.h"

class GameScene;

class Player : public OBBCollider {

public:

	Player() {};
	~Player() {};

	void Init();
	void Update(BaseCamera* camera);
	void Draw3D(BaseCamera* camera);
	void Draw2DF(BaseCamera* camera);

	void OnCollision(uint32_t id) override;
	void onCollisionWithEnemy();
	void onCollisionWithEnemyBullet();

#pragma region Get

	WorldTransform GetWorldTransform() { return this->wt_; }
	Vector3 GetOBBWorldPos() override { return this->wt_.GetWorldPos(); }
	Vector3 GetRotate() override { return this->wt_.rotate; }
	Vector3 GetSize() override { return this->size_; }

#pragma endregion 

#pragma region Set

	void SetRegisterScene(GameScene* scene) { this->gameScene_ = scene; }
	void SetParent(WorldTransform* parent) { this->wt_.SetParent(parent); }

#pragma endregion 

private:

	void Move();
	void Attack();
	void SettingOBB();


private:

	unique_ptr<Model> model_ = nullptr;
	WorldTransform wt_{};
	Vector3 velocity_{};
	Vector3 move_{};
	float moveSpeed_;
	const VHInfo vhInfo_ = { 12.5f, 1.5f, 14.5f, -14.5f };
	Vector3 size_{};


#pragma region Bullet バレット

private:

	void PushBackBulletList();
	Vector3 CalcDirection();


private:

	GameScene* gameScene_ = nullptr;
	unique_ptr<Model> bulletModel_ = nullptr;
	const float kBulletSpeed_ = 4.0f;

#pragma endregion 


#pragma region Reticle レティクル

private:

	void ReticleUpdate(BaseCamera* camera);


private:

	unique_ptr<PlayerReticle> reticle_ = nullptr;

#pragma endregion 
};

