#include "Program.h"

int main()
{
    
    if(Program::GetInstance().Init())Program::GetInstance().Run(); 

    return 0;
}

/* ////////////////////// BUGS /////////////////////////

-ha a kamer�t alulr�l akarod flippeltetni akkor n�ha elt�nik az object


 /////////////////////// TODO /////////////////////////

-ne dobjak const char-t


-Meshnek legyen meshranderee es annak vbo-ja vagy valahogy observeresen de a  mostrani nem jo
-shader setuniformozzon

-push_back-re valami megoldast(vbodata adásnál)
-selectedPoints.pushBacket Selectpoint()-ra
-operationokat torolni - poolba rakni (OperationFactory), nem operation inputbindablleket mehagyni
-fg selectedpointsokat valahogy rendezni kell
-InputManager Getmouse changginputttal kene
-Renderer mennyi csucsot nyomjon ki


-OVertMove

 /////////////////////// TODO DONE ////////////////////////
 -Mesh konstruktor stringgel h mit csinaljon
 -Mesh-es operationoknak kulon os
-find midpoint 








/////////////////////// NOPE ////////////////////////
-surface csinalja a meshhandlerert








*/