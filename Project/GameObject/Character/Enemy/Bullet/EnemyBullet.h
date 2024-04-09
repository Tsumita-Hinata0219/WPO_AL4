#pragma once

#include "GameObject.h"
#include "OBBCollider.h"
#include "ColliderConfig.h"


/* PlayerBulletクラス */
class EnemyBullet : public OBBCollider {

public: // メンバ関数

	/// <summary>
	/// コンストラクタ
	/// </summary>
	EnemyBullet() {};

	/// <summary>
	/// デストラクタ
	/// </summary>
	~EnemyBullet() {};

	/// <summary>
	/// 初期化処理
	/// </summary>
	void Initialize(Vector3 pos, Vector3 vel);
	void InitializeMortar(Vector3 pos, Vector3 pPos);

	/// <summary>
	/// 更新処理
	/// </summary>
	void Update();
	void UpdateMortar();

	/// <summary>
	/// 描画処理
	/// </summary>
	void Draw(Camera* camera);

	// <summary>
	// 衝突時コールバック関数
	// </summary>
	void OnCollision(uint32_t id) override;
	void OnCollisionWithPlayer();


#pragma region Get

	/// <summary>
	/// 死亡フラグの取得
	/// </summary>
	/// <returns></returns>
	bool IsDead() { return this->isDead_; }

	/// <summary>
	/// ワールドトランスフォームの取得
	/// </summary>
	WorldTransform GetWorldTransform() { return this->worldTransform_; }

	/// <summary>
	/// ワールド座標の取得
	/// </summary>
	Vector3 GetOBBWorldPos() override { return this->worldTransform_.GetWorldPos(); }

	/// <summary>
	/// 回転軸の取得
	/// </summary>
	Vector3 GetRotate() override { return this->worldTransform_.rotate; }

	/// <summary>
	/// サイズの取得
	/// </summary>
	Vector3 GetSize() override { return this->size_; }

#pragma endregion


private: // メンバ関数

	/// <summary>
	/// 移動処理
	/// </summary>
	void Move();
	void MoveMortar();

	/// <summary>
	/// OBBの設定
	/// </summary>
	void SettingOBBProperties();

	/// <summary>
	/// 寿命の処理
	/// </summary>
	void UpdateLifeTimer();


private: // メンバ変数

	// モデル
	Model model_;

	// ワールドトランスフォーム
	WorldTransform worldTransform_{};
	WorldTransform tmpWorldTransform_{};

	// 加算速度
	Vector3 velocity_{};

	// 移動量
	Vector3 move_{};

	//
	Vector3 distance_{};
	int flame;

	// サイズ
	Vector3 size_{};

	// 死亡フラグ
	bool isDead_ = false;

	// 寿命
	int32_t kLifeTimer_;
	int32_t lifeTimer_;
	int32_t bombTimer_;
};

float easeOutCubic(float x);

float easeOutCirc(float x);