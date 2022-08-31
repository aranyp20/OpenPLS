#include "Program.h"

int main()
{
    
    if(Program::GetInstance().Init())Program::GetInstance().Run(); 

    return 0;
}