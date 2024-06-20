// Fill out your copyright notice in the Description page of Project Settings.


#include "Item/Item.h"
#include "Components/BoxComponent.h"
#include "Data/Item/ItemData.h"

// Sets default values
AItem::AItem()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	Collider = CreateDefaultSubobject<UBoxComponent>(TEXT("Collider"));
	SetRootComponent(Collider);

	Mesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("Mesh"));
	Mesh->SetupAttachment(Collider);
}

// Called when the game starts or when spawned
void AItem::BeginPlay()
{
	Super::BeginPlay();
	
	SetInfo(FText::FromString(TEXT("Item\nPick Up")));

	DataRow = DataHandle.GetRow<FItemData>(TEXT(""));
}

// Called every frame
void AItem::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

#include "Controller/ControllerPC.h"
void AItem::Interact(AActor* InActor)
{
	if (APawn* Pawn = Cast<APawn>(InActor))
	{
		if (AControllerPC* Cont = Cast<AControllerPC>(Pawn->GetController()))
		{
			Cont->PickUpItem(DataRow->ItemName);
		}
	}
}

