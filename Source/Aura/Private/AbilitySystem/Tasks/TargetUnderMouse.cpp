// sakae's gas project


#include "AbilitySystem/Tasks/TargetUnderMouse.h"

#include "AbilitySystemComponent.h"
#include "GameFramework/PlayerController.h"


UTargetUnderMouse* UTargetUnderMouse::CreateTargetUnderMouse(UGameplayAbility* OwningAbility)
{
	UTargetUnderMouse* MyObj = NewAbilityTask<UTargetUnderMouse>(OwningAbility);

	return MyObj;
}


void UTargetUnderMouse::Activate()
{
	const bool bIsLocalPlayerController = Ability->GetCurrentActorInfo()->IsLocallyControlled();
	if(bIsLocalPlayerController)
	{
		SendMouseCursorData();
	}else
	{
		const FGameplayAbilitySpecHandle SpecHandle = GetAbilitySpecHandle();
		const FPredictionKey ActivationPredictionKey = GetActivationPredictionKey();
		AbilitySystemComponent.Get()->AbilityTargetDataSetDelegate(GetAbilitySpecHandle(), ActivationPredictionKey).AddUObject(this , &UTargetUnderMouse::OnTargetDataReplicatedCallback);
		const bool bCalledDelegate = AbilitySystemComponent.Get()->CallReplicatedTargetDataDelegatesIfSet(SpecHandle, ActivationPredictionKey);
		if(!bCalledDelegate)
		{
			SetWaitingOnRemotePlayerData();
			// AbilitySystemComponent.Get()->CallServerSetReplicatedTargetData(SpecHandle, ActivationPredictionKey, FGameplayAbilityTargetDataHandle(), FGameplayTag(), AbilitySystemComponent.Get()->ScopedPredictionKey);
		}
	}

}

void UTargetUnderMouse::SendMouseCursorData()
{
	FScopedPredictionWindow ScopedPrediction(AbilitySystemComponent.Get());
	
	APlayerController* PlayerController = Ability->GetCurrentActorInfo()->PlayerController.Get();
	FHitResult CursorHit;
	PlayerController->GetHitResultUnderCursor(ECC_Visibility, false, CursorHit);


	FGameplayAbilityTargetDataHandle Handle;
	FGameplayAbilityTargetData_SingleTargetHit* DataHandle = new FGameplayAbilityTargetData_SingleTargetHit();
	DataHandle->HitResult =CursorHit;
	Handle.Add(DataHandle);
	//FGameplayTag Tag = FGameplayTag::RequestGameplayTag("Target");
	AbilitySystemComponent->ServerSetReplicatedTargetData(
		GetAbilitySpecHandle(),
		GetActivationPredictionKey(),
		Handle,
		FGameplayTag(),
		AbilitySystemComponent->ScopedPredictionKey);

	if(ShouldBroadcastAbilityTaskDelegates())
	{
		ValidData.Broadcast(Handle);
	}

}


void UTargetUnderMouse::OnTargetDataReplicatedCallback(const FGameplayAbilityTargetDataHandle& DataHandle, FGameplayTag ActivationTag)
{
	AbilitySystemComponent->ConsumeClientReplicatedTargetData(GetAbilitySpecHandle(), GetActivationPredictionKey());
	if (ShouldBroadcastAbilityTaskDelegates())
	{
		ValidData.Broadcast(DataHandle);
	}
}
