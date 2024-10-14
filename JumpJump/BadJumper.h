#pragma once
#include "Actor.h"
class BadJumper :
    public Actor
{
private:
    Vector2 vel_ = {};
    void Jump();
    void OnGround();
public:
    BadJumper(int inx=500);
    void Update();

};

