#pragma once
#include "Actor.h"
//等速ジャンプ野郎
class PoorJumper :
    public Actor
{
private:
    Vector2 vel_ = {};
    void Jump();
    void OnGround();
public:
    PoorJumper(int inx=500);
    void Update();

};

