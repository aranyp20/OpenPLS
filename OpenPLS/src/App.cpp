#include "Program.h"

int main()
{
    
    if(Program::GetInstance().Init())Program::GetInstance().Run(); 

    return 0;
}

/*  BUGS:

-ha a kamer�t alulr�l akarod flippeltetni akkor n�ha elt�nik az object

-InputManager Getmouse changginputttal kene
-push_back-re valami megoldast(vbodata adásnál)

-selectedPoints.pushBacket Selectpoint()-ra

*/