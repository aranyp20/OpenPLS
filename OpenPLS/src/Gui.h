#ifndef GUI_H
#define GUI_H



#include "Shader.h"

#include "VAO.h"
#include "VBO.h"

#include "Geometry.h"
#include "InputManager.h"


namespace GUI{




    class HudObserver;
    class Observer;


    struct ComponentTheme{
        virtual std::vector<float> GiveData(Rect&, int) = 0;
    };

    struct BasicTheme : public ComponentTheme{
        vec3 color;
        std::vector<float> GiveData(Rect&, int);

        BasicTheme(vec3);
    };

    struct TransparentTheme : public ComponentTheme{
        std::vector<float> GiveData(Rect&, int);
    };


    class Subject{
        protected:
        std::vector<Observer*> observers;
        public:
        Subject();
        void AddObserver(Observer*);


    };

    class Observer{
        protected:
        int level;
        std::vector<float>& dataHere;
        Subject* subject;


        public:
        Observer(Subject*,std::vector<float>&,int);
        virtual void Notify() = 0;
        std::vector<float>& GetDataHere();


    };

    class ComponentObserver : public Observer{
        protected:
        virtual void FillDataHere(Rect&);
        public:
        ComponentObserver(Subject*,std::vector<float>&,int);
        virtual void Notify();
    };

    class Component : public Subject{
        protected:
        Component* root;
        int level;
        std::vector<Component*> ownedComponents;
        Rect influenceZone;
        ComponentTheme* theme;
        public:
        Component(Component*,ComponentTheme* _theme = NULL);
        InputAnswer CheckHit(const vec2&);
        virtual InputAnswer HandleHit(const vec2&){ return InputAnswer();}
        ComponentTheme* GetTheme();
        Rect& GetInfluenceZone();
        //Real-time maradjon!!
        void AddComponent(Rect,Component*,std::vector<float>&, bool relToWindow = false);   
        void SetInfluenceZone(Rect&);
        void TestHappened();
        void SetLevel(int);
    };
    
    class Hud :  public Component, public InputProcessor{
        
        public:
        Hud(Shader*);

        InputAnswer ProcessMouseClick();

        friend class HudObserver;
    };

    class HudObserver : public ComponentObserver{
        std::vector<float> renderData;
        Shader* shader;
        VAO* vao;
        VBO* vbo;
        public:
        HudObserver(Hud*,Shader*,int);
        void Notify();

        void Render();
        std::vector<float>& GetRenderData();
    };

 

    class Button : public Component, public InputBindable{
        public:
        Button(Component*,ComponentTheme* _theme = NULL);
        InputAnswer HandleHit(const vec2&);
        void Release();
        void Update();
    };

    class Panel : public Component{
        public:
        Panel(Component*,vec3 _color = vec3(0.2f,0.2f,0.2f),ComponentTheme* _theme = NULL);
    };


    class TimeLine : public Component{
        public:
        TimeLine(Component*,ComponentTheme* _theme = NULL);
        InputAnswer HandleHit(const vec2&);
    };



};




















#endif