#pragma once

#include "InteractableActor.generated.h"

UINTERFACE(Blueprintable)
class UInteractableActor : public UInterface
{
    GENERATED_BODY()
};

class IInteractableActor
{    
    GENERATED_BODY()

public:
    virtual void Interact() = 0;
};