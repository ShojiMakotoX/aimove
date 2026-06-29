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
	void Patrol();//プレイヤーパトロール
	void Attack();
	void Search();
private:
	DIR TurnRight(DIR d);
	DIR TurnLeft(DIR d);
	DIR TurnBack(DIR d);
	bool CanMove(DIR dir);

	int distright;
	int distleft;
	int distup;
	int distdown;

	int smalldist;
	DIR bestdir;
};

