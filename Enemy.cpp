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
	const int CHASE_RANGE = 100;
	const int RADIUS = 100;
	const float SEARCH_TIME = 3.0f;
	const float SEARCH_INTERVAL = 0.75f;
	const float PLAYER_LOST = 3.0f;
	
	

}

Enemy::Enemy()
	: GameObject() 
{
	hImage_ = LoadGraph("Assets/panda_R.png");
	pos_ = ENEMY_START_POS; //32はブロックの位置pos_
	dir_ = INIT_ENEMY_DIR;
	searchCount_ = 0;
	searchTimer_ = 0.0f;
	lostTime_ = 0.0f;

	

	
}

Enemy::~Enemy()
{
}

void Enemy::Update()
{
	switch (state_)
	{
	case PATROL:
		Patrol();
		break;
	case CHASE:
		Chase();
		break;
	case ATTACK:
		Attack();
		break;
	case SEARCH:
		Search();
		break;
	}
	
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
		if (CanMove(dir_))
		{
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
		}
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
	int radius = RADIUS;
	float startAngle = -DX_PI / 4;
	float endAngle = DX_PI / 4;//DX_PIが180である

	for (int y = 0;y < STAGE_HEIGHT;y++)
	{
		for (int x = 0;x < STAGE_WIDTH;x++)
		{
			int tileCenterX = x * 32 + 16;
			int tileCenterY = y * 32 + 16;

			int dx = tileCenterX - centerX;
			int dy = tileCenterY - centerY;

			float distance = sqrtf(dx * dx + dy * dy);

			float angle = atan2f(dy, dx);//マスまでの角度を求める
			if (angle < 0)
			{
				angle += DX_TWO_PI;//2πを足す
			}
			float brightness = 255.0f * (1.0f - distance / RADIUS);
			if (brightness < 0)
			{
				brightness = 0;
			}
			int color = GetColor((int)brightness, (int)brightness, 0);

			/*if (distance <= RADIUS && angle >= startAngle && angle <= endAngle)
			{
				DrawBox(x * 32, y * 32, x * 32 + 32, y * 32 + 32, color, TRUE);
			}*/
			
		}

	}

	//dir_に応じて角度を変更する。
	switch (dir_)
	{
	case RIGHT://見えている範囲　-45度から+45度
		 startAngle = -DX_PI / 4;
		 endAngle = DX_PI / 4;
		break;
	case DOWN://見えている範囲　45度から135度
		startAngle = DX_PI / 4;
		endAngle = 3*DX_PI / 4;
		break;
	case LEFT://見えている範囲　135度から225度
		startAngle = 3*DX_PI / 4;
		endAngle = 5*DX_PI / 4;
		break;
	case UP://見えている範囲　225度から315度
		startAngle = 5*DX_PI / 4;
		endAngle = 7*DX_PI / 4;
		break;
	}

	//扇形の描画
	float prevX = centerX + cosf(startAngle) * radius;
	float prevY = centerY + sinf(startAngle) * radius;

	for (float angle = startAngle;angle <= endAngle;angle += DX_PI / 90)//90度以下ならば
	{
		float x = centerX + cosf(angle) * radius;
		float y = centerY + sinf(angle) * radius;

		SetDrawBlendMode(DX_BLENDMODE_ALPHA, 128);
		DrawTriangle(centerX, centerY, (int)prevX,(int)prevY,(int)x, (int)y, GetColor(255, 255, 224),TRUE);

		prevX = x;
		prevY = y;
	}
	SetDrawBlendMode(DX_BLENDMODE_NOBLEND, 0);
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

	int dist = abs(p.x - testPos.x) + abs(p.y - testPos.y);

	smalldist = distright;
	bestdir = RIGHT;

	if (dist <= CHASE_RANGE)
	{
		lostTime_ = 0.0f;
		if (distleft < smalldist)
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
		if (p.x == pos_.x && p.y == pos_.y)
		{
			state_ = ATTACK;
		}
	}
	else
	{
		lostTime_ += Time::DeltaTime();
		if (lostTime_ >= PLAYER_LOST)
		{
			state_ = SEARCH;
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

void Enemy::Patrol()
{
	Player* player = FindGameObject<Player>();
	Point p = player->GetPlayerPos();

	int dist = abs(p.x - pos_.x) + abs(p.y - pos_.y);

	if (dist <= CHASE_RANGE)
	{
		state_ = CHASE;
		return;
	}

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

void Enemy::Attack()
{
	Player* player = FindGameObject<Player>();

	Point p = player->GetPlayerPos();

	if (p.x == pos_.x && p.y == pos_.y)
	{
		player->DestroyMe();
		SceneManager::ChangeScene("GAMEOVER");
	}
}

void Enemy::Search()
{
	//Player* player = FindGameObject<Player>();

	searchTimer_ += Time::DeltaTime();

	if (searchTimer_>=SEARCH_INTERVAL)
	{
		searchTimer_ = 0.0f;
		if (searchCount_ == 0)
		{
			dir_ = TurnRight(dir_);
		}
		else if (searchCount_ == 1)
		{
			dir_ = TurnLeft(dir_);
		}
		else if (searchCount_ == 2)
		{
			dir_ = TurnBack(dir_);
		}
		else
		{
			searchCount_ = 0;
			searchTimer_ = 0.0f;

			state_ = PATROL;

			return;
		}
		searchCount_++;
	}


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
