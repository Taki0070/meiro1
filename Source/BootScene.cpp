#include "BootScene.h"
#include "../Engine/SceneManager.h"

BootScene::BootScene()
{
}

BootScene::~BootScene()
{
}

void BootScene::Update()
{
	SceneManager::ChangeScene("PLAY"); // 起動が終わったらTitleを表示
}

void BootScene::Draw()
{
}
