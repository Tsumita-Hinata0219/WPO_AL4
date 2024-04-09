#pragma once

#include "IScene.h"
#include "GameManager.h"
#include "GameObject.h"


class ClearScene : public IScene {

public:

	/// <summary>
	/// コンストラクタ
	/// </summary>
	ClearScene() {};

	/// <summary>
	/// デストラクタ
	/// </summary>
	~ClearScene() {};

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

private:

	// メインカメラ
	unique_ptr<Camera> camera_ = nullptr;
};

