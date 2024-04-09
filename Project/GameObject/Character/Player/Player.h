#pragma once

#include "GameObject.h"
#include "PlayerBaseStatus/PlayerBaseStatus.h"
#include "PlayerReticle/PlayerReticle.h"
#include "PlayerBullet/IPlayerBullet.h"
#include "PlayerBullet/NormalBullet/NormalBullet.h"
#include "PlayerBullet/PenetrationBullet/PenetrationBullet.h"
#include "PlayerBullet/Shotgun/Shotgun.h"

#include "OBBCollider.h"

/* GameSceneの前方宣言 */
class GameScene;

/* Enemyの前方宣言 */
class IEnemy;

/* Playerクラス */
class Player : public OBBCollider {

public: // プレイヤー本体 : メンバ関数

	// コンストラクタとデストラクタ
	Player() {};
	~Player() {};

	// 初期化処理　更新処理　描画処理
	void Initialize();
	void Update(Camera* camera);
	void Draw3D(Camera* camera);
	void Draw2DFront(Camera* camera);


	// レベルの変更処理
	void SettingLev();

#pragma region Get 取得
	 
	// ワールド座標の取得
	Vector3 GetWorldPos() { return this->bodyWt_.GetWorldPos(); }

	// 各レベルの取得
	int GetHPLevel() { return this->hpLv_; }
	int GetVelocityLevel() { return this->velLv_; }
	int GetAttackLevel() { return this->attackLv_; }
	int GetAttackIntervalLevel() { return this->attackIntervalLv_; }
	int GetTotalLevel() { return this->totalLevel_; }

	// ULT中かのフラグの取得
	int GetIsULT() { return this->isULTFunc_; }

#pragma endregion

#pragma region Set 設定

	// エネルギーの設定
	void AddEnergy(int add) { this->energy_ += add; }

#pragma endregion

#pragma region Collider 衝突判定

	// 衝突判定
	void OnCollision(uint32_t id) override;

	// エネミーとの個別衝突判定
	void OnCollisionWithEnemy(IEnemy* enemy);
	void OnCollisionWithEnemyBullet();

	// コライダーのゲッター
	Vector3 GetOBBWorldPos() override { return bodyWt_.GetWorldPos(); }
	Vector3 GetSize() override { return this->size_; }
	Vector3 GetRotate() override { return this->bodyWt_.rotate; }

#pragma endregion 

private:

	// Colliderのビットのセッティング
	void SettingColliderBit();

	// OBBColliderのセッティング
	void SettingCollider();

	// 移動処理
	void Move();

	// プレイヤー本体の姿勢処理
	void CalcBodyRotate();

	// ダッシュの処理
	void DashFunc();

	// ダッシュ先の座標の計算
	Vector3 CalcToDashPos();

	// レベルチェック
	void LevelCheck();

	// ウルトの処理
	void ULTFunc();


private: // プレイヤー本体 : メンバ変数

	// 本体
	unique_ptr<Model> bodyModel_ = nullptr;
	WorldTransform bodyWt_{};

	// 移動速度
	Vector3 velocity_{};
	float moveVector_;

	// サイズ
	Vector3 size_{};

	// 移動先のベクトル
	Vector2 moveV_{};

	// エネルギー
	int energy_;

	// LevelPrope
	int levelPrope_;

	// HP
	int hp_;
	int hpLv_;

	// Vel
	int velLv_;

	// Attack
	int attack_;
	int attackLv_;

	// AttackInterval
	int attackIntervalLv_;

	// DashLevel
	int dashLv_;
	float toDash_;

	// totalLevel
	int totalLevel_;

	// プレイヤーの基本ステータス
	unique_ptr<PlayerBaseStatus> baseStatus_ = nullptr;

	Segment segment_{};

#pragma region Dash

	// ダッシュしているかのフラグ
	bool isDash_;

	// ダッシュ先の座標
	Vector3 toDashPosition_{};
	Vector3 toInitDashPosition_{};

	// ダッシュに必要なフレーム
	int ASDashFrame_;

	// ダッシュにかかってるフレーム
	int nowDashFrame_;

#pragma endregion 

#pragma region ULT

	// ウルト中かのフラグ
	bool isULTFunc_;

	// ウルトの範囲
	Scope ultScope_;

	// ウルト開始時の座標
	Vector3 ultStartPos_{};

	// ウルトで使う移動先の座標
	array<Vector3, 50> ultPos_;

	// ウルトで使う移動時の初期座標
	Vector3 ultInitPos_;

	// ウルトの配列のカウント
	int ultArrayCount_;

	// ウルトの移動にかかるフレーム
	int ASUltFrame_;
	
	// ウルトの移動にかかってるフレーム
	int nowUltFrame_;

#pragma endregion 


public: // バレット : メンバ関数

#pragma region Set 設定

	// バレットの登録先のシーン
	void SetGameScene(GameScene* scene) { this->registerScene_ = scene; }

	// 射撃のインターバルの設定
	void SetAttackInterval(uint32_t newInterval) { this->AttackTimer_ = newInterval; }

#pragma endregion

private:

	// 射撃処理
	void IsAttack();

	// バレットの設定
	void SettingNewBullet();

	// バレットの進行方向の計算
	Vector3 CalcBulletDirection();

private: // バレット : メンバ変数

	// Bullet
	unique_ptr<Model> bulletModel_ = nullptr;

	// バレットの速度
	float bulletVelocity_;

	// 射撃のインターバル
	int AttackInterval_;
	int AttackTimer_;

	// バレットのタイプ
	PlayerBulletType bulletType_;

	// 散弾の弾数
	int pelletNum_;



private: // レティクル : メンバ変数

	// Reticle
	unique_ptr<PlayerReticle> reticle_ = nullptr;



private: // シーン : メンバ変数

	// 登録先シーン
	GameScene* registerScene_ = nullptr;

};

