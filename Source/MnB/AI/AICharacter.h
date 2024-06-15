// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "BehaviorTree/BehaviorTree.h"
#include "PatrolPath.h"
#include "GameFramework/Character.h"
#include "Animation/AnimMontage.h"
#include "CombatInterface.h"
#include "AICharacter.generated.h"

UCLASS()
class MNB_API AAICharacter : public ACharacter, public ICombatInterface
{
	GENERATED_BODY()

public:
	// Sets default values for this character's properties
	AAICharacter();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	// Called to bind functionality to input
	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;

	UBehaviorTree* GetBehaviorTree() const;

	APatrolPath* GetPatrolPath() const;

	UAnimMontage* GetMontage() const;

	int MeleeAttack_Implementation() override;

	class AWeapon* GetCurrentWeapon() { return CurWeapon; }
protected:
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "AI", meta = (AllowPrivateAccess = "true"))
	UBehaviorTree* Tree;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "AI", meta = (AllowPrivateAccess = "true"))
	APatrolPath* PatrolPath;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Animation, meta = (AllowPrivateAccess = "true"))
	UAnimMontage* Montage;

	TSubclassOf<class AWeapon> WeaponAsset = nullptr;

	class AWeapon* CurWeapon = nullptr;

	void EquipWeapon();

	UAnimMontage* LeftAttack = nullptr;
	UAnimMontage* RightAttack = nullptr;

	UAnimMontage* DamagedMontage = nullptr;

	class UHealth* Health = nullptr;

public:
	void PlayAttack(bool bLeft);

private:
	virtual float TakeDamage(float Damage, struct FDamageEvent const& DamageEvent, AController* EventInstigator, AActor* DamageCauser) override;

	bool bDie = false;
	void Die();
public:
	bool IsDie() { return bDie; }

private:
	void MyDestroy();

	UPROPERTY(Editanywhere)
	class UAudioComponent* AudioComponent = nullptr;
};
