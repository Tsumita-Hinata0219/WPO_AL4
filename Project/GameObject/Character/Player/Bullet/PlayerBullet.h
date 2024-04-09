#pragma once

#include "GameObject.h"

class PlayerBullet : public OBBCollider {

public:

	PlayerBullet() {};
	~PlayerBullet() {};

	void Init();
	void Update();
	void Draw();

	void onCollisionWithEnemy();
	void onCollisionWithEnemyBullet();

#pragma region Get

	bool IsDead() { return this->isDead_; }
	WorldTransform GetWorldTransform() { return this->wt_; }
	Vector3 GetOBBWorldPos() override { return this->wt_.GetWorldPos(); }
	Vector3 GetRotate() override { return this->wt_.rotate; }
	Vector3 GetSize() override { return this->size_; }

#pragma endregion 

private:

	void Move();
	void SettingOBB();
	void UpdateLifeTimer();


private:

	unique_ptr<Model> model_ = nullptr;
	WorldTransform wt_{};
	Vector3 velocity_{};
	Vector3 move_{};
	Vector3 size_{};
	bool isDead_;
	int32_t lLifeTime_;
	int32_t lifeTimer_;
};

