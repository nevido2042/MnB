// Fill out your copyright notice in the Description page of Project Settings.


#include "Weapons/Bow.h"
#include "Components/CapsuleComponent.h"

ABow::ABow()
{
	SkeletalMeshComponent = CreateDefaultSubobject<USkeletalMeshComponent>(TEXT("SkeletalMeshComponent"));
	SkeletalMeshComponent->SetupAttachment(RootComponent);

	SkeletalMeshComponent->SetSimulatePhysics(true);
	SkeletalMeshComponent->SetCanEverAffectNavigation(false);

	GuardCollider->DestroyComponent();
}
