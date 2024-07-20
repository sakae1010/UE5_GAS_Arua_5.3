// sakae's gas project


#include "UI/WidgetController/OverlayWidgetController.h"

#include "AuraGameplayTag.h"
#include "AbilitySystem/AuraAbilitySystemComponent.h"
#include "AbilitySystem/AuraAttributeSet.h"
#include "AbilitySystem/Data/AbilityInfo.h"
#include "AbilitySystem/Data/LevelUpInfo.h"

void UOverlayWidgetController::BroadcastInitValues()
{
	OnHealthChanged.Broadcast(GetAuraAttributeSet()->GetHealth());
	OnMaxHealthChanged.Broadcast(GetAuraAttributeSet()->GetMaxHealth());

	OnManaChanged.Broadcast(GetAuraAttributeSet()->GetMana());
	OnMaxManaChanged.Broadcast(GetAuraAttributeSet()->GetMaxMana());
	
}

void UOverlayWidgetController::BindCallBacksToDependencies()
{
	//bind to health
	GetAuraPlayerState()->OnXPChangedDelegate.AddUObject(this, &UOverlayWidgetController::OnXPChanged);
	GetAuraPlayerState()->OnLevelChangedDelegate.AddLambda([this](const int32 Level)
	{
		OnLevelChanged.Broadcast(Level);
	});
	
	AbilitySystemComponent->GetGameplayAttributeValueChangeDelegate(
		GetAuraAttributeSet()->GetHealthAttribute()).AddLambda(
			[this](const FOnAttributeChangeData& Data)->void
			{
				OnHealthChanged.Broadcast(Data.NewValue);
			});

	AbilitySystemComponent->GetGameplayAttributeValueChangeDelegate(
		GetAuraAttributeSet()->GetMaxHealthAttribute()).AddLambda(
			[this](const FOnAttributeChangeData& Data)->void
			{
				OnMaxHealthChanged.Broadcast(Data.NewValue);
			});
	
	
	AbilitySystemComponent->GetGameplayAttributeValueChangeDelegate(
		GetAuraAttributeSet()->GetManaAttribute()).AddLambda(
			[this](const FOnAttributeChangeData& Data)->void
			{
				OnManaChanged.Broadcast(Data.NewValue);
			});

	AbilitySystemComponent->GetGameplayAttributeValueChangeDelegate(
		GetAuraAttributeSet()->GetMaxManaAttribute()).AddLambda(
		[this](const FOnAttributeChangeData& Data)->void
		{
			OnMaxManaChanged.Broadcast(Data.NewValue);
		});
	if(UAuraAbilitySystemComponent* AuraASC  = GetAuraAbilitySystemComponent())
	{
		
		AuraASC->AbilityEquipedDelegate.AddUObject(this,&UOverlayWidgetController::OnAbilityEquiped);

		
		if (AuraASC->bStartupAbilitiesGiven)
		{
			BroadcastAbilityInfo();
		}else
		{
			AuraASC->AbilitiesGivenDelegate.AddUObject(this, &UOverlayWidgetController::BroadcastAbilityInfo);
		}
	
		AuraASC->EffectAssetTags.AddLambda(
			[this](const FGameplayTagContainer& AssetTags)->void
			{
				for ( const FGameplayTag& Tag : AssetTags)
				{
					FGameplayTag MessageTag = FGameplayTag::RequestGameplayTag(FName("Message"));
					if(Tag.MatchesTag(MessageTag))
					{
						if(const FUIWidgetRow* Row = GetDataTable<FUIWidgetRow>(MessageWidgetDataTable, Tag))
						{
							MessageWidgetRowDelegate.Broadcast(*Row);
						}
						else
						{
							UE_LOG(LogTemp, Warning, TEXT("No row found for tag %s"), *Tag.ToString());
						}
					}
				}
			}
			);
	}
}

void UOverlayWidgetController::OnXPChanged(int32 NewXP)
{
	const ULevelUpInfo* LevelUpInfo = GetAuraPlayerState()->LevelUpInfo;
	checkf(LevelUpInfo , TEXT("LevelUpInfo is null check AuraPlayerState Blueprint "));

	const int32 Level = LevelUpInfo->FindLevelForXP(NewXP);
	const int32 MaxLevel = LevelUpInfo->LevelUpInfos.Num();
	
	if (Level <= MaxLevel && Level > 0)
	{
		const int32 CurrentLevelXP = LevelUpInfo->LevelUpInfos[Level].Exp;
		const int32 LastXp = Level == 0 ? 0 : LevelUpInfo->LevelUpInfos[Level-1].Exp;
		const float XPPercent = static_cast<float>((NewXP - LastXp) ) / static_cast<float>((CurrentLevelXP - LastXp));
		OnXPPercentChanged.Broadcast(XPPercent);
	}
	else
	{
		OnXPPercentChanged.Broadcast(1.0f);
	}
}

void UOverlayWidgetController::OnAbilityEquiped(const FGameplayTag& AbilityTag, const FGameplayTag& StatusTag,const FGameplayTag& Slot, const FGameplayTag& PreviousSlot) const
{
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
}

