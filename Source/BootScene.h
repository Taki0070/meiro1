#pragma once

#include "../Engine/SceneBase.h"

class BootScene : public SceneBase
{
public:
	BootScene();
	~BootScene();
	void Update() override;
	void Draw() override;
};