#pragma once
#include ".\Library\GameObject.h"
#include "global.h"


class Enemy :
    public GameObject
{
	int hImage_;//画像ID
	Point pos_;//位置
	DIR dir_;//移動方向
public:
	Enemy();
	~Enemy();
	void Update() override;
	void Draw() override;
	void Chase();//プレイヤーを追跡する
private:
	DIR turnright_;

};

