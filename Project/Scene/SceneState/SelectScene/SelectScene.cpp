#include "SelectScene.h"



// 初期化処理
void SelectScene::Initialize()
{
	// メインカメラ
	camera_ = make_unique<Camera>();
	camera_->Initialize();

}


// 更新処理
void SelectScene::Update(GameManager* state)
{
	/* ----- GameCamera ゲームカメラ----- */
	camera_->UpdateMatrix();


	// ボタン押下でシーンチェンジ
	if (GamePadInput::TriggerButton(PadData::X)) {
		state->ChangeSceneState(new ManualScene());
	}
	if (GamePadInput::TriggerButton(PadData::A)) {
		state->ChangeSceneState(new GameScene());
	}

#ifdef _DEBUG

	ImGui::Begin("SelectScene");
	ImGui::Text("Scene change to 'ManualScene' by pressing X button.");
	ImGui::Text("Scene change to 'GameScene' by pressing A button.");
	ImGui::End();

#endif // _DEBUG
}


// 背景スプライトの描画処理
void SelectScene::BackSpriteDraw()
{

}


// ３Dオブジェクトの描画処理
void SelectScene::ModelDraw()
{

}


// 前景スプライトの描画処理
void SelectScene::FrontSpriteDraw()
{

}