#ifndef FACTORY_H
#define FACTORY_H

#include "Surface.h"



class InputAnswer;

class Factory{

    Surface* surface;

    std::vector<Operation*> activeOperations;

    public:

    struct CreationAddons{
        vec3 pvec3;
        float pfloat;
        CreationAddons();
        CreationAddons(vec3);
        CreationAddons(float);
    };

    struct OperationHolder{
        bool active;
        InputAnswer::OperationType type;
        CreationAddons addons;

        OperationHolder();
        OperationHolder(InputAnswer::OperationType,CreationAddons);
    };



    

    struct OperationCreationParam{
        InputAnswer::OperationType type;
        CreationAddons addons;
        bool delayed;
        OperationCreationParam(InputAnswer::OperationType,CreationAddons,bool delayed = false);
    };



    Operation* TriggerHolded();
    void ReleaseHolded();

    Factory(Surface*);

    Operation* CreateOperation(InputAnswer::OperationType,CreationAddons addons = CreationAddons(),bool delayed = false);
    void CreateOperation(OperationCreationParam);
    void CreateOperation2(InputAnswer::OperationType,float);

    void DeleteActiveOperations();

    private:
    static OperationHolder oh;
   

    void SetHolded(OperationHolder);

    void CreateOperationC(Operation*,bool needBind);
};
















#endif