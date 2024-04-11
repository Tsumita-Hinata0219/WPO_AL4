#pragma once

#include "IScene.h"
#include "GameManager.h"
#include "GameObject.h"

#include "Camera/MainCamera/MainCamera.h"

#include "Player/Player.h"
#include "Player/Bullet/PlayerBullet.h"

#include "Skydome/Skydome.h"
#include "Ground/Ground.h"

#include "CollisionManager.h"


class GameScene : public IScene {

public:

	/// <summary>
	/// コンストラクタ
	/// </summary>
	GameScene();

	/// <summary>
	/// デストラクタ
	/// </summary>
	~GameScene();

	/// <summary>
	/// 初期化処理
	/// </summary>
	void Initialize() override;

	/// <summary>
	/// 更新処理
	/// </summary>
	void Update(GameManager* state) override;

	/// <summary>
	/// 背景スプライトの描画処理
	/// </summary>
	void BackSpriteDraw() override;

	/// <summary>
	/// ３Dオブジェクトの描画処理
	/// </summary>
	void ModelDraw() override;

	/// <summary>
	/// 前景スプライトの描画処理
	/// </summary>
	void FrontSpriteDraw() override;

	/// <summary>
	/// プレイヤーバレットリストの追加
	/// </summary>
	void AddPlayerBullets(shared_ptr<PlayerBullet> addBullet) { playerBullets_.push_back(addBullet); }

private:

	/// <summary>
	/// コライダーの衝突判定
	/// </summary>
	void CheckAllCollision();

	/// <summary>
	/// プレイヤー関連の更新処理
	/// </summary>
	void PlayerUpdate();

	/// <summary>
	/// エネミー関連の更新処理
	/// </summary>
	void EnemyUpdate();


private:

	// メインカメラ
	unique_ptr<BaseCamera> mainCamera_ = nullptr;

	/* ----- Player プレイヤー ----- */
	unique_ptr<Player> player_ = nullptr;
	list<shared_ptr<PlayerBullet>> playerBullets_{};

	/* ----- CollisionManager コリジョンマネージャー ----- */
	unique_ptr<CollisionManager> collisionManager_ = nullptr;

	/* ----- ParticleManager パーティクルマネージャー ----- */
	ParticleManager* particleManager_;
};
