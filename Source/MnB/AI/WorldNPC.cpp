// Fill out your copyright notice in the Description page of Project Settings.


#include "AI/WorldNPC.h"
#include "Components/SphereComponent.h"
AWorldNPC::AWorldNPC()
{
	Sphere = CreateDefaultSubobject<USphereComponent>(TEXT("Sphere"));
	Sphere->SetupAttachment(GetMesh());
}
