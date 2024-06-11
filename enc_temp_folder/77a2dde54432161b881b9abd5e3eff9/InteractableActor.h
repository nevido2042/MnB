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
    virtual void Interact(AActor* InActor = nullptr) = 0;

protected:
    	FText Info = FText::FromString("Pick Up Weapon");
public:
    	FText& GetInfo() { return Info; }
};