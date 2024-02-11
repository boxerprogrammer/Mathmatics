#pragma once
#include "Actor.h"
//“™‘¬ƒWƒƒƒ“ƒv–ì˜Y
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

