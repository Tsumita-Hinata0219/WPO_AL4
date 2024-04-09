#include "ManualScene.h"



// 初期化処理
void ManualScene::Initialize()
{
	// メインカメラ
	camera_ = make_unique<Camera>();
	camera_->Initialize();

}


// 更新処理
void ManualScene::Update(GameManager* state)
{
	/* ----- GameCamera ゲームカメラ----- */
	camera_->UpdateMatrix();

	
	// ボタン押下でシーンチェンジ
	if (GamePadInput::TriggerButton(PadData::B)) {
		state->ChangeSceneState(new SelectScene());
	}

#ifdef _DEBUG

	ImGui::Begin("ManualScene");
	ImGui::Text("Scene change to 'SelectScene' by pressing B button.");
	ImGui::End();

#endif // _DEBUG
}


// 背景スプライトの描画処理
void ManualScene::BackSpriteDraw()
{

}


// ３Dオブジェクトの描画処理
void ManualScene::ModelDraw()
{

}


// 前景スプライトの描画処理
void ManualScene::FrontSpriteDraw()
{

}
