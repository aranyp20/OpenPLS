#include "Program.h"

int main()
{
    
    if(Program::GetInstance().Init())Program::GetInstance().Run(); 

    return 0;
}

/* ////////////////////// BUGS /////////////////////////

-ha a kamer�t alulr�l akarod flippeltetni akkor n�ha elt�nik az object


 /////////////////////// TODO /////////////////////////

-Meshnek legyen meshranderee es annak vbo-ja vagy valahogy observeresen de a  mostrani nem jo
 -Mesh-es operationoknak kulon os
-push_back-re valami megoldast(vbodata adásnál)
-surface csinalja a meshhandlerert
-selectedPoints.pushBacket Selectpoint()-ra
-operationokat torolni, nem operation inputbindablleket mehagyni
-find midpoint 
-fg selectedpointsokat valahogy rendezni kell
-InputManager Getmouse changginputttal kene
-Renderer mennyi csucsot nyomjon ki


-OVertMove

 /////////////////////// TODO DONE ////////////////////////
 -Mesh konstruktor stringgel h mit csinaljon


*/