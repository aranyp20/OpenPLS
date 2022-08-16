#include "Factory.h"
#include "Mesh.h"


Factory::Factory(Surface* _surface) : surface(_surface){}

Factory::OperationHolder::OperationHolder() : active(false){}

Factory::OperationHolder::OperationHolder(InputAnswer::OperationType _type,CreationAddons _addons) : active(true), type(_type), addons(_addons){}

Operation* Factory::CreateOperation(InputAnswer::OperationType which,Factory::CreationAddons addons, bool delayed)
{
    if(delayed){
        SetHolded(OperationHolder(which,addons));
        return NULL;
    }


    Operation* createdOperation;


    Mesh* activeMesh = surface->meshHandler->activeMesh;


    switch(which){
        case InputAnswer::OperationType::VERT_EXTRUDE: 
            createdOperation = new OVertExtrude(activeMesh);
        break;
        case InputAnswer::OperationType::VERT_MOVE:
            createdOperation = new OVertMove(activeMesh,addons.pvec3,surface->viewCamera);
            return createdOperation;
        break;
        case InputAnswer::OperationType::CAMERA_MOVE:
            createdOperation = new OCameraMove(surface->viewCamera);
        break;
        case InputAnswer::OperationType::VERT_SCALE:
            createdOperation = new OVertScale(activeMesh);
        break;
        case InputAnswer::OperationType::VERT_ROTATE:
            createdOperation = new OVertRotate(activeMesh,vec3(1,0,0));
        break;
        default:return NULL;break;
    }

    InputManager::ChangeBind(createdOperation);

    return createdOperation;
}

Operation* Factory::TriggerHolded()
{
    
    if(oh.active){
        oh.active=false;
        return CreateOperation(oh.type,oh.addons);
    }
    return NULL;
}

void Factory::SetHolded(OperationHolder _oh){oh = _oh;}

void Factory::ReleaseHolded(){oh.active = false;}

Factory::CreationAddons::CreationAddons(){}
Factory::CreationAddons::CreationAddons(vec3 _v) : pvec3(_v){}


