#include "Gui.h"

#include "Program.h"
#include "Factory.h"


using namespace GUI;




Subject::Subject() {}
void Subject::AddObserver(Observer* _observer)
{
    observers.push_back(_observer);
}

Observer::Observer(Subject* _subject,std::vector<float>& _rd, int _level) : subject(_subject), dataHere(_rd), level(_level)
{
    
}

ComponentObserver::ComponentObserver(Subject* _s,std::vector<float>& _v,int _level) : Observer(_s,_v,_level){}

Hud::Hud(Shader* _shader) : Component(this)
{
    level = 0;
    influenceZone = Rect(-1,1,2,-2);
    AddObserver(new HudObserver(this, _shader,level));

    

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

    Panel* panel1 = new Panel(this);
    Panel* panel2 = new Panel(this);
    Panel* panel3 = new Panel(this);
    Panel* panel4 = new Panel(this);
    

    AddComponent(Rect(p1.x,p1.y,(p2-p1).length(),-(p3-p2).length()),panel1,static_cast<HudObserver*>(observers[0])->GetRenderData(),true);
    AddComponent(Rect(p5.x,p5.y,(p5-p1).length(),-(p6-p5).length()),panel2,static_cast<HudObserver*>(observers[0])->GetRenderData(),true);
    AddComponent(Rect(p8.x,p8.y,(p9-p8).length(),-(p10-p8).length()),panel3,static_cast<HudObserver*>(observers[0])->GetRenderData(),true);
    AddComponent(Rect(p12.x,p12.y,(p14-p12).length(),-(p14-p15).length()),panel4,static_cast<HudObserver*>(observers[0])->GetRenderData(),true);

    Rect tempR = panel4->GetInfluenceZone();

   


    Rect tempR2(0.1f,0,tempR.width-0.1f,-tempR.height);

    panel1->AddComponent(Rect(0.2,0.1,0.1,0.1),new Button<Factory,Factory::OperationCreationParam>(Factory::OperationCreationParam(InputAnswer::OperationType::VERT_EXTRUDE,Factory::CreationAddons()),InputManager::GetFactory(),&Factory::CreateOperation,this),static_cast<HudObserver*>(observers[0])->GetRenderData());
    panel4->AddComponent(tempR2,new TimeLine(this),static_cast<HudObserver*>(observers[0])->GetRenderData());
  
    Sel* sel = new Sel(this);
    panel1->AddComponent(Rect(0.05,0.5,0.1,0.4),sel,static_cast<HudObserver*>(observers[0])->GetRenderData());
    sel->AddComponent(Rect(),new SelButton<Factory,Factory::OperationCreationParam>(Factory::OperationCreationParam(InputAnswer::OperationType::CAMERA_MOVE,Factory::CreationAddons(),true),InputManager::GetFactory(),&Factory::CreateOperation,this),static_cast<HudObserver*>(observers[0])->GetRenderData());
    sel->AddComponent(Rect(),new SelButton<Factory,Factory::OperationCreationParam>(Factory::OperationCreationParam(InputAnswer::OperationType::VERT_SCALE,Factory::CreationAddons(),true),InputManager::GetFactory(),&Factory::CreateOperation,this),static_cast<HudObserver*>(observers[0])->GetRenderData());
    sel->AddComponent(Rect(),new SelButton<Factory,Factory::OperationCreationParam>(Factory::OperationCreationParam(InputAnswer::OperationType::VERT_ROTATE,Factory::CreationAddons(vec3(1,0,0)),true),InputManager::GetFactory(),&Factory::CreateOperation,this),static_cast<HudObserver*>(observers[0])->GetRenderData());


    tempR2.width = 0.1f;
    tempR2.startX -=0.1f;

    //panel4->AddComponent(tempR2,new Button<Test,Test::TestData>(Test::TestData(0.1f),&(this->t),&Test::hal,this),static_cast<HudObserver*>(observers[0])->GetRenderData());
}

std::vector<float>& HudObserver::GetRenderData()
{
    return (this->renderData);
}

HudObserver::HudObserver(Hud* hud,Shader* _shader, int _level) : ComponentObserver(hud,(this->renderData),_level)
{
    vao = new VAO();
    vbo = new VBO3f3f();
    vao->AddVBO(*vbo);
    
    shader = _shader;
    std::vector<float> back;
  

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
    vbo->RefreshData(renderData);
    glDrawArrays(GL_TRIANGLES,0,vao->GetCount()); 
    renderData.clear();
    
}


void Component::TestHappened()
{
    for(auto& o : observers ){
        o->Notify();
    }
    for(auto& c : ownedComponents){ 
        c->TestHappened();
    }
}

Component::Component(Component* _root,ComponentTheme* _theme) :root(_root)
{
    _theme == NULL ? theme = new TransparentTheme() : theme = _theme;
}

InputAnswer Component::CheckHit(const vec2& pos)
{
    if(influenceZone.Contains(pos)){
        
        for(auto& ch : ownedComponents){
        
            InputAnswer tempAnsw = ch->CheckHit(pos);
            if(tempAnsw.react != InputAnswer::ReactionType::IGNORED)return tempAnsw;
        }
        
        return HandleHit(pos);
    }else{
        return InputAnswer();
    }
}



void ComponentObserver::FillDataHere(Rect& iz)
{
    
    Component* subjectConv = static_cast<Component*>(subject);
    std::vector<float> temp = subjectConv->GetTheme()->GiveData(iz,level);
    dataHere.insert(dataHere.end(),temp.begin(),temp.end());

    
}


std::vector<float> BasicTheme::GiveData(Rect& r, int level)
{
    std::vector<float> result;
    std::vector<vec2> temp = r.GiveCornersTriangle();
 
    PushBack(result,temp,color,0-(level*0.01)); 
   
    return result; 
}

std::vector<float> TransparentTheme::GiveData(Rect& r, int level)
{
    std::vector<float> result;
    return result;
}


ComponentTheme* Component::GetTheme(){return theme;}




Panel::Panel(Component* _root,vec3 _color, ComponentTheme* _theme) : Component(_root, _theme == NULL ? new BasicTheme(_color) : _theme){}

void Component::AddComponent(Rect _r, Component* _c,std::vector<float>& _rd, bool relToWindow)
{
    _c->SetLevel(level+1);
    _c->AddObserver(new ComponentObserver(_c,_rd,level+1));
    ownedComponents.push_back(_c);
    
    Rect newRect(_r.startX + influenceZone.startX ,-_r.startY+influenceZone.startY ,_r.width,-_r.height);
    
  
    if(relToWindow)_c->SetInfluenceZone(_r);
    else {_c->SetInfluenceZone(newRect);}
}

void Component::SetInfluenceZone(Rect& _r){influenceZone = _r;}

void Component::SetLevel(int l){level = l;}


BasicTheme::BasicTheme(vec3 _color) : color(_color){}



InputAnswer Hud::ProcessMouseClick()
{
    
    return Component::CheckHit(InputManager::ChangeInput(InputManager::GetMousePos2(),false));
}





TimeLine::TimeLine(Component* _root,ComponentTheme* _theme ) : Component(_root,_theme == NULL ? new BasicTheme(vec3(0.1f,0.4f,0.4f)) : _theme){}

InputAnswer TimeLine::HandleHit(const vec2& p)
{
    
    AddComponent(Rect(p.x-influenceZone.width/200,influenceZone.startY,influenceZone.width/100,influenceZone.height),new Panel(root,vec3(1,1,0)),observers[0]->GetDataHere(),true);
    return InputAnswer(InputAnswer::ReactionType::PROCESSED);
}


std::vector<float>& Observer::GetDataHere(){return dataHere;}
