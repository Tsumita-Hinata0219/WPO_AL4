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
	player_ = make_unique<Player>();
	player_->Initialize();
	player_->SetGameScene(this);


	/* ----- CollisionManager コリジョンマネージャー ----- */
	collisionManager_ = make_unique<CollisionManager>();


	/* ----- GameWave ゲームウェーブ ----- */
	waveCount_ = 1;
	waveMax_ = 100;
	waveNowTime_ = 0;
	waveEndTime_ = 6 * 100;
	isWaveStart_ = true;
	isSceneChange_ = false;
	isWaveGaming_ = true;


	/* ----- Enemy 敵 ----- */
	enemyManager_.SetGameScene(this);
	enemyManager_.EnemySpawn(player_.get());
	enemies_ = enemyManager_.GetEnemy();


	/* ----- Fade フェード ----- */
	fadeTexHD_ = TextureManager::LoadTexture("Fade.png");
	fadeSprite_ = make_unique<Sprite>();
	fadeSprite_->Initialize({ 1280, 720 }, fadeColor_);
	fadeWt_.Initialize();
	isFade_ = false;

	/* ----- ParticleManager パーティクルマネージャー ----- */
	particleManager_->Initialize();
}


/// <summary>
/// 更新処理
/// </summary>
void GameScene::Update(GameManager* state) {

	//std::this_thread::sleep_for(std::chrono::milliseconds(16)); // 60FPSを想定
	///* ----- GameCamera ゲームカメラ----- */
	//camera_->UpdateMatrix();


	///* ----- Skydome 天球 ----- */
	//Skydome::GetInstance()->Update();


	///* ----- Ground 床 ----- */
	//Ground::GetInstance()->Update();


	///* ----- Player プレイヤー ----- */
	//PlayerUpdate();


	///* ----- Enemy 敵 ----- */
	//enemyManager_.Update();


	///* ----- CollisionManager コリジョンマネージャー ----- */
	//CheckAllCollision();

	
	/* ----- GameWave ゲームウェーブ ----- */
	// ウェーブ開始フラグがたっていたら初期化処理に入る
	if (isWaveStart_) {
		WaveInit();
	}
	
	// ウェーブの更新処理
	WaveUpdate();

	// シーン遷移のフラグが立っていたらシーン遷移
	if (isSceneChange_) {

		// 時間いっぱい生き残ったってことだからクリアシーンへ
		state->ChangeSceneState(new ClearScene());
		return;
	}

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
	ImGui::Text("Description of scene change");
	ImGui::Text("Scene change to 'ClearScene' by pressing X button while holding down Right.");
	ImGui::Text("Scene change to 'OverScene' by pressing Y button while holding down Right.");
	ImGui::Text("");

	ImGui::Text("Wave");
	ImGui::Text("WaveCount = %d : WaveMax = %d", waveCount_, waveMax_);
	ImGui::Text("WaveNowTime = %d", waveNowTime_);
	ImGui::Text("WaveEndTime = %d", waveEndTime_);


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
	Skydome::GetInstance()->Draw(camera_.get());

	/* ----- Ground 床 ----- */
	Ground::GetInstance()->Draw(camera_.get());

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

	/* ----- Fade フェード ----- */
	if (isFade_) {
		fadeSprite_->Draw(fadeTexHD_, fadeWt_, camera_.get());
	}
}



/// <summary>
/// ウェーブの初期化処理
/// </summary>
void GameScene::WaveInit()
{
	enemyManager_.EnemySpawn(player_.get());
	enemies_ = enemyManager_.GetEnemy();

	// ウェーブ開始のフラグを折って初期化処理に入らないようにする
	isWaveStart_ = false;
}


/// <summary>
/// ウェーブの更新処理
/// </summary>
void GameScene::WaveUpdate()
{
	// フラグがたっていなかったら早期リターン
	if (isWaveGaming_) {

		// ウェーブタイマーのインクリメント
		//waveNowTime_++;

		/* ----- GameCamera ゲームカメラ----- */
		camera_->UpdateMatrix();


		/* ----- Skydome 天球 ----- */
		Skydome::GetInstance()->Update();


		/* ----- Ground 床 ----- */
		Ground::GetInstance()->Update();


		/* ----- Player プレイヤー ----- */
		PlayerUpdate();

		/* ----- LevelManager レベルマネージャー ----- */
		levelManager_.Update(&enemyManager_, player_.get(), waveCount_);

		/* ----- Enemy 敵 ----- */
		EnemyUpdate();

		/* ----- Fade フェード ----- */
		fadeWt_.UpdateMatrix();
		fadeSprite_->SetColor(fadeColor_);

		/* ----- CollisionManager コリジョンマネージャー ----- */
		CheckAllCollision();
	}

	// タイマーが既定値になっていたら終了処理に入る
	if (waveNowTime_ == waveEndTime_) {

		isWaveGaming_ = false;

		// フェードの出現
		isFade_ = true;

		WaveExit();
	}
}


/// <summary>
/// ウェーブの終了処理
/// </summary>
void GameScene::WaveExit()
{
	if (GamePadInput::TriggerButton(PadData::B) || KeysInput::TriggerKey(DIK_SPACE)) {

		// ウェーブ開始のフラグを立ててウェーブの初期化処理に入るようにする
		isWaveStart_ = true;

		// ウェーブのカウントをインクリメント
		waveCount_++;

		// ウェーブのタイマーを0に戻す
		waveNowTime_ = 0;

		// ゲーム処理に入るフラグを立てる
		isWaveGaming_ = true;

		// フェードを消す
		isFade_ = false;
	}

	player_->SettingLev();

	// ウェーブのカウントがマックスだったらシーン遷移
	if (waveCount_ == waveMax_) {

		// シーン遷移のフラグを立てる
		isSceneChange_ = true;
	}
}


// コライダーの衝突判定
void GameScene::CheckAllCollision()
{
	// プレイヤーとエネミーの個別処理
	for (IEnemy* enemy : *enemies_) {

		if (CollisionManager::CheckOBBxOBB(player_.get(), enemy->GetOBBCollider())) {
			
			player_->OnCollisionWithEnemy(enemy);
			enemy->OnCollisionWithPlayer(player_.get());
		}
	}

	// プレイヤーバレットとエネミーの個別処理
	for (shared_ptr<IPlayerBullet> bullet : playerBullets_) {

		for (IEnemy* enemy : *enemies_) {

			if (CollisionManager::CheckOBBxOBB(bullet->GetOBBCOllider(), enemy->GetOBBCollider())) {
				
				bullet->onCollisionWithEnemy(enemy);
				enemy->OnCollisionWithPlayerBullet(bullet.get());
			}
		}
	}

	// これ何？
	for (IEnemy* enemy : *enemies_) {
		for (EnemyBullet* bullet : *enemy->GetEnemyBullets()) {
			if (CollisionManager::CheckOBBxOBB(player_.get(), bullet)) {

				player_->OnCollisionWithEnemyBullet();
				bullet->OnCollisionWithPlayer();
			}
		}
	}


	//// コライダーリストのクリア
	//collisionManager_->ColliderClear();

	//// コライダーリストの追加
	//collisionManager_->AddOBBColliders(player_.get());
	//for (shared_ptr<IPlayerBullet> bullet : playerBullets_) {
	//	collisionManager_->AddOBBColliders(bullet->GetOBBCOllider());
	//}

	//for (IEnemy* enemy : *enemies_) {
	//	collisionManager_->AddOBBColliders(enemy->GetOBBCollider());
	//}
	/*for (EnemyBullet* bullet : enemyBullets_) {
		collisionManager_->AddOBBColliders(bullet);
	}
	for (EnemyBullet* bullet : enemyBulletsM_) {
		collisionManager_->AddOBBColliders(bullet);
	}*/

	// コライダーの衝突判定
	//collisionManager_->CheckAllCollision();
}


// プレイヤー関連の更新処理
void GameScene::PlayerUpdate()
{
	// プレイヤー本体
	player_->Update(camera_.get());

	// プレイヤーバレット
	for (shared_ptr<IPlayerBullet> bullet : playerBullets_) {
		bullet->Update();
	}
	playerBullets_.remove_if([](shared_ptr<IPlayerBullet> bullet) {
		if (bullet->isDead()) {
			bullet.reset();
			return true;
		}
		return false;
		}
	);

}


// エネミー関連の更新処理
void GameScene::EnemyUpdate()
{
	enemyManager_.Update();
}
