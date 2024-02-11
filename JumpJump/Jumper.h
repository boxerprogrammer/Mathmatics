#pragma once
#include "Actor.h"
class Jumper :
    public Actor
{
private:
    Vector2 vel_ = {};
    void Jump();
    void OnGround();
public:
    Jumper(int inx=500);
    void Update();

};

