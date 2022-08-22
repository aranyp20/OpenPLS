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
        bool pressed = false;
        Component* root;
        int level;
        std::vector<Component*> ownedComponents;
        Rect influenceZone;
        ComponentTheme* theme;
        public:
        Component(Component*,ComponentTheme* _theme = NULL);
        
        virtual InputAnswer CheckHit(const vec2&);
        virtual InputAnswer HandleHit(const vec2&){ return InputAnswer();}
        ComponentTheme* GetTheme();
        Rect& GetInfluenceZone();
        //Real-time maradjon!!
        virtual void AddComponent(Rect,Component*,std::vector<float>&, bool relToWindow = false);   
        void SetInfluenceZone(Rect&);
        void TestHappened();
        void SetLevel(int);
        virtual void Activate(){}
        virtual void InActivate(){}
        bool IsActive(){return pressed;}
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

 


    template <typename OBJ,typename  DAT>
    class Button : public Component, public InputBindable{
        protected:
        
        typedef void(OBJ::*function_pointer)();
        function_pointer func;
        OBJ* obj;

        DAT param;
        public:

        template<typename ...A>
        Button(DAT _param,OBJ* _obj, void(OBJ::*f)(A...),Component* _root,ComponentTheme* _theme = NULL): param(_param),obj(_obj),func((void(OBJ::*)())f), Component(_root,_theme == NULL ? new BasicTheme(vec3(0.4f,0.4f,0.4f)) : _theme){}
       

        template<typename ...A>
        void Call(A... a) const 
        {
            void(OBJ::*f)(A...) = (void(OBJ::*)(A...))(func);
            ((*obj).*f)(a...);
        }

        virtual InputAnswer HandleHit(const vec2&)
        {
           
            static_cast<BasicTheme*>(theme)->color = vec3(0,1,0);
            this->Call(param);
            InputManager::ChangeBind(this);
            return InputAnswer(InputAnswer::ReactionType::PROCESSED);
        }
        virtual void Release()
        {
            static_cast<BasicTheme*>(theme)->color = vec3(0.4f,0.4f,0.4f);   
        }
        void Update()
        {

        }
        
  
    };

    

    template<typename OBJ,typename DAT>
    class SelButton : public Button<OBJ,DAT>{

        
        public:
        template<typename ...A>
        SelButton(DAT _param,OBJ* _obj, void(OBJ::*f)(A...),Component* _root,ComponentTheme* _theme = NULL) : Button<OBJ,DAT>(_param,_obj,f,_root,_theme){}


        InputAnswer HandleHit(const vec2&)
        {
            
            static_cast<BasicTheme*>(Component::theme)->color = vec3(0,1,0);
            this->Call(Button<OBJ,DAT>::param);
            return InputAnswer(InputAnswer::ReactionType::PROCESSED); 

        }

        InputAnswer CheckHit(const vec2&){return InputAnswer();}

        

        void Release()
        {
            
        }

        void Activate()
        {
            static_cast<BasicTheme*>(Component::theme)->color = vec3(0.0f,0.8f,0.4f);
            Component::pressed = true;
        }

        void InActivate()
        {
            
            static_cast<BasicTheme*>(Component::theme)->color = vec3(0.4f,0.4f,0.4f);
            Component::pressed = false;
        }

    };


    class Sel : public Component, public InputBindable{
       
        bool vertical;
  
        public:
        
        Sel(Component* _root, bool _vertical = true) : Component(_root,new BasicTheme(vec3(0.8f,0.8f,0.8f))),vertical(_vertical)
        {
        }

        void AddComponent(Rect _r,Component* _c,std::vector<float>& _rd, bool relToWindow = false)
        {
            Component::AddComponent(_r,_c,_rd);
            float margin = std::min(fabs(influenceZone.width),fabs(influenceZone.height));
            margin /= 10;
            int bS = ownedComponents.size();
            if(!vertical){
                float widthHere = influenceZone.width/bS + ((bS+1)*margin)/bS;
                for(int i=0;i<bS;i++){
                    Rect newRect(influenceZone.startX - i*(-widthHere+margin)-margin,influenceZone.startY + margin,widthHere,influenceZone.height-2*margin);
                    ownedComponents[i]->SetInfluenceZone(newRect);
                }
                return;
            }
            float heightHere = influenceZone.height/bS + ((bS+1)*margin)/bS;
            
            for(int i=0;i<bS;i++){
                Rect newRect(influenceZone.startX + margin,influenceZone.startY - i*(-heightHere+margin)-margin,influenceZone.width -2*margin,heightHere );
                ownedComponents[i]->SetInfluenceZone(newRect);
            }
        }


        void Update()
        {

        }

        InputAnswer HandleHit(const vec2& p){
            for(auto& b : ownedComponents){
                InputAnswer itsAnswer = b->Component::CheckHit(p).react;
                if(itsAnswer.react != InputAnswer::ReactionType::IGNORED){
                    if(b->IsActive()){
                        InputManager::GetFactory()->ReleaseHolded();
                        b->InActivate();
                    }else{
                        for(auto& _c : ownedComponents){
                            _c->InActivate();
                        }
                        b->Activate();
                    }
                    //InputManager::ChangeBind((InputBindable*)(b));
                    
                    return itsAnswer;
                }
            }
            return InputAnswer();
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