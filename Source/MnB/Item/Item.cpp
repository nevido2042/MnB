#include "Item.h"
#include "Data/Item/ItemData.h"
#include "Subsystem/DataSubsystem.h"
#include "MnBCharacter.h"

//void UItem_Potion::UseItem(ARPGPlayerController* Controller, FItemData& ItemData)
//{
//	/*FDamageEvent DamageEvent;
//	Controller->GetPawn()->TakeDamage(10.f, DamageEvent, nullptr, nullptr);*/
//}
//
//void UItem_Weapon::UseItem(ARPGPlayerController* Controller, FItemData& ItemData)
//{
//	/*APawn* Pawn = Controller->GetPawn();
//	AKDT2Character* Character = Cast<AKDT2Character>(Pawn);
//	ensure(Character);
//
//	UDataSubsystem* DataSubsystem = Controller->GetWorld()->GetGameInstance()->GetSubsystem<UDataSubsystem>();
//	const FWeaponDataTableRow* WeaponDataTableRow = DataSubsystem->FindWeapon(ItemData.ItemName);
//	Character->SetWeaponData(WeaponDataTableRow);*/
//}
//
//void UItem_Weapon::UnEquipItem(ARPGPlayerController* Controller, FItemData& ItemData)
//{
//	/*APawn* Pawn = Controller->GetPawn();
//	AKDT2Character* Character = Cast<AKDT2Character>(Pawn);
//	ensure(Character);
//
//	UDataSubsystem* DataSubsystem = Controller->GetWorld()->GetGameInstance()->GetSubsystem<UDataSubsystem>();
//	const FWeaponDataTableRow* WeaponDataTableRow = DataSubsystem->FindWeapon(TEXT("Fist"));
//	Character->SetWeaponData(WeaponDataTableRow);*/
//}

void UItem_Chest::UseItem(AController* Controller, FItemData& ItemData)
{
	AMnBCharacter* MnBCharacter = Cast<AMnBCharacter>(Controller->GetPawn());
	MnBCharacter->GetMesh()->SetMaterial(1, ItemData.Material);
}

void UItem_Chest::UnEquipItem(AController* Controller, FItemData& ItemData)
{
	AMnBCharacter* MnBCharacter = Cast<AMnBCharacter>(Controller->GetPawn());
	MnBCharacter->SetInitChestMaterial();
}

void UItem_Head::UseItem(AController* Controller, FItemData& ItemData)
{
	AMnBCharacter* MnBCharacter = Cast<AMnBCharacter>(Controller->GetPawn());
	MnBCharacter->GetMesh()->SetMaterial(2, ItemData.Material);
}

void UItem_Head::UnEquipItem(AController* Controller, FItemData& ItemData)
{
	AMnBCharacter* MnBCharacter = Cast<AMnBCharacter>(Controller->GetPawn());
	MnBCharacter->SetInitHeadMaterial();
}
