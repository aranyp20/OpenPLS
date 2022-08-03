#include "Program.h"

int main()
{
    
    if(Program::GetInstance().Init())Program::GetInstance().Run(); 

    return 0;
}

/*  BUGS:

-ha a kamerát alulról akarod flippeltetni akkor néha eltûnik az object






*/