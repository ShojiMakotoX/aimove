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
}

void GameOverScene::Draw()
{
	DrawString(0, 0, "GameOver", GetColor(255, 0, 0));
	DrawString(100, 400, "Push [P]Key To Play", GetColor(255, 255, 255));
}
