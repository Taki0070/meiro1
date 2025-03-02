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

    void Bfs();
    void Dfs();
    void Update() override;
    void Draw() override;
    void XCloserMove();
    void YCloserMove();
    void XYCloserMove();
    void RightPointMove();
    bool CheckHit(const Rect& me, const Rect& other);
};

