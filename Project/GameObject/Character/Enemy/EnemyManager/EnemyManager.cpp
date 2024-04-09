#include "EnemyManager.h"
#include <cassert>
#include <fstream>
#include "GameScene/GameScene.h"


void EnemyManager::LoadEnemyPopData() {
	// ファイルを開く
	std::ifstream file{};
	file.open("./resources./csv./enemyPop.csv");
	assert(file.is_open());

	// ファイルの内容を文字列ストリームにコピー
	popCommands << file.rdbuf();

	// ファイルを閉じる
	file.close();
}

void EnemyManager::UpdateEnemyPopCommands(Player* player,int level) {
	// 1行分の文字列を入れる変数名
	std::string line;

	// コマンド実行ループ
	while (getline(popCommands, line)) {
		// 1行分の文字列をストリームに変換して解析しやすくする
		std::istringstream line_stream(line);

		std::string word;
		// ,区切りで行の戦闘文字列を取得
		getline(line_stream, word, ',');

		// "//"から始まる行はコメント
		if (word.find("//") == 0) {
			// コメント行を飛ばす
			continue;
		}

		// TraceEnemyのPOPコマンド
		if (word.find("TRACEPOP") == 0) {
			// x座標
			getline(line_stream, word, ',');
			float x = (float)std::atof(word.c_str());

			// y座標
			getline(line_stream, word, ',');
			float y = (float)std::atof(word.c_str());

			// z座標
			getline(line_stream, word, ',');
			float z = (float)std::atof(word.c_str());

			// 敵を発生させる
			Trace::Spawn(player,Vector3(x, y, z), level);

		}

		// BlastEnemyのPOPコマンド
		if (word.find("BLASTPOP") == 0) {
			// x座標
			getline(line_stream, word, ',');
			float x = (float)std::atof(word.c_str());

			// y座標
			getline(line_stream, word, ',');
			float y = (float)std::atof(word.c_str());

			// z座標
			getline(line_stream, word, ',');
			float z = (float)std::atof(word.c_str());

			// 敵を発生させる
			Blast::Spawn(player, Vector3(x, y, z), level);
		}
	}
}

EnemyManager::EnemyManager() {
}

EnemyManager::~EnemyManager() {
	for (IEnemy* enemy : enemies_) {
		delete enemy;
	}
}

void EnemyManager::EnemySpawn(Player* player) {
	flame_++;

	traceMax_ = 10;
	blastMax_ = 10;

	for (int i = 0; i < MAX_ENEMY; i++) {
		/*Fire* newFire = new Fire();

		float rad = 30;

		Scope scope = { rad / 2,rad };
		rad_ = RandomGenerator::getRandom(scope);
		tmpPos_ = CalculateNextPosition(player->GetWorldPos(), rad_);

		newFire->SetGameScene(gameScene);
		newFire->Initialize(player, tmpPos_, level_);

		enemies_.push_back(newFire);*/

		/////////////////////////////////////////////////
		Trace* newTrace = new Trace();

		float rad = 30;

		Scope scope = { rad / 2,rad };
		rad_ = RandomGenerator::getRandom(scope);
		tmpPos_ = CalculateNextPosition(player->GetWorldPos(), rad_);

		newTrace->SetGameScene(gameScene);
		newTrace->Initialize(player, tmpPos_, level_);

		enemies_.push_back(newTrace);
		////////////////////////////////////////////////////////////////////

		//Mortar* newMortar = new Mortar();

		////float rad = 30;

		////Scope scope = { rad / 2,rad };
		//rad_ = RandomGenerator::getRandom(scope);
		//tmpPos_ = CalculateNextPosition(player->GetWorldPos(), rad_);

		//newMortar->SetGameScene(gameScene);
		//newMortar->Initialize(player, tmpPos_, level_);

		//enemies_.push_back(newMortar);
	}

	/*for (int i = 0; i < traceMax_; i++) {
		trace_.push_back(std::make_unique<Trace>());
		float rad = 30;

		Scope scope = { rad / 2,rad };
		rad_ = RandomGenerator::getRandom(scope);
		tmpPos_ = CalculateNextPosition(player->GetWorldPos(), rad_);

		trace_[i]->Initialize(player, tmpPos_, level);
	}

	for (int i = 0; i < blastMax_; i++) {
		blast_.push_back(std::make_unique<Blast>());
		float rad = 30;

		Scope scope = { rad / 2,rad };
		rad_ = RandomGenerator::getRandom(scope);
		tmpPos_ = CalculateNextPosition(player->GetWorldPos(), rad_);


		blast_[i]->Initialize(player, tmpPos_, level);
	}*/

}

void EnemyManager::Initialize()
{

}

Vector3 EnemyManager::CalculateNextPosition(const Vector3& position, float radius) {
	// 乱数生成器の準備
	static std::random_device rd;
	static std::mt19937 gen(rd());
	static std::uniform_real_distribution<float> dis(-1.0f, 1.0f);

	// ランダムな方向ベクトルを生成し、長さをradiusにスケーリングする
	float dx = dis(gen) * radius;
	float dy = dis(gen) * radius;

	// 予測移動先座標を計算する
	float nextX = position.x + dx;
	float nextZ = position.z + dy;

	return { nextX, 0,nextZ };
}

void EnemyManager::Update() {
	
	for (IEnemy* enemy : enemies_) {
		enemy->Update();
	}

	enemies_.remove_if([](IEnemy* enemy) {
		if (!enemy->IsAlive()) {
			delete enemy;
			return true;
		}
		return false;
		});
}

void EnemyManager::Draw(Camera* camera) {
	for (IEnemy* enemy : enemies_) {
		enemy->Draw(camera);
	}
}
