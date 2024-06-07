// Fill out your copyright notice in the Description page of Project Settings.


#include "AI/AICharacter.h"

// Sets default values
AAICharacter::AAICharacter()
{
 	// Set this character to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

}

// Called when the game starts or when spawned
void AAICharacter::BeginPlay()
{
	Super::BeginPlay();
	
}

#include "Runtime/Engine/Classes/Kismet/GameplayStatics.h"
#include "Kismet/KismetMathLibrary.h"
// Called every frame
void AAICharacter::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	//{
	//	auto const* const Player = UGameplayStatics::GetPlayerCharacter(GetWorld(), 0);

	//	auto* const Cont = GetController();
	//	auto* const Pawn = Cont->GetPawn();

	//	const FVector MyLocation = Cont->GetPawn()->GetActorLocation();
	//	const FVector TargetLocation = Player->GetActorLocation(); //���߿� �÷��̾�� Ÿ������ �ٲ㺸��

	//	FRotator NewControlRotation = UKismetMathLibrary::MakeRotFromX((TargetLocation - MyLocation).GetSafeNormal());
	//	//FRotator New = FRotator(Pawn->GetActorRotation().Pitch, NewControlRotation.Yaw, Pawn->GetActorRotation().Roll);
	//	FRotator NewRotation = FRotator(0.f, NewControlRotation.Yaw, NewControlRotation.Roll);

	//	Cont->GetPawn()->SetActorRotation(NewRotation);

	//	//UE_LOG(LogTemp, Display, TEXT("Pitch:%f Yaw:%f Roll:%f"), NewControlRotation.Pitch, NewControlRotation.Yaw, NewControlRotation.Roll);
	//	//Cont->SetControlRotation(New);
	//}
}

// Called to bind functionality to input
void AAICharacter::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);

}

UBehaviorTree* AAICharacter::GetBehaviorTree() const
{
	return Tree;
}

APatrolPath* AAICharacter::GetPatrolPath() const
{
	return PatrolPath;
}

UAnimMontage* AAICharacter::GetMontage() const
{
	return Montage;
}

int AAICharacter::MeleeAttack_Implementation()
{
	if (Montage)
	{
		PlayAnimMontage(Montage);
	}

	return 0;
}

