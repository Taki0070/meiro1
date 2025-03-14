#pragma once
#include "./Engine/GameObject.h"
#include "./globals.h"


class Enemy :
    public GameObject
{
    Point pos_;
    bool isAlive_;
    float speed_;
    Point nextPos_;
    DIR forward_; //globals のenumのやつ 切り替えよう

    bool isWall_;//右に壁がある

public:
    Enemy();
    ~Enemy();


    void Update() override;
    void Draw() override;
    void XCloserMove();
    void YCloserMove();
    void XYCloserMove();
    void RightPointMove();
    bool CheckHit(const Rect& me, const Rect& other);
};

