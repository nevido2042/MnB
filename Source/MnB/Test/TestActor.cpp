// Fill out your copyright notice in the Description page of Project Settings.


#include "Test/TestActor.h"
#include "Components/CapsuleComponent.h"

// Sets default values
ATestActor::ATestActor()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	StaticMeshComponent = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("StaticMeshComponent"));
	SetRootComponent(StaticMeshComponent);

	CapsuleComponent = CreateDefaultSubobject<UCapsuleComponent>(TEXT("CapsuleComponent"));
	CapsuleComponent->SetupAttachment(StaticMeshComponent);

	//CapsuleComponent->SetCollisionEnabled(ECollisionEnabled::QueryOnly);
	//CapsuleComponent->SetCollisionObjectType(ECollisionChannel::ECC_GameTraceChannel1);
	//CapsuleComponent->SetCollisionResponseToAllChannels(ECollisionResponse::ECR_Overlap);

	CapsuleComponent->OnComponentBeginOverlap.AddDynamic(this, &ATestActor::OnCapsuleBeginOverlap);
	CapsuleComponent->SetGenerateOverlapEvents(true);

}

// Called when the game starts or when spawned
void ATestActor::BeginPlay()
{
	Super::BeginPlay();
	
}

// Called every frame
void ATestActor::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

void ATestActor::OnCapsuleBeginOverlap(UPrimitiveComponent* OverlappedComp, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
}

