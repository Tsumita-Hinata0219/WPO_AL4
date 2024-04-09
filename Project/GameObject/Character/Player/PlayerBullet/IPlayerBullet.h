#pragma once

#include "GameObject.h"
#include "OBBCollider.h"


/* Playerの前方宣言 */
class Player;

/* IEnmeyの前方宣言 */
class IEnemy;


enum PlayerBulletType {
	TypeNormal,
	TypePenetration,
	TypeShotgun,
};

class IPlayerBullet {

public: // プレイヤーバレット基底クラス : メンバ関数

	// コンストラクタとデストラクタ
	IPlayerBullet() {};
	~IPlayerBullet() {};

	// 初期化処理　更新処理　描画処理
	virtual void Initialize(Player* player, Model& modeHD, Vector3 initPos, Vector3 vel, int damage) = 0;
	virtual void Update() = 0;
	virtual void Draw3D(Camera* camera) = 0;

#pragma region onCollision 衝突判定

	// エネミーとの衝突判定
	void onCollisionWithEnemy(IEnemy* enemy);

#pragma endregion 

#pragma region Get 取得

	// 死亡フラグの取得
	virtual bool isDead() = 0;

	// OBBコライダーの取得
	virtual OBBCollider* GetOBBCOllider() = 0;

	// ダメージの取得
	virtual int GetDamage() { return this->damage_; }

#pragma endregion 

protected: // プレイヤーバレット基底クラス : メンバ変数

	// 寿命の処理
	void RemoveAfterLifetime();

protected:

	// 死亡フラグ
	bool isDead_ = false;

	// 寿命のタイマー
	int lifeTimer_ = 0;

	// Player
	Player* player_ = nullptr;

	// ダメージ
	int damage_;

};

