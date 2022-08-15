#ifndef FACTORY_H
#define FACTORY_H

#include "Surface.h"



class InputAnswer;

class Factory{

    Surface* surface;

    public:

    struct CreationAddons{
        vec3 pvec3;
        CreationAddons();
        CreationAddons(vec3);
    };

    Factory(Surface*);

    Operation* CreateOperation(InputAnswer::OperationType,CreationAddons addons = CreationAddons());


};
















#endif