// Fill out your copyright notice in the Description page of Project Settings.


#include "Data/Input/BasicInputDataConfig.h"

UBasicInputDataConfig::UBasicInputDataConfig()
{
	{
		static ConstructorHelpers::FObjectFinder<UInputMappingContext> Asset
		{TEXT("/Script/EnhancedInput.InputMappingContext'/Game/MyAssets/VR/Input/BaseInput/IMC_Base.IMC_Base'")};
		check(Asset.Succeeded());
		InputMappingContext = Asset.Object;
	}
	{
		static ConstructorHelpers::FObjectFinder<UInputAction> Asset
		{ TEXT("/Script/EnhancedInput.InputAction'/Game/MyAssets/VR/Input/BaseInput/IA_VRMove.IA_VRMove'") };
		check(Asset.Succeeded());
		Move = Asset.Object;
	}
	{
		static ConstructorHelpers::FObjectFinder<UInputAction> Asset
		{ TEXT("/Script/EnhancedInput.InputAction'/Game/MyAssets/VR/Input/BaseInput/IA_LookMouse.IA_LookMouse'") };
		check(Asset.Succeeded());
		Look = Asset.Object;
	}
}
