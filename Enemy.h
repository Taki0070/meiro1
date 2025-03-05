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
    DIR forward_; //globals ÇÃenumÇÃÇ‚Ç¬ êÿÇËë÷Ç¶ÇÊÇ§

    bool isWall_;//âEÇ…ï«Ç™Ç†ÇÈ

public:
    Enemy();
    ~Enemy();


    void Update() override;
    void Draw() override;
  /*  void XCloserMove();
    void YCloserMove();
    void XYCloserMove();*/

    void MOVE(Point p);
    void RightPointMove();
    bool CheckHit(const Rect& me, const Rect& other);
    bool HitChip(int x, int y);
};

