#include "GameOverScene.h"
#include <DxLib.h>


GameOverScene::GameOverScene()
{
}

GameOverScene::~GameOverScene()
{
}

void GameOverScene::Update()
{
	if (CheckHitKey(KEY_INPUT_R))
	{
		SceneManager::ChangeScene("TITLE");
	}
}

void GameOverScene::Draw()
{
	SetFontSize(50);
	DrawString(100, 300, "GameOver", GetColor(255, 0, 0));
	SetFontSize(16);
	DrawString(300, 400, "Push [r]Key To title", GetColor(255, 255, 255));
}
