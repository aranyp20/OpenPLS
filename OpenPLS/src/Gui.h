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

    //ez még örököljön componenttől és inputprocessurból
    class Hud : public Subject{
        
        public:
        Hud(Shader*);
        void TestHappened();

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
    };


    class Component : public Subject{
        protected:
        std::vector<Component*> ownedComponents;
        Rect influenceZone;
        ComponentTheme* theme;
        public:
        Component(Rect&);
        bool CheckHit(vec2&);
        void Draw();
        virtual void HandleHit(vec2&){}
        ComponentTheme* GetTheme();
        Rect& GetInfluenceZone();
        
    };

    class Button : public Component{

    };




};




















#endif