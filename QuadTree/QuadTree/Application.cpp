#include "Application.h"
#include<DxLib.h>
#include"NullDivider.h"
#include"Actor.h"
#include<algorithm>

Application::Application() {


}

void 
Application::TraverseActors(std::shared_ptr<IDivider> divider, int limit)
{
    const auto& size = divider->GetSize();
    auto left = divider->GetLeft();
    auto right = divider->GetLeft()+size.w;
    auto top = divider->GetTop();
    auto bottom = divider->GetTop() + size.h;
    auto count=std::count_if(actors_.begin(), actors_.end(), [left, top, right, bottom](std::shared_ptr<Actor> actor) {
        const auto& pos = actor->GetPos();
        return (left < pos.x && pos.x < right && top < pos.y && pos.y < bottom);
    });
    if (count >= 3 && limit>0) {
        TraverseActors(divider->AddQuadDivider(Position2((left+right)*0.5f, (top+bottom)*0.5f)), limit - 1);
        TraverseActors(divider->AddQuadDivider(Position2(right+size.w*0.5f, (top + bottom) * 0.5f)), limit - 1);
        TraverseActors(divider->AddQuadDivider(Position2((left + right ) * 0.5f, bottom+size.h*0.5f)), limit - 1);
        TraverseActors(divider->AddQuadDivider(Position2(right+size.w * 0.5f, bottom+size.h * 0.5f)), limit - 1);
    }
}

Application& Application::GetInstance()
{
    static Application app;
    return app;
}

bool Application::Init()
{
    ChangeWindowMode(true);
    if (DxLib_Init() == -1) {
        return false;
    }
    SetDrawScreen(DX_SCREEN_BACK);
    rootDivider_ = std::make_shared<NullDivider>();
    //rootDivider_->AddQuadDivider(Position2(0.0f, 0.0f))->AddQuadDivider(Position2(0.0f, 0.0f))->AddQuadDivider(Position2(0.0f, 0.0f));
    actors_.resize(100);
    for (auto& act : actors_) {
        act = std::make_shared<Actor>(Position2(GetRand(640), GetRand(480)));
    }
    TraverseActors(rootDivider_,10);
    return true;
}

void Application::Run()
{
    while (ProcessMessage() != -1) {
        ClearDrawScreen();
        rootDivider_->Update();
        rootDivider_->Draw();
        for (auto& act : actors_) {
            act->Draw();
        }
        ScreenFlip();
    }
}

void Application::Terminate()
{
    DxLib_End();
}
