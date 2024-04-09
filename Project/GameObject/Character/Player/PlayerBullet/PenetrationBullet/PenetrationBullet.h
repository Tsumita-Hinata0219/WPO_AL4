#pragma once

#include "GameObject.h"

#include "OBBCollider.h"
#include "Player/PlayerBullet/IPlayerBullet.h"

/* PenetrationBulletクラス */
class PenetrationBullet : public IPlayerBullet, public OBBCollider {

public: // プレイヤーバレット基底クラス : メンバ関数

	// コンストラクタとデストラクタ
	PenetrationBullet() {};
	~PenetrationBullet() {};

	// 初期化処理　更新処理　描画処理
	void Initialize(Player* player, Model& modeHD, Vector3 initPos, Vector3 vel, int damage) override;
	void Update() override;
	void Draw3D(Camera* camera) override;

#pragma region Get 取得

	// 死亡フラグの取得
	bool isDead() override { return this->isDead_; }

	// OBBコライダーの取得
	OBBCollider* GetOBBCOllider() override { return this; }

	// ダメージの取得
	//int GetDamage() override { return this->damage_; }

#pragma endregion 

#pragma region Collider 衝突判定

	// 衝突判定
	void OnCollision(uint32_t id) override;

	// コライダーのゲッター
	Vector3 GetOBBWorldPos() override { return bulletWt_.GetWorldPos(); }
	Vector3 GetSize() override { return this->size_; }
	Vector3 GetRotate() override { return this->bulletWt_.rotate; }

#pragma endregion 


private: // プレイヤーバレット : メンバ関数

	// Colliderのビットのセッティング
	void SettingColliderBit();

	// OBBColliderのセッティング
	void SettingCollider();

	// 移動処理
	void Move();

	// 無敵時間の処理
	bool InitiateShieldFunc();


private: // プレイヤーバレット基底クラス : メンバ変数

	unique_ptr<Model> bulletModel_ = nullptr;
	WorldTransform bulletWt_{};

	// 移動速度
	Vector3 velocity_;

	// サイズ
	Vector3 size_{};

	// 衝突カウント
	uint32_t hitCount_;

	// 死亡カウント
	uint32_t deadCount_;

	// 衝突したかのフラグ
	bool isHit_;

	// 無敵時間
	uint32_t shieldTimer_;
};

