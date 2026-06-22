#include "Enemy.h"
#include "time.h"
#include "Stage.h"
#include "Player.h"

namespace
{
	const int ENEMY_SIZE = 48; //敵のサイズ 32*32
	const Point ENEMY_START_POS = { 20 * ENEMY_SIZE, 10 * ENEMY_SIZE }; //敵の初期位置
	const DIR INIT_ENEMY_DIR = { LEFT };
	const int ENEMY_DRAW_SIZE = 32; //敵の描画サイズ
	const int animFrame[4]{ 0, 1, 2, 1 };
	const float ANIM_INTERVAL = 0.2f;
}


Enemy::Enemy()
	: GameObject() 
{
	hImage_ = LoadGraph("Assets/panda_R.png");
	pos_ = ENEMY_START_POS; //32はブロックの位置pos_
	dir_ = INIT_ENEMY_DIR;
}

Enemy::~Enemy()
{
}

void Enemy::Update()
{
	//GetRand(数値)
	//3秒に1回向きをランダムに変える
	static float dir_timer = 3.0f;
	static float prog_timer = 0.5f;
	float dt = Time::DeltaTime();
	dir_timer = dir_timer - dt;
	prog_timer = prog_timer - dt;
	if (dir_timer < 0.0f)
	{
		//dir_ = (DIR)(GetRand(3));
		dir_timer = 3.0f + dir_timer;
	}

	if (prog_timer < 0.0f)
	{

		Chase();
		
		//ここで1マス移動する
		Point newPos = pos_;

		switch (dir_)
		{
		case UP:
			newPos.y -= ENEMY_DRAW_SIZE;
			break;
		case DOWN:
			newPos.y += ENEMY_DRAW_SIZE;
			break;
		case LEFT:
			newPos.x -= ENEMY_DRAW_SIZE;
			break;
		case RIGHT:
			newPos.x += ENEMY_DRAW_SIZE;
			break;
		default:
			break;
		}
		pos_ = newPos;
		prog_timer = 0.25f + prog_timer;
	}

}

void Enemy::Draw()
{
	static float animTimer = ANIM_INTERVAL;
	static int frame = 0;
	int nowFrame = animFrame[frame];

	Rect iRect[4] = {
		{  nowFrame * ENEMY_SIZE, 3 * ENEMY_SIZE, ENEMY_SIZE, ENEMY_SIZE},
		{  nowFrame * ENEMY_SIZE, 2 * ENEMY_SIZE, ENEMY_SIZE, ENEMY_SIZE},
		{  nowFrame * ENEMY_SIZE, 0 * ENEMY_SIZE, ENEMY_SIZE, ENEMY_SIZE},
		{  nowFrame * ENEMY_SIZE, 1 * ENEMY_SIZE, ENEMY_SIZE, ENEMY_SIZE}
	};
	DrawBox(pos_.x, pos_.y, pos_.x + ENEMY_DRAW_SIZE, pos_.y + ENEMY_DRAW_SIZE,
		GetColor(255, 255, 0), FALSE,2);
	DrawRectExtendGraph(pos_.x, pos_.y,pos_.x + ENEMY_DRAW_SIZE, pos_.y + ENEMY_DRAW_SIZE,
		               iRect[dir_].x, iRect[dir_].y, iRect[dir_].w, iRect[dir_].h, hImage_, TRUE);
	if (animTimer < 0) {
		frame = (++frame) % 4;
		animTimer = ANIM_INTERVAL + animTimer;
	}
	animTimer = animTimer - Time::DeltaTime();

	int centerX = pos_.x + ENEMY_DRAW_SIZE / 2;
	int centerY = pos_.y + ENEMY_DRAW_SIZE / 2;
	DrawCircle(centerX, centerY, 200, GetColor(255, 255, 0), FALSE);
}

void Enemy::Chase()
{
	Player* player = FindGameObject<Player>();
	Point p = player->GetPlayerPos();
	VECTOR playerPos = VGet(p.x, p.y, 0);
	VECTOR enemyPos = VGet(pos_.x, pos_.y, 0);

	Point testPos = pos_;//お試し距離
	testPos.x += ENEMY_DRAW_SIZE;
	distright = abs(p.x - testPos.x) + abs(p.y - testPos.y);//マンハッタン距離を求める
	
	testPos = pos_;//一回リセット
	testPos.x -= ENEMY_DRAW_SIZE;
	distleft = abs(p.x - testPos.x) + abs(p.y - testPos.y);//マンハッタン距離を求める

	testPos = pos_;
	testPos.y -= ENEMY_DRAW_SIZE;
	distup = abs(p.x - testPos.x) + abs(p.y - testPos.y);//マンハッタン距離を求める

	testPos = pos_;
	testPos.y += ENEMY_DRAW_SIZE;
	distdown = abs(p.x - testPos.x) + abs(p.y - testPos.y);//マンハッタン距離を求める

	smalldist = distright;
	bestdir = RIGHT;


	if (distleft <smalldist)
	{
		smalldist = distleft;
		bestdir = LEFT;
	}
	if (distup < smalldist)
	{
		smalldist = distup;
		bestdir = UP;
	}
	if (distdown < smalldist)
	{
		smalldist = distdown;
		bestdir = DOWN;
	}
	if (CanMove(bestdir))
	{
		dir_ = bestdir;
	}
	else
	{
		DIR right = TurnRight(dir_);

		if (CanMove(right) && !CanMove(dir_))
		{
			dir_ = right;
		}
		else if (CanMove(right))
		{

		}
		else if (CanMove(TurnLeft(dir_)))
		{
			dir_ = TurnLeft(dir_);
		}
		else
		{
			dir_ = TurnBack(dir_);
		}
	}

	
	/*DIR right = TurnRight(dir_);

	if (CanMove(right) && !CanMove(dir_))
	{
		dir_ = right;
	}
	else if (CanMove(right))
	{

	}
	else if (CanMove(TurnLeft(dir_)))
	{
		dir_ = TurnLeft(dir_);
	}
	else
	{
		dir_ = TurnBack(dir_);
	}
*/


}

DIR Enemy::TurnRight(DIR d)
{
	return (DIR)((d + 1 )% 4);
}

DIR Enemy::TurnLeft(DIR d)
{
	return DIR((d + 3) % 4);
}

DIR Enemy::TurnBack(DIR d)
{
	return DIR((d + 2) % 4);
}

bool Enemy::CanMove(DIR dir)
{
	Point newPos = pos_;
	switch (dir)
	{
	case UP:
		newPos.y -= ENEMY_DRAW_SIZE;
		break;
	case DOWN:
		newPos.y += ENEMY_DRAW_SIZE;
		break;
	case LEFT:
		newPos.x -= ENEMY_DRAW_SIZE;
		break;
	case RIGHT:
		newPos.x += ENEMY_DRAW_SIZE;
		break;
	default:
		break;
	}
	int mapValue = FindGameObject<Stage>()->GetMap(newPos.x / ENEMY_DRAW_SIZE, newPos.y / ENEMY_DRAW_SIZE);
	return (mapValue != 1);
}
