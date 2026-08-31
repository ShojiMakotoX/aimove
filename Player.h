#pragma once
#include "Library\GameObject.h"
#include "global.h"

class Player :
    public GameObject
{
public:
	Player();
	~Player();
	void Update() override;
	void Draw() override;
	Point GetPlayerPos() { return pos_; }
	
private:
	int hImage_;//画像ID
	Point pos_;
	DIR dir_;//移動方向
	int life_;//ダメージを使うときに
};

