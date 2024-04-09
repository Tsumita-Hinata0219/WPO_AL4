#include "Player.h"
#include "GameScene/GameScene.h"
#include "Enemy/IEnemy/IEnemy.h"


// 初期化処理
void Player::Initialize()
{
	// Bodyの初期化処理
	bodyModel_ = make_unique<Model>();
	bodyModel_->CreateFromObj("PlayerBody", "Player");
	bodyWt_.Initialize();


	// 移動速度
	velocity_ = Vector3::zero;
	moveVector_ = 0.3f;


	// Bulletの初期化処理
	bulletModel_ = make_unique<Model>();
	bulletModel_->CreateFromObj("PlayerBullet", "Player");
	bulletVelocity_ = 1.5f;
	AttackInterval_ = 20;
	AttackTimer_ = 1;
	bulletType_ = TypePenetration;
	pelletNum_ = 5;

	
	// Reticleの初期化処理
	reticle_ = make_unique<PlayerReticle>();
	reticle_->Initialize();
	reticle_->SetParent(&bodyWt_);


	// エネルギーの初期設定
	energy_ = 100;


	// 初期レベル
	levelPrope_ = HP;
	hpLv_ = LvOne;
	velLv_ = LvOne;
	attackLv_ = LvOne;;
	attackIntervalLv_ = LvOne;
	dashLv_ = LvOne;
	totalLevel_ = 0;


	// ダッシュしているかのフラグ
	isDash_ = false;

	// ダッシュにかかるフレーム
	ASDashFrame_ = 20;


	// ウルト中かのフラグ
	isULTFunc_ = false;

	// ウルトの移動にかかるフレーム
	ASUltFrame_ = 10;

	segment_.origin = { 0.0f, 2.0f, 0.0f };
	segment_.diff = { 5.0f, 0.0f, 10.0f };


	// レベルチェック
	LevelCheck();

	// Colliderのビットのセッティング
	SettingColliderBit();

}


// 更新処理
void Player::Update(Camera* camera)
{
	// ワールドトランスフォームの初期化
	bodyWt_.UpdateMatrix();

	// Reticleの更新処理
	reticle_->Update(camera);

	// 射撃処理
	IsAttack();

	// 移動処理
	Move();

	// 姿勢の処理
	//CalcBodyRotate();

	// ダッシュの処理
	DashFunc();

	// ウルトの処理
	ULTFunc();

	// OBBColliderのセッティング
	SettingCollider();


	Vector2 reticle = GamePadInput::GetRStick();

	//#ifdef _DEBUG

	if (ImGui::TreeNode("PlayerManual")) {

		ImGui::Text("Player_Manual");
		ImGui::Text("R_Stick : Move");
		ImGui::Text("L_Stick : Reticle");
		ImGui::Text("R_Shoulder : Attack");
		ImGui::Text("A_Button : Dash");
		ImGui::Text("");
		ImGui::Text("L_Shoulder_Keep and Up or Down : LevelPrope Change");
		ImGui::Text("L_Shoulder_Keep and A_Button : LevelUp");
		ImGui::Text("");
		ImGui::Text("Level Up kinou ga wakari zurakute sumimasen");
		ImGui::Text("L_Shoulder wo ositamama de ijiremasu");
		ImGui::TreePop();
	}
	if (ImGui::TreeNode("Player")) {

		ImGui::Text("WorldTransform");
		ImGui::DragFloat3("Scale", &bodyWt_.scale.x, 0.01f, 0.0f, 10.0f);
		ImGui::DragFloat3("Rotate", &bodyWt_.rotate.x, 0.01f);
		ImGui::DragFloat3("Translate", &bodyWt_.translate.x, 0.01f);

		ImGui::Text("");
		if (bulletType_ == TypeNormal) {
			ImGui::Text("BulletType= Normal");
		}
		else if (bulletType_ == TypePenetration) {
			ImGui::Text("BUlletType= Penetration");
		}
		else if (bulletType_ == TypeShotgun) {
			ImGui::Text("BulletType = Shotgun");
		}

		ImGui::Text("");
		ImGui::Text("Energy = %d", energy_);

		ImGui::Text("");
		ImGui::Text(" HPLv = %d : HP = %d", hpLv_, hp_);
		ImGui::Text("VelLv = %d : Val = %.2f", velLv_, moveVector_);
		ImGui::Text("AttLv = %d : attack = %d", attackLv_, attack_);
		ImGui::Text("AtILv = %d : Interval = %d", attackIntervalLv_, AttackInterval_);
		ImGui::Text("DasLv = %d : Dash = %.1f", dashLv_, toDash_);

		ImGui::Text("");
		ImGui::Text("ULT");
		ImGui::Checkbox("isULT_Func", &isULTFunc_);
		ImGui::Text("ArrayCount = %d", ultArrayCount_);

		ImGui::Text("");
		ImGui::Text("Segment");
		ImGui::DragFloat3("origin", &segment_.origin.x, 0.1f);
		ImGui::DragFloat3("diff", &segment_.diff.x, 0.1f);

		ImGui::TreePop();
	}

	//#endif // _DEBUG
}


// 描画処理
void Player::Draw3D(Camera* camera)
{
	// Bodyの描画
	bodyModel_->Draw(bodyWt_, camera);

	// Reticleの初期化
	reticle_->Draw3D(camera);


	DrawSystem::Line(segment_, camera, { 1.0f, 0.0f, 0.0f, 1.0f });
}
void Player::Draw2DFront(Camera* camera)
{
	camera;
}


// 衝突判定
void Player::OnCollision(uint32_t id)
{
	if (id == ObjectId::EnemyBullet) {
		hp_ -= 1;
	}
}


// エネミーとの個別衝突判定
void Player::OnCollisionWithEnemy(IEnemy* enemy)
{
	enemy;
	/*if (enemy->) {
		energy_ += 10;
	}*/
}

void Player::OnCollisionWithEnemyBullet()
{
	hp_ -= 1;

}


// Colliderのビットのセッティング
void Player::SettingColliderBit()
{
	OBBCollider::SetCollosionAttribute(ObjectAttribute::Player);
	OBBCollider::SetCollisionMask(ObjectMask::Player);
	OBBCollider::SetID(ObjectId::Player);
}


// OBBColliderのセッティング
void Player::SettingCollider()
{
	// OBBのサイズの設定
	this->size_ = 2.0f * bodyWt_.scale;
	OBBCollider::SetSize(this->size_);
	OBBCollider::SetRotate(bodyWt_.rotate);
}


// 移動処理
void Player::Move() {

	// velocityは0で毎フレーム初期化
	velocity_ = Vector3::zero;

	// ウルト中は移動の入力を受け付けない
	if (!isULTFunc_) {
		// キーの処理
		if (KeysInput::PressKeys(DIK_W))
		{
			velocity_.z = moveVector_;
		};
		if (KeysInput::PressKeys(DIK_A))
		{
			velocity_.x = -moveVector_;
		};
		if (KeysInput::PressKeys(DIK_S))
		{
			velocity_.z = -moveVector_;
		};
		if (KeysInput::PressKeys(DIK_D))
		{
			velocity_.x = moveVector_;
		};

		// パッドの処理
		if (GamePadInput::GetLStick().x <= -0.3f)
		{
			velocity_.x = -1.0f;
		}
		if (GamePadInput::GetLStick().x >= 0.3f)
		{
			velocity_.x = 1.0f;
		}
		if (GamePadInput::GetLStick().y <= -0.3f)
		{
			velocity_.z = -1.0f;
		}
		if (GamePadInput::GetLStick().y >= 0.3f)
		{
			velocity_.z = 1.0f;
		}
	}

	// 正規化
	if (velocity_.x != 0.0f || velocity_.z != 0.0f) {
		float length = Length({ velocity_.x, velocity_.z });
		velocity_.x /= length;
		velocity_.z /= length;
	}

	// velocityに速度を掛けて座標に加算
	bodyWt_.translate += (velocity_ * moveVector_);
}


// プレイヤー本体の姿勢処理
void Player::CalcBodyRotate()
{
	Vector2 addRotate{};

	if (GamePadInput::GetLStick().x <= -0.1f ||
		GamePadInput::GetLStick().x >= 0.1f)
	{
		addRotate.x = GamePadInput::GetLStick().x;
	}

	if (GamePadInput::GetLStick().y <= -0.1f ||
		GamePadInput::GetLStick().y >= 0.1f)
	{
		addRotate.y = GamePadInput::GetLStick().y;
	}

	// Y軸周り角度(θy)
	bodyWt_.rotate.y = std::atan2(addRotate.x, addRotate.y);
}


// ダッシュの処理
void Player::DashFunc()
{
	// ダッシュ先のベクトル取得
	if (GamePadInput::TriggerButton(PadData::A)) {
		Log("Button_A\n");

		// ボタン押下時にフラグが立っていなかったら
		if (!isDash_) {

			// フラグを立てる
			isDash_ = true;

			// 移動先の座標の計算
			toDashPosition_ = CalcToDashPos() + bodyWt_.GetWorldPos();

			// ダッシュ処理開始時の座標
			toInitDashPosition_ = bodyWt_.GetWorldPos();

			// ダッシュのフレームを０に設定
			nowDashFrame_ = 0;
		}
	}


	// ダッシュのフラグがたったら
	if (isDash_) {

		// ダッシュのフレームをインクリメン
		nowDashFrame_++;

		// 今のフレームを終了フレームで割る
		float dashTime = float(nowDashFrame_) / float(ASDashFrame_);

		// イージングで座標に加算
		bodyWt_.translate.x = toDashPosition_.x + (toDashPosition_.x - toInitDashPosition_.x) * Ease::OutExpo(dashTime);
		bodyWt_.translate.z = toDashPosition_.z + (toDashPosition_.z - toInitDashPosition_.z) * Ease::OutExpo(dashTime);


		// 終了したらフラグを折る
		if (nowDashFrame_ == ASDashFrame_) {

			isDash_ = false;
			nowDashFrame_ = 0;
		}
	}
}


// ダッシュ先の座標の計算
Vector3 Player::CalcToDashPos()
{
	Vector2 LStick{};
	if (GamePadInput::GetLStick().x <= 0.05f && GamePadInput::GetLStick().x >= -0.05f &&
		GamePadInput::GetLStick().y <= 0.05f && GamePadInput::GetLStick().y >= -0.05f) {
		LStick = { 0.0f, 1.0f };
	}
	else {
		LStick = GamePadInput::GetLStick();
	}
	Vector3 toDash = {
		bodyWt_.GetWorldPos().x + LStick.x,
		bodyWt_.GetWorldPos().y,
		bodyWt_.GetWorldPos().z + LStick.y,
	};
	Vector3 result = Normalize(toDash - bodyWt_.GetWorldPos());
	return result * toDash_;
}


// レベルチェック
void Player::LevelCheck()
{
	// HP
	hp_ = baseStatus_->LevelCheck_HP(hpLv_);

	//Vel
	moveVector_ = baseStatus_->LevelCheck_Velocity(velLv_);

	// Attack
	attack_ = baseStatus_->LevelCheck_Attack(attackLv_);

	// AttackInterval
	AttackInterval_ = baseStatus_->LevelCheck_AttackInterval(attackIntervalLv_, bulletType_);

	// Dash
	toDash_ = baseStatus_->LevelCheck_Dash(dashLv_);
}


// ウルトの処理
void Player::ULTFunc()
{
	// ULTのフラグが立っていないときにボタンの入力を受け付ける
	if (!isULTFunc_) {

		// ボタン押下でULTスタート
		if (GamePadInput::TriggerButton(PadData::X) || KeysInput::TriggerKey(DIK_X)) {

			// ULTのフラグを立てる
			isULTFunc_ = true;

			// ULTの座標の範囲を決める
			ultScope_ = { -5.0, 5.0f };

			// ULT開始時の座標を保存
			ultStartPos_ = bodyWt_.GetWorldPos();

			// ULTの移動先の座標を決める
			for (int i = 0; i < ultPos_.size(); ++i) {

				ultPos_[i] = {
					bodyWt_.GetWorldPos().x + RandomGenerator::getRandom(ultScope_),
					0.0f,
					bodyWt_.GetWorldPos().z + RandomGenerator::getRandom(ultScope_),
				};

				// 最後の要素にはスタート時の座標を入れる
				if (i == ultPos_.size() - 1) {

					ultPos_[i] = ultStartPos_;
				}
			}

			// ULT移動時の初期座標
			ultInitPos_ = bodyWt_.GetWorldPos();

			// ULTの配列カウント
			ultArrayCount_ = 0;

			// ULTにかかってるフレームを0に設定
			nowUltFrame_ = 0;
		}
	}

	// ULTが開始されたら
	if (isULTFunc_) {

		// ULTのフレームをインクリメント
		nowUltFrame_++;

		// 今のフレームを終了フレームで割る
		float ultTime = float(nowUltFrame_) / float(ASUltFrame_);

		// イージング関数で座標に加算
		bodyWt_.translate.x = 
			ultInitPos_.x + (ultPos_[ultArrayCount_].x - ultInitPos_.x) * Ease::OutSine(ultTime);
		bodyWt_.translate.z = 
			ultInitPos_.z + (ultPos_[ultArrayCount_].z - ultInitPos_.z) * Ease::OutSine(ultTime);


		// 条件分岐で終了処理
		if (nowUltFrame_ == ASUltFrame_) {

			// ウルト合計移動サイズとカウントが一緒だったら全部終了したら
			// フラグを折ってウルト終了
			if (ultPos_.size() - 1 == ultArrayCount_) {
				
				isULTFunc_ = false;

				// ULTにかかってるフレームを0に設定
				nowUltFrame_ = 0;

				// カウントを0に設定
				ultArrayCount_ = 0;
			}
			else if (ultPos_.size() - 1 != ultArrayCount_) {

				// カウントをインクリメント継続
				ultArrayCount_++;

				// ULT移動時の初期座標
				ultInitPos_ = bodyWt_.GetWorldPos();

				// ULTにかかってるフレームを0に設定
				nowUltFrame_ = 0;
			}
		}
	}
}


// レベルの変更処理
void Player::SettingLev()
{
	if (GamePadInput::TriggerButton(PadData::UP)) {
		levelPrope_ = levelPrope_ + 1;
	}
	if (GamePadInput::TriggerButton(PadData::DOWN)) {
		levelPrope_ = levelPrope_ - 1;
	}

	if (levelPrope_ > 4) {
		levelPrope_ = 0;
	}
	if (levelPrope_ < 0) {
		levelPrope_ = 4;
	}

	if (GamePadInput::TriggerButton(PadData::A)) {

		if (energy_ > 0) {

			energy_ = energy_ - 10;

			if (levelPrope_ == HP) {
				hpLv_ = baseStatus_->AddLevel(hpLv_);
			}
			else if (levelPrope_ == Vel) {
				velLv_ = baseStatus_->AddLevel(velLv_);
			}
			else if (levelPrope_ == Attack) {
				attackLv_ = baseStatus_->AddLevel(attackLv_);
			}
			else if (levelPrope_ == AttackInter) {
				attackIntervalLv_ = baseStatus_->AddLevel(attackIntervalLv_);
			}
			else if (levelPrope_ == Dash) {
				dashLv_ = baseStatus_->AddLevel(dashLv_);
			}

			totalLevel_ = baseStatus_->AddLevel(totalLevel_);
		}

		// レベルチェック
		LevelCheck();
	}


	if (GamePadInput::TriggerButton(PadData::RIGHT)) {

		if (bulletType_ == TypeNormal) {
			bulletType_ = TypePenetration;
		}
		else if (bulletType_ == TypePenetration) {
			bulletType_ = TypeShotgun;
		}
		else if (bulletType_ == TypeShotgun) {
			bulletType_ = TypeNormal;
		}
	}
	if (GamePadInput::TriggerButton(PadData::LEFT)) {

		if (bulletType_ == TypeNormal) {
			bulletType_ = TypeShotgun;
		}
		else if (bulletType_ == TypePenetration) {
			bulletType_ = TypeNormal;
		}
		else if (bulletType_ == TypeShotgun) {
			bulletType_ = TypePenetration;
		}
	}


	//#ifdef _DEBUG

	ImGui::Begin("PlayerLevel");
	ImGui::Text("");
	ImGui::Text("Energy = %d", energy_);

	ImGui::Text("");
	if (levelPrope_ == HP) {
		ImGui::Text("LevelPrope = HP");
	}
	else if (levelPrope_ == Vel) {
		ImGui::Text("LevelPrope = Vel");
	}
	else if (levelPrope_ == Attack) {
		ImGui::Text("LevelPrope = Attack");
	}
	else if (levelPrope_ == AttackInter) {
		ImGui::Text("LevelPrope = AttackInterval");
	}
	else if (levelPrope_ == Dash) {
		ImGui::Text("LevelPrope = Dash");
	}
	ImGui::Text(" HPLv = %d : val = %d", hpLv_, hp_);
	ImGui::Text("VelLv = %d : val = %.2f", velLv_, moveVector_);
	ImGui::Text("AttLv = %d : val = %d", attackLv_, attack_);
	ImGui::Text("AtILv = %d : val = %d", attackIntervalLv_, AttackInterval_);
	ImGui::Text("DasLv = %d : val = %.1f", dashLv_, toDash_);
	ImGui::Text("TotalLv = %d", totalLevel_);

	ImGui::Text("");
	if (bulletType_ == TypeNormal) {
		ImGui::Text("BulletType= Normal");
	}
	else if (bulletType_ == TypePenetration) {
		ImGui::Text("BulletType= Penetration");
	}
	else if (bulletType_ == TypeShotgun) {
		ImGui::Text("BulletType = Shotgun");

	}

	ImGui::End();

	//#endif // _DEBUG
}


// 射撃処理
void Player::IsAttack()
{
	// 射撃タイマーを減らす
	AttackTimer_--;

	// キー押下でタイマーを減らし射撃
	if (KeysInput::PressKeys(DIK_SPACE) && AttackTimer_ <= 0) {
		// 設定し直す
		AttackTimer_ = AttackInterval_;
		// バレットの設定をして射撃
		SettingNewBullet();
	}
	else if (KeysInput::ReleaseKeys(DIK_SPACE)) {

		// キーを離したら次の瞬間押してすぐ射撃できるように
		// １フレームを入れておく
		AttackTimer_ = 1;
	}

	// ボタン押下でタイマーを減らし射撃
	if (GamePadInput::PressButton(PadData::RIGHT_SHOULDER) && AttackTimer_ <= 0) {
		// 設定し直す
		AttackTimer_ = AttackInterval_;
		// バレットの設定をして射撃
		SettingNewBullet();
	}
	else if (GamePadInput::ReleaseButton(PadData::RIGHT_SHOULDER)) {

		// キーを離したら次の瞬間押してすぐ射撃できるように
		// １フレームを入れておく
		AttackTimer_ = 1;
	}
}


// バレットの設定
void Player::SettingNewBullet()
{
	// バレットタイプで処理分岐
	if (bulletType_ == TypeNormal || bulletType_ == TypePenetration) {

		// バレットのインスタンス作成
		shared_ptr<IPlayerBullet> newbullet = make_unique<PenetrationBullet>();

		// 初期座標
		Vector3 initPos = bodyWt_.GetWorldPos();
		// 移動速度
		Vector3 vel = CalcBulletDirection();

		// Bulletの初期化処理
		newbullet->Initialize(this, (*bulletModel_), initPos, vel, attack_);

		// シーンに登録
		registerScene_->AddPlayerBullets(newbullet);
	}
	else if (bulletType_ == TypeShotgun) {

		for (int i = 0; i < pelletNum_; i++) {

			// バレットのインスタンス作成
			shared_ptr<IPlayerBullet> newbullet = make_unique<Shotgun>();

			// 初期座標
			Vector3 initPos = bodyWt_.GetWorldPos();
			// 移動速度
			Vector3 vel = CalcBulletDirection();

			// Bulletの初期化処理
			newbullet->Initialize(this, (*bulletModel_), initPos, vel, attack_);

			// シーンに登録
			registerScene_->AddPlayerBullets(newbullet);
		}
	}
}


// バレットの進行方向の計算
Vector3 Player::CalcBulletDirection()
{
	Vector3 ReticlePos{};
	if (bulletType_ == TypeNormal || bulletType_ == TypePenetration) {

		ReticlePos = reticle_->GetWorldPos();
	}
	else if (bulletType_ == TypeShotgun) {

		if (GamePadInput::GetRStick().y <= 0.3f && GamePadInput::GetRStick().y >= -0.3f) {
			ReticlePos = reticle_->GetWorldPos() + RandomGenerator::getRandom({ { -0.05f, 0.05f, }, { 0.0f, 0.0f }, { 0.0f, 0.0f } });
		}
		else {
			ReticlePos = reticle_->GetWorldPos() + RandomGenerator::getRandom({ { -2.0f, 2.0f, }, { 0.0f, 0.0f }, { -2.0f, 2.0f } });
		}

	}
	Vector3 PlaToRet = Normalize(ReticlePos - bodyWt_.GetWorldPos());
	return PlaToRet * bulletVelocity_;
}
