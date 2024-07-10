// sakae's gas project
#include "AbilitySystem/AuraAbilitySystemComponent.h"
#include "AbilitySystem/Data/AbilityInfo.h"
#include "UI/WidgetController/SpellMenuWidgetController.h"
#include "AuraGameplayTag.h"
#include "Player/AuraPlayerState.h"

void USpellMenuWidgetController::BroadcastInitValues()
{
	BroadcastAbilityInfo();
	OnSpellPointChanged.Broadcast(GetAuraPlayerState()->GetSpellPoints());
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

	GetAuraPlayerState()->OnSpellPointsChangedDelegate.AddLambda([this](int InSpellPoints)
	{
		OnSpellPointChanged.Broadcast(InSpellPoints);

	});
}

void USpellMenuWidgetController::SpellGlobeSelected(const FGameplayTag& AbilityTag)
{
	const int32 SpellPoints = GetAuraPlayerState()->GetSpellPoints();
	FGameplayTag AbilityStatusTag;
	
	const FAuraGameplayTags GameplayTags = FAuraGameplayTags::Get();
	const bool bTagValid = AbilityTag.IsValid();
	const bool bTagNone = AbilityTag.MatchesTag(GameplayTags.Abilities_None);
	const FGameplayAbilitySpec* AbilitySpec = GetAuraAbilitySystemComponent()->GetGameplayAbilitySpec(AbilityTag) ;
	const bool bSpecValid = AbilitySpec!= nullptr;


	if(!bTagValid || !bSpecValid || bTagNone)
	{
		AbilityStatusTag = GameplayTags.Abilities_Status_Locked;
	}else
	{
		AbilityStatusTag = GetAuraAbilitySystemComponent()->GetStatusTagFromSpec(*AbilitySpec);
	}
	bool bEquipButtonEnable = false;
	bool bSpendPointsButtonEnable = false;
	ShouldEnableButtons(AbilityStatusTag , SpellPoints ,bSpendPointsButtonEnable,bEquipButtonEnable);
	
	SpellGlobeSelectDelegate.Broadcast(bSpendPointsButtonEnable,bEquipButtonEnable);
}

void USpellMenuWidgetController::ShouldEnableButtons(const FGameplayTag& StatusTag,const int32 SpellPoints,	bool& bEnableSpellPointsButton, bool& bEnableEquipButton)
{
	const FAuraGameplayTags GameplayTags = FAuraGameplayTags::Get();
	bEnableSpellPointsButton = false;
	bEnableEquipButton = false;
	if(StatusTag.MatchesTag(GameplayTags.Abilities_Status_Equipped))
	{
		bEnableEquipButton = true;
		if (SpellPoints>0)
		{
			bEnableSpellPointsButton = true;
		}
	}else if(StatusTag.MatchesTag(GameplayTags.Abilities_Status_Eligible))
	{
		bEnableEquipButton = false;

		if (SpellPoints>0)
		{
			bEnableSpellPointsButton = true;
		}
	}
	else if(StatusTag.MatchesTag(GameplayTags.Abilities_Status_Unlocked))
	{
		bEnableEquipButton = true;
		if (SpellPoints>0)
		{
			bEnableSpellPointsButton = true;
		}
	}
	
}
