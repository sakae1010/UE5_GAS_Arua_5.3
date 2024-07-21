
#include "UI/WidgetController/SpellMenuWidgetController.h"
#include "AuraGameplayTag.h"
#include "AbilitySystem/AuraAbilitySystemComponent.h"
#include "AbilitySystem/Data/AbilityInfo.h"
#include "Player/AuraPlayerState.h"

void USpellMenuWidgetController::BroadcastInitValues()
{
	BroadcastAbilityInfo();
	OnSpellPointChanged.Broadcast(GetAuraPlayerState()->GetSpellPoints());
}

void USpellMenuWidgetController::BindCallBacksToDependencies()
{
	GetAuraAbilitySystemComponent()->AbilityStatusChangedDelegate.AddLambda([this](const FGameplayTag& AbilityTag , const FGameplayTag& StatusTag ,int32 AbilityLevel)
	{
		if(SelectAbilty.AbilityTag .MatchesTagExact(AbilityTag))
		{
			SelectAbilty.StatusTag = StatusTag;
			ChangeButtonState(AbilityTag ,StatusTag,CurrentSpellPoints);
		}
	
		
		if(AbilityWidgetDataTable)
		{
			FAuraAbilityInfo Info = AbilityWidgetDataTable->FindAbilityInfoForTag(AbilityTag);
			Info.StatusTag = StatusTag;
			AbilityInfoDelegate.Broadcast(Info);
		}
	});
	GetAuraAbilitySystemComponent()->AbilityEquipedDelegate.AddUObject(this,&USpellMenuWidgetController::OnAbilityEquiped);
	
	GetAuraPlayerState()->OnSpellPointsChangedDelegate.AddLambda([this](const int InSpellPoints)
	{
		OnSpellPointChanged.Broadcast(InSpellPoints);
		CurrentSpellPoints = InSpellPoints;
		FString Description;
		FString NextLevelDescription;
		ChangeButtonState(SelectAbilty.AbilityTag  ,SelectAbilty.StatusTag , InSpellPoints);
	});

}

void USpellMenuWidgetController::SpellGlobeSelected(const FGameplayTag& AbilityTag)
{
	if(bWaitingForEquipSelection)
	{
		const FGameplayTag SelectAbilityType = AbilityWidgetDataTable->FindAbilityInfoForTag(SelectAbilty.AbilityTag).AbilityType;
		StopWaittingForEquipDelegate.Broadcast(SelectAbilityType);
		bWaitingForEquipSelection = false;
	}
	const int32 SpellPoints = GetAuraPlayerState()->GetSpellPoints();
	FGameplayTag AbilityStatusTag;
	
	const FAuraGameplayTags GameplayTags = FAuraGameplayTags::Get();
	const bool bTagValid = AbilityTag.IsValid();
	const bool bTagNone = AbilityTag.MatchesTag(GameplayTags.Abilities_None);
	const FGameplayAbilitySpec* AbilitySpec = GetAuraAbilitySystemComponent()->GetSpecFormAbilityTag(AbilityTag) ;
	const bool bSpecValid = AbilitySpec!= nullptr;


	if(!bTagValid || !bSpecValid || bTagNone)
	{
		AbilityStatusTag = GameplayTags.Abilities_Status_Locked;
	}else
	{
		AbilityStatusTag = GetAuraAbilitySystemComponent()->GetStatusTagFromSpec(*AbilitySpec);
	}
	SelectAbilty.AbilityTag = AbilityTag;
	SelectAbilty.StatusTag = AbilityStatusTag;
	
	ChangeButtonState(AbilityTag ,AbilityStatusTag , SpellPoints);
	// bool bEquipButtonEnable = false;
	// bool bSpendPointsButtonEnable = false;
	// ShouldEnableButtons(AbilityStatusTag , SpellPoints ,bSpendPointsButtonEnable,bEquipButtonEnable);
	// SpellGlobeSelectDelegate.Broadcast(bSpendPointsButtonEnable,bEquipButtonEnable);
}

void USpellMenuWidgetController::SpendButtonClick()
{
	if(GetAuraAbilitySystemComponent())
	{
		GetAuraAbilitySystemComponent()->ServerSpendSpellPoint(SelectAbilty.AbilityTag);
	}
}

void USpellMenuWidgetController::GlobeDeselect()
{
	if(bWaitingForEquipSelection)
	{
		const FGameplayTag SelectAbilityType = AbilityWidgetDataTable->FindAbilityInfoForTag(SelectAbilty.AbilityTag).AbilityType;
		StopWaittingForEquipDelegate.Broadcast(SelectAbilityType);
		bWaitingForEquipSelection = false;
	}
	const FAuraGameplayTags GameplayTags = FAuraGameplayTags::Get();
	SelectAbilty.AbilityTag = GameplayTags.Abilities_Type_None;
	SelectAbilty.StatusTag = GameplayTags.Abilities_Status_Locked;
	SpellGlobeSelectDelegate.Broadcast(false,false,FString(),FString());
}

void USpellMenuWidgetController::EquipButtonPressed()
{
	const FAuraAbilityInfo AbilityInfo = AbilityWidgetDataTable->FindAbilityInfoForTag(SelectAbilty.AbilityTag);
	const FGameplayTag& AbilityType = AbilityInfo.AbilityType;
	WaitForEquipSelectionDelegate.Broadcast(AbilityType);
	bWaitingForEquipSelection = true;
	auto SelectStatusTag = GetAuraAbilitySystemComponent()->GetStatusTagFromAbilityTag(SelectAbilty.AbilityTag);
	if(SelectStatusTag.MatchesTag(FAuraGameplayTags::Get().Abilities_Status_Equipped))
	{
		SelectSlot = GetAuraAbilitySystemComponent()->GetInputTagFromAbilityTag(SelectAbilty.AbilityTag);
	}
	

}

void USpellMenuWidgetController::SpellRowGlobePressed(const FGameplayTag& SlotTag, const FGameplayTag& AbilityType)
{
	if (!bWaitingForEquipSelection) return ;
	const FGameplayTag SelectedAbilityType = AbilityWidgetDataTable->FindAbilityInfoForTag(SelectAbilty.AbilityTag).AbilityType;
	if (!SelectedAbilityType.MatchesTagExact(AbilityType)) return;
	GetAuraAbilitySystemComponent()->ServerEquipAbility(SelectAbilty.AbilityTag,SlotTag);
}

void USpellMenuWidgetController::OnAbilityEquiped(const FGameplayTag& AbilityTag, const FGameplayTag& StatusTag,const FGameplayTag& Slot, const FGameplayTag& PreviousSlot)
{
	bWaitingForEquipSelection = false;
	const FAuraGameplayTags AuraGameplayTag = FAuraGameplayTags::Get();

	FAuraAbilityInfo LastSlotInfo;
	LastSlotInfo.StatusTag = AuraGameplayTag.Abilities_Status_Unlocked;
	LastSlotInfo.InputTag = PreviousSlot;
	LastSlotInfo.AbilityTag  = AuraGameplayTag.Abilities_None;
	// 只通知 已裝備的Ability
	AbilityInfoDelegate.Broadcast(LastSlotInfo);
	
	FAuraAbilityInfo Info = AbilityWidgetDataTable->FindAbilityInfoForTag(AbilityTag);
	Info.StatusTag = StatusTag;
	Info.InputTag = Slot;
	AbilityInfoDelegate.Broadcast(Info);

	StopWaittingForEquipDelegate.Broadcast(Info.AbilityType);
	SpellGlobeReassignedDelegate.Broadcast(AbilityTag);
	GlobeDeselect();
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

void USpellMenuWidgetController::ChangeButtonState(const FGameplayTag AbilityTag ,const FGameplayTag StatusTag ,const int32 SpellPoints )
{
	bool bEquipButtonEnable = false;
	bool bSpendPointsButtonEnable = false;
	
	ShouldEnableButtons(StatusTag, SpellPoints ,bSpendPointsButtonEnable,bEquipButtonEnable);
	FString Description;
	FString NextLevelDescription;
	GetAuraAbilitySystemComponent()->GetDescriptionsByAbilityTag(AbilityTag , Description  , NextLevelDescription);
	SpellGlobeSelectDelegate.Broadcast(bSpendPointsButtonEnable,bEquipButtonEnable, Description , NextLevelDescription);
}
