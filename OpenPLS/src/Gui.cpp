#include "Gui.h"

#include "Program.h"



using namespace GUI;




Subject::Subject() {}
void Subject::AddObserver(Observer* _observer)
{
    observers.push_back(_observer);
}

Observer::Observer(Subject* _subject,std::vector<float>& _rd) : subject(_subject), dataHere(_rd)
{
    
}

ComponentObserver::ComponentObserver(Subject* _s,std::vector<float>& _v) : Observer(_s,_v){}

Hud::Hud(Shader* _shader) 
{
    AddObserver(new HudObserver(this, _shader));
}

HudObserver::HudObserver(Hud* hud,Shader* _shader) : ComponentObserver(hud,(this->dataHere))
{
    vao = new VAO();
    vbo = new VBO3f3f();
    vao->AddVBO(*vbo);
    
    shader = _shader;
    std::vector<float> back;
    //ezek lesznek a panelok koordinatai
    vec2 p1 = InputManager::ChangeInput(vec2((float)(Program::SurfaceStartingX()+Program::SurfaceWidth()),0.0f),false);
    vec2 p2 =  InputManager::ChangeInput(vec2((float)(Program::WindowWidthR()),0.0f),false);
    vec2 p3 =  InputManager::ChangeInput(vec2((float)(Program::WindowWidthR()),(float)Program::WindowHeightR()),false);
    vec2 p4 =  InputManager::ChangeInput(vec2((float)(Program::SurfaceStartingX()+Program::SurfaceWidth()),(float)Program::WindowHeightR()),false);
    vec2 p5 = InputManager::ChangeInput(vec2(0,0),false);
    vec2 p6 = InputManager::ChangeInput(vec2(0,(float)Program::SurfaceStartingY()),false);
    vec2 p7 = InputManager::ChangeInput(vec2((float)(Program::SurfaceStartingX()+Program::SurfaceWidth()),(float)Program::SurfaceStartingY()),false);
    vec2 p8 = InputManager::ChangeInput(vec2(0,Program::SurfaceStartingY()),false);
    vec2 p9 = InputManager::ChangeInput(vec2(Program::SurfaceStartingX(),Program::SurfaceStartingY()),false);
    vec2 p10 = InputManager::ChangeInput(vec2(0,Program::SurfaceStartingY()+Program::SurfaceHeight()),false);
    vec2 p11 = InputManager::ChangeInput(vec2(Program::SurfaceStartingX(),Program::SurfaceStartingY()+Program::SurfaceHeight()),false);
    vec2 p12 = InputManager::ChangeInput(vec2(0,Program::SurfaceStartingY()+Program::SurfaceHeight()),false);
    vec2 p13 = InputManager::ChangeInput(vec2(0,Program::WindowHeightR()),false);
    vec2 p14 = InputManager::ChangeInput(vec2(Program::SurfaceStartingX()+Program::SurfaceWidth(),Program::SurfaceStartingY()+Program::SurfaceHeight()),false);
    vec2 p15 = InputManager::ChangeInput(vec2(Program::SurfaceStartingX()+Program::SurfaceWidth(),Program::WindowHeightR()),false);


    PushBack(back,vec3(p1.x,p1.y,0));
    PushBack(back,vec3(0,0,0));
    PushBack(back,vec3(p2.x,p2.y,0));
    PushBack(back,vec3(0,0,0));
    PushBack(back,vec3(p3.x,p3.y,0));
    PushBack(back,vec3(0,0,0));
    PushBack(back,vec3(p1.x,p1.y,0));
    PushBack(back,vec3(0,0,0));
    PushBack(back,vec3(p3.x,p3.y,0));
    PushBack(back,vec3(0,0,0));
    PushBack(back,vec3(p4.x,p4.y,0));
    PushBack(back,vec3(0,0,0));
    PushBack(back,vec3(p1.x,p1.y,0));
    PushBack(back,vec3(0,0,0));
    PushBack(back,vec3(p5.x,p5.y,0));
    PushBack(back,vec3(0,0,0));
    PushBack(back,vec3(p6.x,p6.y,0));
    PushBack(back,vec3(0,0,0));
    PushBack(back,vec3(p1.x,p1.y,0));
    PushBack(back,vec3(0,0,0));
    PushBack(back,vec3(p6.x,p6.y,0));
    PushBack(back,vec3(0,0,0));
    PushBack(back,vec3(p7.x,p7.y,0));
    PushBack(back,vec3(0,0,0));
    PushBack(back,vec3(p8.x,p8.y,0));
    PushBack(back,vec3(0,0,0));
    PushBack(back,vec3(p9.x,p9.y,0));
    PushBack(back,vec3(0,0,0));
    PushBack(back,vec3(p10.x,p10.y,0));
    PushBack(back,vec3(0,0,0));
    PushBack(back,vec3(p9.x,p9.y,0));
    PushBack(back,vec3(0,0,0));
    PushBack(back,vec3(p10.x,p10.y,0));
    PushBack(back,vec3(0,0,0));
    PushBack(back,vec3(p11.x,p11.y,0));
    PushBack(back,vec3(0,0,0));
    PushBack(back,vec3(p12.x,p12.y,0));
    PushBack(back,vec3(0,0,0));
    PushBack(back,vec3(p13.x,p13.y,0));
    PushBack(back,vec3(0,0,0));
    PushBack(back,vec3(p14.x,p14.y,0));
    PushBack(back,vec3(0,0,0));
    PushBack(back,vec3(p13.x,p13.y,0));
    PushBack(back,vec3(0,0,0));
    PushBack(back,vec3(p14.x,p14.y,0));
    PushBack(back,vec3(0,0,0));
    PushBack(back,vec3(p15.x,p15.y,0));
    PushBack(back,vec3(0,0,0));
    vbo->RefreshData(back); 

}

Rect& Component::GetInfluenceZone(){return influenceZone;}

void ComponentObserver::Notify()
{
    FillDataHere(static_cast<Component*>(subject)->GetInfluenceZone());
}


void HudObserver::Notify()
{
    ComponentObserver::Notify();
    Render();
}

void HudObserver::Render()
{
    glViewport(0,0,Program::WindowWidthR(),Program::WindowHeightR());
    shader->Bind();
    vao->Bind();
    glDrawArrays(GL_TRIANGLES,0,vao->GetCount());
}


void Hud::TestHappened()
{
    for(auto& o : observers ){
        o->Notify();
    }
}

Component::Component(Rect& _iz) : influenceZone(_iz)
{

}

bool Component::CheckHit(vec2& pos)
{
    if(influenceZone.Contains(pos)){
        for(auto& ch : ownedComponents){
            if(ch->CheckHit(pos))return true;
        }
        HandleHit(pos);
        return true;
    }else{
        return false;
    }
}

void Component::Draw()
{
    for(auto& o : observers){
        o->Notify();
    }

    for(auto& c : ownedComponents){
        c->Draw();
    }
}

void ComponentObserver::FillDataHere(Rect& iz)
{
    Component* subjectConv = static_cast<Component*>(subject);
    std::vector<float> temp = subjectConv->GetTheme()->GiveData(iz);
    dataHere.insert(dataHere.end(),temp.begin(),temp.end());
}


std::vector<float> BasicTheme::GiveData(Rect& r)
{
    std::vector<float> result;
    std::vector<vec2> temp = r.GiveCornersTriangle();
    PushBack(result,temp,vec3(0,1,0),1.0f);
    return result;
}

ComponentTheme* Component::GetTheme(){return theme;}