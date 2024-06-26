#pragma once

#include "GameObject.h"

class Player;

class PlayerReticle {

public:

	PlayerReticle() {};
	~PlayerReticle() {};

	void Init(Model& modelHD, Vector3 initTranslate);
	void Update(BaseCamera* camera);
	void Draw3D(BaseCamera* camera);
	void Draw2DF(BaseCamera* camera);

#pragma region Get

	WorldTransform GetWorldTransform() { return this->wt3D_; }
	Vector3 GetWorldPosition() { return this->wt3D_.GetWorldPos(); }

#pragma endregion 

#pragma region Set

	void SetPlayer(Player* player) { this->player_ = player; }
	void SetParent(WorldTransform* parent) { this->wt3D_.SetParent(parent); }
	void SetReticlePos(Vector3 setPos) { this->wt3D_.translate = setPos; }

#pragma endregion

private:

	void Move();

private:

	Player* player_ = nullptr;
	unique_ptr<Model> model_ = nullptr;
	unique_ptr<Sprite> sprite_ = nullptr;
	uint32_t reticleTexHD_;
	WorldTransform wt3D_{};
	WorldTransform wt2D_{};
	const float kDistReticle_ = 60.0f;
	Vector3 offsetVec_{};
	Vector2 size_{};
	Vector3 move_{};
	Vector3 velocity_{};
	Vector2 rStick_{};
	Vector2 spritePos_{};
	float moveSpeed_;
};
