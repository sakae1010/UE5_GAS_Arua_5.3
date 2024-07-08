// sakae's gas project


#include "AbilitySystem/AuraAbilitySystemComponent.h"
#include "AbilitySystem/Data/AbilityInfo.h"
#include "UI/WidgetController/SpellMenuWidgetController.h"

void USpellMenuWidgetController::BroadcastInitValues()
{
	BroadcastAbilityInfo();
}

void USpellMenuWidgetController::BindCallBacksToDependencies()
{
	GetAuraAbilitySystemComponent()->AbilityStatusChangedDelegate.AddLambda([this](const FGameplayTag& AbilityTag , const FGameplayTag& StatusTag)
	{
		if(AbilityWidgetDataTable)
		{
			FAuraAbilityInfo Info = AbilityWidgetDataTable->FindAbilityInfForTag(AbilityTag);
			Info.StatusTag = StatusTag;
			AbilityInfoDelegate.Broadcast(Info);
		}
	});
}
