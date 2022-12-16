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
    
    std::vector<float>& rData = static_cast<HudObserver*>(observers[0])->GetRenderData();

    AddComponent(Rect(p1.x,p1.y,(p2-p1).length(),-(p3-p2).length()),panel1,rData,true);
    AddComponent(Rect(p5.x,p5.y,(p5-p1).length(),-(p6-p5).length()),panel2,rData,true);
    AddComponent(Rect(p8.x,p8.y,(p9-p8).length(),-(p10-p8).length()),panel3,rData,true);
    AddComponent(Rect(p12.x,p12.y,(p14-p12).length(),-(p14-p15).length()),panel4,rData,true);

    Rect tempR = panel4->GetInfluenceZone();

   



    panel1->AddComponent(Rect(0.2,0.1,0.1,0.1),new Button<Factory,Factory::OperationCreationParam>(Factory::OperationCreationParam(InputAnswer::OperationType::VERT_EXTRUDE,Factory::CreationAddons()),InputManager::GetFactory(),&Factory::CreateOperation,this),rData);
    panel1->AddComponent(Rect(0.2,0.3,0.1,0.1),new Button<Factory,Factory::OperationCreationParam>(Factory::OperationCreationParam(InputAnswer::OperationType::VERT_SUBDIVIDE,Factory::CreationAddons()),InputManager::GetFactory(),&Factory::CreateOperation,this),rData);

    Rect tempR2(0.1f,PressableTheme::CalculateMargin(tempR),tempR.width-0.1f-PressableTheme::CalculateMargin(tempR),-tempR.height - 2*PressableTheme::CalculateMargin(tempR));

    panel4->AddComponent(tempR2,new TimeLine(this),rData);
  
    Sel* sel = new Sel(this);
    panel1->AddComponent(Rect(0.05,0.1,0.1,0.55),sel,rData);
    sel->AddComponent(Rect(),new SelButton<Factory,Factory::OperationCreationParam>(Factory::OperationCreationParam(InputAnswer::OperationType::CAMERA_MOVE,Factory::CreationAddons(),true),InputManager::GetFactory(),&Factory::CreateOperation,this),rData);
    sel->AddComponent(Rect(),new SelButton<Factory,Factory::OperationCreationParam>(Factory::OperationCreationParam(InputAnswer::OperationType::VERT_SCALE,Factory::CreationAddons(),true),InputManager::GetFactory(),&Factory::CreateOperation,this),rData);
    sel->AddComponent(Rect(),new SelButton<Factory,Factory::OperationCreationParam>(Factory::OperationCreationParam(InputAnswer::OperationType::VERT_ROTATE,Factory::CreationAddons(vec3(1,0,0)),true),InputManager::GetFactory(),&Factory::CreateOperation,this),rData);
    sel->AddComponent(Rect(),new SelButton<Factory,Factory::OperationCreationParam>(Factory::OperationCreationParam(InputAnswer::OperationType::BOX_SELECTION,Factory::CreationAddons(),true),InputManager::GetFactory(),&Factory::CreateOperation,this),rData);

    
    
    Sel* mSel = new Sel(this,false);
    SelButton<MeshHandler,Mesh::Mode>* sb1 = new SelButton<MeshHandler,Mesh::Mode>(Mesh::Mode::OBJECT,Program::GetSurface()->meshHandler,&MeshHandler::ChangeMode,this);
    SelButton<MeshHandler,Mesh::Mode>* sb2 = new SelButton<MeshHandler,Mesh::Mode>(Mesh::Mode::EDIT,Program::GetSurface()->meshHandler,&MeshHandler::ChangeMode,this);
    SelButton<MeshHandler,Mesh::Mode>* sb3 = new SelButton<MeshHandler,Mesh::Mode>(Mesh::Mode::VERTEX,Program::GetSurface()->meshHandler,&MeshHandler::ChangeMode,this);

    panel2->AddComponent(Rect(0.45,PressableTheme::CalculateMargin(panel2->GetInfluenceZone()),0.4,-panel2->GetInfluenceZone().height-2*PressableTheme::CalculateMargin(panel2->GetInfluenceZone())),mSel,rData);
    


    mSel->AddComponent(Rect(),sb1,rData);
    mSel->AddComponent(Rect(),sb2,rData);
    mSel->AddComponent(Rect(),sb3,rData);


    tempR2.width = 0.1f;
    tempR2.startX -=0.1f;

    panel2->AddComponent(Rect(0.04,0.04,0.4,0.04),new Slide<Factory,InputAnswer::OperationType>(InputAnswer::OperationType::CAMERA_FOCUS_SET,InputManager::GetFactory(),&Factory::CreateOperation2,this),rData);
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


std::vector<float> BasicTheme::GiveData(const Rect& r, int level)
{
    std::vector<float> result;
    
    std::vector<vec2> temp = r.GiveCornersTriangle();
 
    PushBack(result,temp,color,0-(level*0.1)); 
   


    return result; 
}

std::vector<float> TransparentTheme::GiveData(const Rect& r, int level)
{
    std::vector<float> result;
    return result;
}


ComponentTheme* Component::GetTheme(){return theme;}




Panel::Panel(Component* _root,vec3 _color, ComponentTheme* _theme) : Component(_root, _theme == NULL ? new PressableTheme() : _theme){}

void Component::AddComponent(Rect _r, Component* _c,std::vector<float>& _rd, bool relToWindow)
{
    _c->SetLevel(level+1);
    _c->AddObserver(new ComponentObserver(_c,_rd,level+1));
    ownedComponents.push_back(_c);
    
    Rect newRect(_r.startX + influenceZone.startX ,-_r.startY+influenceZone.startY ,_r.width,-_r.height);
    
  
    if(relToWindow){_c->SetInfluenceZone(_r);}
    else {_c->SetInfluenceZone(newRect);}

    _c->YouAreAdded();
}

void Component::SetInfluenceZone(const Rect& _r){influenceZone = _r;}

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


std::vector<float> PressableTheme::GiveData(const Rect& r, int level)
{
    float il = 0-level*0.1;

    std::vector<float> result;
   
   

    std::vector<vec3> collector1 = {vec3(r.startX,r.startY,il),vec3(r.startX+r.width,r.startY,il),vec3(r.startX+r.width,r.startY+r.height,il),vec3(r.startX,r.startY+r.height,il)};

    vec3 topCol =  vec3(0.2,0.2,0.2);
    vec3 botCol = vec3(0.1,0.1,0.1);

    if(pressed){
    
        topCol = vec3(0.4,0,0);
        botCol = vec3(0.15,0,0);
    }

    PushBack(result,std::vector<vec3>{collector1[0],topCol,collector1[1],topCol,collector1[2],botCol,collector1[0],topCol,collector1[2],botCol,collector1[3],botCol});

    il -= 0.01;

    float margin = PressableTheme::CalculateMargin(r);
    
   


    std::vector<vec3> ocp{vec3(r.startX,r.startY,il),vec3(r.startX+r.width,r.startY,il),vec3(r.startX+r.width,r.startY+r.height,il),vec3(r.startX,r.startY+r.height,il)};
    std::vector<vec3> omp{vec3(r.startX + r.width/2,r.startY,il),vec3(r.startX+r.width,r.startY+r.height/2,il),vec3(r.startX+r.width/2,r.startY+r.height,il),vec3(r.startX,r.startY+ r.height/2,il)};
    std::vector<vec3> icp{vec3(r.startX+margin,r.startY-margin,il),vec3(r.startX+r.width-margin,r.startY-margin,il),vec3(r.startX+r.width-margin,r.startY+r.height+margin,il),vec3(r.startX+margin,r.startY+r.height+margin,il)};
    std::vector<vec3> imp{vec3(r.startX+r.width/2,r.startY-margin,il),vec3(r.startX+r.width-margin,r.startY+r.height/2,il),vec3(r.startX+r.width/2,r.startY+r.height+margin,il),vec3(r.startX+margin,r.startY+r.height/2,il)};
    

    vec3 ocpC(0.1,0.1,0.1);
    vec3 ompC(0.12,0.12,0.12);
    vec3 icpC(0.15,0.15,0.15);
    vec3 impC(0.5,0.5,0.5);

    if(pressed)impC = vec3(0.3,0,0);


    std::vector<vec3>collector{ocp[0],ocpC,icp[0],icpC,imp[0],impC,ocp[0],ocpC,omp[0],ompC,imp[0],impC,
        ocp[1],ocpC,icp[1],icpC,imp[0],impC,ocp[1],ocpC,omp[0],ompC,imp[0],impC,
        ocp[1],ocpC,icp[1],icpC,imp[1],impC,ocp[1],ocpC,omp[1],ompC,imp[1],impC,
        ocp[2],ocpC,icp[2],icpC,imp[1],impC,ocp[2],ocpC,omp[1],ompC,imp[1],impC,
        ocp[2],ocpC,icp[2],icpC,imp[2],impC,ocp[2],ocpC,omp[2],ompC,imp[2],impC,
        ocp[3],ocpC,icp[3],icpC,imp[2],impC,ocp[3],ocpC,omp[2],ompC,imp[2],impC,
        ocp[3],ocpC,icp[3],icpC,imp[3],impC,ocp[3],ocpC,omp[3],ompC,imp[3],impC,
        ocp[0],ocpC,icp[0],icpC,imp[3],impC,ocp[0],ocpC,omp[3],ompC,imp[3],impC
    };

    PushBack(result,collector);

    

    
    




    return result;
}

float PressableTheme::CalculateMargin(const Rect& r)
{
    return std::min(0.015f,std::min(fabs(r.width),fabs(r.height)) / 10);
}