#include "Enemy.h"
#include "./Stage.h"
#include "globals.h"
#include"Player.h"
#include<vector>


std::vector<Point> route_;
namespace
{
	Point nDir[4] = { {0,-2},{0,2},{-2,0},{2,0} };//移動方向
}

Enemy::Enemy()
	:pos_({ 0,0 }), isAlive_(true), nextPos_({ 0,0 })
{
	int rx = 0;
	int ry = 0;
	while (rx % 2 == 0 || ry % 2 == 0)//どちらかが偶数
	{
		rx = GetRand(STAGE_WIDTH - 1);
		ry = GetRand(STAGE_HEIGHT - 1);
	}

	pos_ = { rx * CHA_WIDTH, ry * CHA_HEIGHT };
	forward_ = RIGHT;
}

Enemy::~Enemy()
{
}

void Enemy::Update()
{
	Player* player = (Player*)FindGameObject<Player>();
	Point pPos = player->GetPos();
	Stage* stage = (Stage*)FindGameObject<Stage>();
	static bool stop = false;

	if (!stop) {
		int ex = pos_.x / CHA_WIDTH;
		int ey = pos_.y / CHA_HEIGHT;

		int px = pPos.x / CHA_WIDTH;
		int py = pPos.y / CHA_HEIGHT;

		////幅優先探索
		//stage->BFS({ ex,ey }, { px,py });
		//route_ = stage->restore({ ex,ey }, { px,py });

		//ダイクストラ
		stage->Dijkstra({ ex, ey });
		route_ = stage->restore(px, py);

		stop = true;
	}


	if (!route_.empty()) 
	{
		// ターゲット移動
		Point nextTarget = route_.front();
		MOVE(nextTarget);

		if (pos_.x == nextTarget.x * CHA_WIDTH && pos_.y == nextTarget.y * CHA_HEIGHT)
		{
			route_.erase(route_.begin()); 
		}
	}
	else {
		stop = false;
	}

}

void Enemy::Draw()
{
	DrawBox(pos_.x, pos_.y, pos_.x + CHA_WIDTH, pos_.y + CHA_HEIGHT, 
		GetColor(80, 89, 10), TRUE);
	Point tp[4][3] = {
		{{pos_.x + CHA_WIDTH / 2, pos_.y},              {pos_.x, pos_.y + CHA_HEIGHT / 2}, {pos_.x + CHA_WIDTH, pos_.y + CHA_HEIGHT / 2}},
		{{pos_.x + CHA_WIDTH / 2, pos_.y + CHA_HEIGHT}, {pos_.x, pos_.y + CHA_HEIGHT / 2}, {pos_.x + CHA_WIDTH, pos_.y + CHA_HEIGHT / 2}},
		{{pos_.x            , pos_.y + CHA_HEIGHT / 2}, {pos_.x + CHA_WIDTH / 2, pos_.y}, {pos_.x + CHA_WIDTH/2, pos_.y  + CHA_HEIGHT}},
		{{pos_.x + CHA_WIDTH, pos_.y + CHA_HEIGHT / 2}, {pos_.x + CHA_WIDTH / 2, pos_.y}, {pos_.x + CHA_WIDTH/2, pos_.y + CHA_HEIGHT}}
					};

	DrawTriangle(tp[forward_][0].x, tp[forward_][0].y, tp[forward_][1].x, tp[forward_][1].y, tp[forward_][2].x, tp[forward_][2].y, GetColor(255,255,255), TRUE);
}



void Enemy::MOVE(Point p)
{
	int dx = p.x * CHA_WIDTH;
	int dy = p.y * CHA_HEIGHT;

	//移動量を計算
	if (pos_.x != dx)
	{
		if (dx > pos_.x) {
			pos_.x += 2;
			forward_ = RIGHT;
		}
		else {
			pos_.x -= 2;
			forward_ = LEFT;
		}
	}

	else if (pos_.y != dy) 
	{
		if (dy > pos_.y) {
			pos_.y +=  2;
			forward_ = DOWN;
		}
		else {
			pos_.y -= 2;
			forward_ = UP;
		}
	}
}

void Enemy::RightPointMove()
{
	/* 右の壁に沿って歩いていく
	https://qiita.com/2dgames_jp/items/f29e915357c1decbc4b7
     */

	DIR myRight[4] = { RIGHT, LEFT, UP, DOWN };
	DIR myLeft[4] = { LEFT, RIGHT, DOWN, UP };
	Point nposF = { pos_.x + nDir[forward_].x, pos_.y + nDir[forward_].y };
	Point nposR = { pos_.x + nDir[myRight[forward_]].x, pos_.y + nDir[myRight[forward_]].y };
	Rect myRectF{ nposF.x, nposF.y, CHA_WIDTH, CHA_HEIGHT };
	Rect myRectR{ nposR.x, nposR.y, CHA_WIDTH, CHA_HEIGHT };
	Stage* stage = (Stage*)FindGameObject<Stage>();
	bool isRightOpen = true;
	bool isForwardOpen = true;
	for (auto& obj : stage->GetStageRects()) {
		if (CheckHit(myRectF, obj)) {
			isForwardOpen = false;
		}
		if (CheckHit(myRectR, obj)) {
			isRightOpen = false;
		}
	}
	if (isRightOpen)
	{
		forward_ = myRight[forward_];
	}
	else if (isRightOpen == false && isForwardOpen == false)
	{
		forward_ = myLeft[forward_];
	}
	

}


bool Enemy::CheckHit(const Rect& me, const Rect& other)
{
	if (me.x < other.x + other.w &&
		me.x + me.w > other.x &&
		me.y < other.y + other.h &&
		me.y + me.h > other.y)
	{
		return true;
	}
	return false;
}


bool Enemy::HitChip(int x, int y)
{
	Rect me = { x,y,CHA_WIDTH,CHA_HEIGHT };

	Stage* stage = (Stage*)FindGameObject<Stage>();
	for (auto& obj : stage->GetStageRects())
	{
		if (CheckHit(me, obj))
		{
			return true;
		}
	}
	return false;
}
