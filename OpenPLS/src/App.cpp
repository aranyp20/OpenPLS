#include "Program.h"

int main()
{
    
    if(Program::GetInstance().Init())Program::GetInstance().Run(); 

    return 0;
}

/* ////////////////////// BUGS /////////////////////////

-ha a kamer�t alulr�l akarod flippeltetni akkor n�ha elt�nik az object
-edgeselectionnal parallel lines-t dob-> siman recttel kell majd
-ha szembe van a huzóka akkor néha forditva huzza

 /////////////////////// TODO /////////////////////////

-foleg a Mesh::Point-nak elore allokalt memory pool


-operationokat torolni - poolba rakni (OperationFactory), nem operation inputbindablleket mehagyni
-fg selectedpointsokat valahogy rendezni kell
-InputManager Getmouse changginputttal kene
-Renderer mennyi csucsot nyomjon ki






 /////////////////////// TODO DONE ////////////////////////
-push_back-re valami megoldast(vbodata adásnál)
 -Mesh konstruktor stringgel h mit csinaljon
 -Mesh-es operationoknak kulon os
-find midpoint 
-selectedPoints.pushBacket Selectpoint()-ra
-Meshnek legyen meshranderee es annak vbo-ja vagy valahogy observeresen de a  mostrani nem jo
-shader setuniformozzon

-OVertMove
-ne dobjak const char-t







/////////////////////// NOPE ////////////////////////
-surface csinalja a meshhandlerert








*/