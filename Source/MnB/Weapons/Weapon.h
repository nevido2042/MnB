// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "MnB/Interface/InteractableActor.h"
#include "Components/ArrowComponent.h"
#include "Weapon.generated.h"

UCLASS()
class MNB_API AWeapon : public AActor, public IInteractableActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	AWeapon();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

protected:
	UPROPERTY(EditAnywhere)
	UStaticMeshComponent* StaticMeshComponent = nullptr;

	UPROPERTY(EditAnywhere)
	USceneComponent* SceneComponent = nullptr;

	UPROPERTY(EditAnywhere)
	USceneComponent* WeaponGrip = nullptr;

	UArrowComponent* Arrow = nullptr;

	AController* Owner;

	bool bApplyDamage = false;

	bool bOwnerVR = false;

	bool bHitVRMode = false;

	UPROPERTY(EditAnywhere)
	class UCapsuleComponent* GuardCollider = nullptr;

protected:

	virtual void Interact(AActor* InActor) override;

	UFUNCTION()
	void SetWeaponHitable();

	FTimerHandle Timer;

	TArray<UParticleSystem*> Particles; //blood 0 , default 1
	UParticleSystemComponent* ParticleComponent = nullptr;

public:
	class UCapsuleComponent* GetGuardCollider() { return GuardCollider; }

	USceneComponent* GetWeaponGrip() { return WeaponGrip; }

	void Unequipped();

	void Equipped();

	void Equipped(AController* Controller);

	void HitDetectStart();
	void HitDetect();
	void HitDetectImpl(FHitResult& InHitResult);

	bool ObstacleDitect();
	
	void SetIsApplyDamage(bool value) { bApplyDamage = value; }

protected:
	UPROPERTY(EditAnywhere)
	class UAudioComponent* Audio = nullptr;

	UPROPERTY(EditAnywhere)
	TArray<class USoundBase*> MetalSounds;

private:
	void SetRandomSoundAndPlay();

//private:
//	FText Info = FText::FromString("Pick Up Weapon");
//public:
//	FText* GetInfo() { return &Info; }

	UPROPERTY(EditAnywhere)
	FText WeaponName;

	friend class FLastCache;
	FVector LastHitStart;
	FVector LastHitEnd;
};
