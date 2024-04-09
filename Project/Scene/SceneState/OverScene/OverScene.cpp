#include "OverScene.h"



// 初期化処理
void OverScene::Initialize()
{
	// メインカメラ
	camera_ = make_unique<Camera>();
	camera_->Initialize();

}


// 更新処理
void OverScene::Update(GameManager* state)
{
	/* ----- GameCamera ゲームカメラ----- */
	camera_->UpdateMatrix();


	// ボタン押下でシーンチェンジ
	if (GamePadInput::PressButton(PadData::RIGHT)) {

		if (GamePadInput::TriggerButton(PadData::X)) {
			state->ChangeSceneState(new SelectScene());
		}
		if (GamePadInput::TriggerButton(PadData::Y)) {
			state->ChangeSceneState(new GameScene());
		}
	}


#ifdef _DEBUG

	ImGui::Begin("OverScene");
	ImGui::Text("Scene change to 'SelectScene' by pressing X button while holding down Right.");
	ImGui::Text("Scene change to 'GameScene' by pressing Y button while holding down Right.");
	ImGui::End();

#endif // _DEBUG
}


// 背景スプライトの描画処理
void OverScene::BackSpriteDraw()
{

}


// ３Dオブジェクトの描画処理
void OverScene::ModelDraw()
{

}


// 前景スプライトの描画処理
void OverScene::FrontSpriteDraw()
{

}