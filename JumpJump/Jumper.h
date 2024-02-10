#pragma once
#include "Actor.h"
class Jumper :
    public Actor
{
private:
    void Jump();
public:
    Jumper();
    void Update();

};

