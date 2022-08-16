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

    struct OperationHolder{
        bool active;
        InputAnswer::OperationType type;
        CreationAddons addons;

        OperationHolder();
        OperationHolder(InputAnswer::OperationType,CreationAddons);
    };



    Operation* TriggerHolded();
    void ReleaseHolded();

    Factory(Surface*);

    Operation* CreateOperation(InputAnswer::OperationType,CreationAddons addons = CreationAddons(),bool delayed = false);

    private:
    OperationHolder oh;

    void SetHolded(OperationHolder);
};
















#endif