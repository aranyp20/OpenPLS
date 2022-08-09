#include "Gui.h"

#include "Program.h"



using namespace GUI;

Subject::Subject(Observer* _observer) : observer(_observer){}

Observer::Observer()
{
    vao = new VAO();
    vbo = new VBO3f3f();
    vao->AddVBO(*vbo);
}

Hud::Hud(Shader* _shader) : Subject(new HudObserver(this,_shader)) {}

HudObserver::HudObserver(Hud* hud,Shader* _shader)
{
    AddSubject(hud);
    shader = _shader;
    std::vector<float> back;
    vec2 p1 = InputManager::ChangeInput(vec2((float)(Program::SurfaceStartingX()+Program::SurfaceWidth()),0.0f),false);
    vec2 p2 =  InputManager::ChangeInput(vec2((float)(Program::WindowWidthR()),0.0f),false);
    vec2 p3 =  InputManager::ChangeInput(vec2((float)(Program::WindowWidthR()),(float)Program::WindowHeightR()),false);
    vec2 p4 =  InputManager::ChangeInput(vec2((float)(Program::SurfaceStartingX()+Program::SurfaceWidth()),(float)Program::WindowHeightR()),false);
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
    vbo->RefreshData(back); 

}

void Observer::AddSubject(Subject* sub)
{
    subjects.push_back(sub);
}



void HudObserver::Notify()
{
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
    observer->Notify();
}