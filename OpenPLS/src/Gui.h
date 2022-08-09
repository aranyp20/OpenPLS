#ifndef GUI_H
#define GUI_H



#include "Shader.h"

#include "VAO.h"
#include "VBO.h"


namespace GUI{




    class HudObserver;
    class Observer;


    class Subject{
        protected:
        Observer* observer;
        public:
        Subject(Observer*);


    };

    class Observer{
        protected:
        VAO* vao;
        VBO* vbo;
        std::vector<Subject*> subjects;


        public:
        Observer();
        void AddSubject(Subject*);
        virtual void Notify() = 0;

    };


    class Hud : public Subject{
        
        public:
        Hud(Shader*);
        void TestHappened();

        friend class HudObserver;
    };

    class HudObserver : public Observer{
        Shader* shader;
        public:
        HudObserver(Hud*,Shader*);
        void Notify();

        void Render();
    };


};




















#endif