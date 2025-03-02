#include "Enemy.h"
#include "./Stage.h"
#include "globals.h"
#include"Player.h"
#include <queue>
#include<stack>
#include<vector>

std::vector;
std::stack<Point>stack;
std::queue<Point> queue;

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
    isWall_ = false;
}

Enemy::~Enemy()
{
}

void Enemy::Update()
{
    static bool stop = false;
    if (!stop) {
        if (pos_.x < nextPos_.x) {
            pos_.x += 2; // 徐々に移動
            forward_ = RIGHT;
        }
        else if (pos_.x > nextPos_.x) {
            pos_.x -= 2;
            forward_ = LEFT;
        }

        if (pos_.y < nextPos_.y) {
            pos_.y += 2;
            forward_ = DOWN;
        }
        else if (pos_.y > nextPos_.y) {
            pos_.y -= 2;
            forward_ = UP;
        }
    }

    int prgssx = pos_.x % CHA_WIDTH;
    int prgssy = pos_.y % CHA_HEIGHT;
    int cx = (pos_.x / CHA_WIDTH) % 2;
    int cy = (pos_.y / CHA_HEIGHT) % 2;

    if (prgssx == 0 && prgssy == 0 && cx && cy) {
        Bfs();
    }
}

void Enemy::Draw()
{
    DrawBox(pos_.x, pos_.y, pos_.x + CHA_WIDTH, pos_.y + CHA_HEIGHT, GetColor(80, 89, 10), TRUE);

    Point tp[4][3] = {
        {{pos_.x + CHA_WIDTH / 2, pos_.y},              {pos_.x, pos_.y + CHA_HEIGHT / 2}, {pos_.x + CHA_WIDTH, pos_.y + CHA_HEIGHT / 2}},
        {{pos_.x + CHA_WIDTH / 2, pos_.y + CHA_HEIGHT}, {pos_.x, pos_.y + CHA_HEIGHT / 2}, {pos_.x + CHA_WIDTH, pos_.y + CHA_HEIGHT / 2}},
        {{pos_.x, pos_.y + CHA_HEIGHT / 2}, {pos_.x + CHA_WIDTH / 2, pos_.y}, {pos_.x + CHA_WIDTH / 2, pos_.y + CHA_HEIGHT}},
        {{pos_.x + CHA_WIDTH, pos_.y + CHA_HEIGHT / 2}, {pos_.x + CHA_WIDTH / 2, pos_.y}, {pos_.x + CHA_WIDTH / 2, pos_.y + CHA_HEIGHT}}
    };

    DrawTriangle(tp[forward_][0].x, tp[forward_][0].y, tp[forward_][1].x, tp[forward_][1].y, tp[forward_][2].x, tp[forward_][2].y, GetColor(255, 255, 255), TRUE);
}



void Enemy::Bfs()
{
    Player* pPos = (Player*)FindGameObject<Player>();
    Stage* stage = (Stage*)FindGameObject<Stage>();

    Point playerPos = pPos->GetPos();

    const int width = STAGE_WIDTH;
    const int height = STAGE_HEIGHT;

    vector<vector<bool>> visited(width, vector<bool>(height, false));
    vector<vector<Point>> prev(width, vector<Point>(height, { -1, -1 }));

    std::queue<Point> queue;

    // 敵とプレイヤーの位置をマス座標に変換
    int enemyGridX = pos_.x / CHA_WIDTH;
    int enemyGridY = pos_.y / CHA_HEIGHT;
    int playerGridX = playerPos.x / CHA_WIDTH;
    int playerGridY = playerPos.y / CHA_HEIGHT;

    queue.push({ enemyGridX, enemyGridY });
    visited[enemyGridX][enemyGridY] = true;

    bool found = false;

    while (!queue.empty())
    {
        Point current = queue.front();
        queue.pop();

        for (int i = 0; i < 4; i++) {
            Point next = { current.x + nDir[i].x, current.y + nDir[i].y };
            int nx = next.x;
            int ny = next.y;

            if (nx < 0 || ny < 0 || nx >= width || ny >= height) {
                continue;
            }
            if (visited[nx][ny]) {
                continue;
            }

            bool isWall = false;
            Rect nRect = { nx * CHA_WIDTH, ny * CHA_HEIGHT , CHA_WIDTH , CHA_HEIGHT };
            for (auto& obj : stage->GetStageRects())
            {
                if (CheckHit(nRect, obj)) {
                    isWall = true;
                    break;
                }
            }
            if (isWall) {
                continue;
            }

            queue.push(next);
            visited[nx][ny] = true;
            prev[nx][ny] = current;

            if (nx == playerGridX && ny == playerGridY)
            {
                found = true;
                queue = std::queue<Point>(); 
                break;
            }
        }
    }

    Point path = { playerGridX, playerGridY };
    Point nextMove = { -1, -1 };

    while (prev[path.x][path.y].x != -1) {
        if (prev[path.x][path.y].x == enemyGridX &&
            prev[path.x][path.y].y == enemyGridY) {
            nextMove = path;
            break;
        }
        path = prev[path.x][path.y];
    }

    if (nextMove.x == -1) {
        forward_ = (DIR)(GetRand(3));
        return;
    }

    nextPos_.x = nextMove.x * CHA_WIDTH;
    nextPos_.y = nextMove.y * CHA_HEIGHT;
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
