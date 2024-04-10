#include "GameScene.h"



/// <summary>
/// コンストラクタ
/// </summary>
GameScene::GameScene() {

	particleManager_ = ParticleManager::GetInstance();
}

/// <summary>
/// デストラクタ
/// </summary>
GameScene::~GameScene() 
{

}


/// <summary>
/// 初期化処理
/// </summary>
void GameScene::Initialize() {

	/* ----- Camera カメラ ----- */
	camera_ = make_unique<Camera>();
	camera_->Initialize();
	camera_->rotate = { 0.2f, 0.0f, 0.0f };
	camera_->translate = { 0.0f, 20.0f, -75.0f };

	mainCamera_ = make_unique<MainCamera>();
	mainCamera_->Initialize();

	/* ----- Skydome 天球 ----- */
	Skydome::GetInstance()->Initialize();

	/* ----- Ground 床 ----- */
	Ground::GetInstance()->Initialize();


	/* ----- Player プレイヤー ----- */
	//player_->Initialize();
	//player_ = make_unique<Player>();
	//player_->SetGameScene(this);


	/* ----- CollisionManager コリジョンマネージャー ----- */
	collisionManager_ = make_unique<CollisionManager>();


	/* ----- ParticleManager パーティクルマネージャー ----- */
	particleManager_->Initialize();
}


/// <summary>
/// 更新処理
/// </summary>
void GameScene::Update(GameManager* state) {

	/* ----- GameCamera ゲームカメラ----- */
	camera_->UpdateMatrix();
	mainCamera_->UpdateMatrix();


	/* ----- Skydome 天球 ----- */
	Skydome::GetInstance()->Update();


	/* ----- Ground 床 ----- */
	Ground::GetInstance()->Update();


	///* ----- Player プレイヤー ----- */
	//PlayerUpdate();


	///* ----- Enemy 敵 ----- */
	//enemyManager_.Update();


	///* ----- CollisionManager コリジョンマネージャー ----- */
	//CheckAllCollision();

	
	// ボタン押下でシーンチェンジ
	if (GamePadInput::PressButton(PadData::RIGHT)) {

		if (GamePadInput::TriggerButton(PadData::X)) {
			state->ChangeSceneState(new ClearScene());
		}
		if (GamePadInput::TriggerButton(PadData::Y)) {
			state->ChangeSceneState(new OverScene());
		}
	}

#ifdef _DEBUG

	ImGui::Begin("GameScene");
	
	ImGui::Text("");
	ImGui::Text("Camera");
	ImGui::DragFloat3("Rotate", &camera_->rotate.x, 0.01f);
	ImGui::DragFloat3("Translate", &camera_->translate.x, 0.01f);
	ImGui::Text("");

	ImGui::End();

#endif // _DEBUG
}


/// <summary>
/// 背景スプライトの描画処理
/// </summary>
void GameScene::BackSpriteDraw() {


}


/// <summary>
/// ３Dオブジェクトの描画処理
/// </summary>
void GameScene::ModelDraw() {

	/* ----- Skydome 天球 ----- */
	Skydome::GetInstance()->Draw(mainCamera_.get());

	/* ----- Ground 床 ----- */
	Ground::GetInstance()->Draw(mainCamera_.get());

	/* ----- Player プレイヤー ----- */
	//player_->Draw3D(camera_.get());
	//for (shared_ptr<IPlayerBullet> bullet : playerBullets_) {
	//	bullet->Draw3D(camera_.get());
	//}

	/* ----- Enemy 敵 ----- */
	//enemyManager_.Draw(camera_.get());

	/* ----- ParticleManager パーティクルマネージャー ----- */
	particleManager_->Draw(camera_.get());
}


/// <summary>
/// 前景スプライトの描画処理
/// </summary>
void GameScene::FrontSpriteDraw() {

	//player_->Draw2DFront(camera_.get());

}


// コライダーの衝突判定
void GameScene::CheckAllCollision()
{
	//// プレイヤーとエネミーの個別処理
	//for (IEnemy* enemy : *enemies_) {

	//	if (CollisionManager::CheckOBBxOBB(player_.get(), enemy->GetOBBCollider())) {
	//		
	//		player_->OnCollisionWithEnemy(enemy);
	//		enemy->OnCollisionWithPlayer(player_.get());
	//	}
	//}

	//// プレイヤーバレットとエネミーの個別処理
	//for (shared_ptr<IPlayerBullet> bullet : playerBullets_) {

	//	for (IEnemy* enemy : *enemies_) {

	//		if (CollisionManager::CheckOBBxOBB(bullet->GetOBBCOllider(), enemy->GetOBBCollider())) {
	//			
	//			bullet->onCollisionWithEnemy(enemy);
	//			enemy->OnCollisionWithPlayerBullet(bullet.get());
	//		}
	//	}
	//}

	//for (IEnemy* enemy : *enemies_) {
	//	for (EnemyBullet* bullet : *enemy->GetEnemyBullets()) {
	//		if (CollisionManager::CheckOBBxOBB(player_.get(), bullet)) {

	//			player_->OnCollisionWithEnemyBullet();
	//			bullet->OnCollisionWithPlayer();
	//		}
	//	}
	//}

}


// プレイヤー関連の更新処理
void GameScene::PlayerUpdate()
{
	// プレイヤー本体
	//player_->Update(camera_.get());

	

}


// エネミー関連の更新処理
void GameScene::EnemyUpdate()
{
	//enemyManager_.Update();
}
