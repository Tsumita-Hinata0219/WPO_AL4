#pragma once

#include "GameObject.h"

class GameScene;

class Player {

public:

	Player() {};
	~Player() {};

	void Init();
	void Update();
	void Draw3D();
	void Draw2DF();

	void onCollisionWithEnemy();
	void onCollisionWithEnemyBullet();

#pragma region Get

	WorldTransform GetWorldTransform() { return this->wt_; }
	Vector3 GetWorldPosition() { return this->wt_.GetWorldPos(); }
	Vector3 GetRotate() { return this->wt_.rotate; }
	Vector3 GetSize() { return this->size_; }

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
	Vector3 vekicity_{};
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

	void ReticleUpdate();


private:

	//unique_ptr<PlayerReticle> reticle_ = nullptr;

#pragma endregion 
};

