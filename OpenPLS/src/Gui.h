#ifndef GUI_H
#define GUI_H



#include "Shader.h"

#include "VAO.h"
#include "VBO.h"

#include "Geometry.h"


namespace GUI{




    class HudObserver;
    class Observer;


    struct ComponentTheme{
        virtual std::vector<float> GiveData(Rect&) = 0;
    };

    struct BasicTheme : public ComponentTheme{
        std::vector<float> GiveData(Rect&);
    };

    struct TransparentTheme : public ComponentTheme{
        std::vector<float> GiveData(Rect&);
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
        std::vector<float>& dataHere;
        Subject* subject;


        public:
        Observer(Subject*,std::vector<float>&);
        virtual void Notify() = 0;

    };

    class ComponentObserver : public Observer{
        protected:
        virtual void FillDataHere(Rect&);
        public:
        ComponentObserver(Subject*,std::vector<float>&);
        virtual void Notify();
    };

    class Component : public Subject{
        protected:
        std::vector<Component*> ownedComponents;
        Rect influenceZone;
        ComponentTheme* theme;
        public:
        Component(ComponentTheme* _theme = NULL);
        bool CheckHit(vec2&);
    
        virtual void HandleHit(vec2&){}
        ComponentTheme* GetTheme();
        Rect& GetInfluenceZone();
        //Real-time maradjon!!
        void AddComponent(Rect,Component*,std::vector<float>&);   
        void SetInfluenceZone(Rect&);
         void TestHappened();
    };
    
    class Hud :  public Component{
        
        public:
        Hud(Shader*);
       

        friend class HudObserver;
    };

    class HudObserver : public ComponentObserver{
        std::vector<float> renderData;
        Shader* shader;
        VAO* vao;
        VBO* vbo;
        public:
        HudObserver(Hud*,Shader*);
        void Notify();

        void Render();
        std::vector<float>& GetRenderData();
    };


 

    class Button : public Component{
        public:
        Button(ComponentTheme* _theme = NULL);
    };

    class Panel : public Component{
        public:
        Panel(ComponentTheme* _theme = NULL);
    };




};




















#endif