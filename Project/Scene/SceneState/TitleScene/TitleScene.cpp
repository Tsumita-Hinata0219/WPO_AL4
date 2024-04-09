#include "TitleScene.h"



// 初期化処理
void TitleScene::Initialize()
{
	// メインカメラ
	camera_ = make_unique<Camera>();
	camera_->Initialize();
	
}


// 更新処理
void TitleScene::Update(GameManager* state)
{
	/* ----- GameCamera ゲームカメラ----- */
	camera_->UpdateMatrix();


	// ボタン押下でシーンチェンジ
	if (GamePadInput::TriggerButton(PadData::A)) {
		state->ChangeSceneState(new SelectScene());
	}

#ifdef _DEBUG

	ImGui::Begin("TitleScene");
	ImGui::Text("Scene change to 'SelectScene' by pressing A button.");
	ImGui::End();

#endif // _DEBUG
}


// 背景スプライトの描画処理
void TitleScene::BackSpriteDraw()
{

}


// ３Dオブジェクトの描画処理
void TitleScene::ModelDraw()
{
	
}


// 前景スプライトの描画処理
void TitleScene::FrontSpriteDraw()
{
	
}
