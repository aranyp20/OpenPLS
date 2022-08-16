#ifndef GUI_H
#define GUI_H



#include "Shader.h"

#include "VAO.h"
#include "VBO.h"

#include "Geometry.h"
#include "InputManager.h"
#include "Factory.h"

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
    
    struct Test{
        struct TestData{
            float b;
            TestData(float a) : b(a){}
        };
        void hal(TestData a){std::cout<<a.b<<std::endl;}
    };
    class Hud :  public Component, public InputProcessor{
        Test t;
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

 


    template <typename OBJ,typename DAT>
    class Button : public Component, public InputBindable{

        typedef void(OBJ::*function_pointer)();
        function_pointer func;
        OBJ* obj;

        DAT param;
        public:

        template<typename ...A>
        Button(DAT _param,OBJ* _obj, void(OBJ::*f)(A...),Component* _root,ComponentTheme* _theme = NULL): param(_param),obj(_obj),func((void(OBJ::*)())f), Component(_root,_theme == NULL ? new BasicTheme(vec3(0.4f,0.4f,0.4f)) : _theme){}
/* 
        template<typename ...A>
        Button<OBJ> operator=(void(OBJ::*f)(A...)){func = (void(OBJ::*)())f;return *this;} */

        template<typename ...A>
        void Call(A... a) const 
        {
            void(OBJ::*f)(A...) = (void(OBJ::*)(A...))(func);
            return ((*obj).*f)(a...);
        }

        InputAnswer HandleHit(const vec2&)
        {
   
            static_cast<BasicTheme*>(theme)->color = vec3(0,1,0);
            this->Call(param);
            InputManager::GetFactory()->CreateOperation(InputAnswer::OperationType::CAMERA_MOVE,Factory::CreationAddons(),true);
            InputManager::ChangeBind(this);
            return InputAnswer(InputAnswer::ReactionType::PROCESSED);
        }
        void Release()
        {
            static_cast<BasicTheme*>(theme)->color = vec3(0.4f,0.4f,0.4f);   
        }
        void Update()
        {

        }
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