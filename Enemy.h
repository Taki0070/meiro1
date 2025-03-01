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
    DIR forward_; //globals ‚Ìenum‚Ì‚â‚Â Ø‚è‘Ö‚¦‚æ‚¤

    bool isWall_;//‰E‚É•Ç‚ª‚ ‚é

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

