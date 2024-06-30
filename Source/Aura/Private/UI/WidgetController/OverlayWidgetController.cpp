// sakae's gas project


#include "UI/WidgetController/OverlayWidgetController.h"

#include "AbilitySystem/AuraAbilitySystemComponent.h"
#include "AbilitySystem/AuraAttributeSet.h"
#include "AbilitySystem/Data/AbilityInfo.h"
#include "AbilitySystem/Data/LevelUpInfo.h"

void UOverlayWidgetController::BroadcastInitValues()
{
	const UAuraAttributeSet* AuraAttributeSet = CastChecked<UAuraAttributeSet>(this->AttributeSet);
	check(AuraAttributeSet);


	OnHealthChanged.Broadcast(AuraAttributeSet->GetHealth());
	OnMaxHealthChanged.Broadcast(AuraAttributeSet->GetMaxHealth());

	OnManaChanged.Broadcast(AuraAttributeSet->GetMana());
	OnMaxManaChanged.Broadcast(AuraAttributeSet->GetMaxMana());
	
}

void UOverlayWidgetController::BindCallBacksToDependencies()
{
	//bind to health
	AAuraPlayerState* AuraPlayerState = CastChecked<AAuraPlayerState>(this->PlayerState);
	AuraPlayerState->OnXPChangedDelegate.AddUObject(this, &UOverlayWidgetController::OnXPChanged);
	AuraPlayerState->OnLevelChangedDelegate.AddLambda([this](const int32 Level)
	{
		OnLevelChanged.Broadcast(Level);
	});
	
	const UAuraAttributeSet* AuraAttributeSet = CastChecked<UAuraAttributeSet>(this->AttributeSet);

	
	AbilitySystemComponent->GetGameplayAttributeValueChangeDelegate(
		AuraAttributeSet->GetHealthAttribute()).AddLambda(
			[this](const FOnAttributeChangeData& Data)->void
			{
				OnHealthChanged.Broadcast(Data.NewValue);
			});

	AbilitySystemComponent->GetGameplayAttributeValueChangeDelegate(
		AuraAttributeSet->GetMaxHealthAttribute()).AddLambda(
			[this](const FOnAttributeChangeData& Data)->void
			{
				OnMaxHealthChanged.Broadcast(Data.NewValue);
			});
	
	
	AbilitySystemComponent->GetGameplayAttributeValueChangeDelegate(
		AuraAttributeSet->GetManaAttribute()).AddLambda(
			[this](const FOnAttributeChangeData& Data)->void
			{
				OnManaChanged.Broadcast(Data.NewValue);
			});

	AbilitySystemComponent->GetGameplayAttributeValueChangeDelegate(
	AuraAttributeSet->GetMaxManaAttribute()).AddLambda(
		[this](const FOnAttributeChangeData& Data)->void
		{
			OnMaxManaChanged.Broadcast(Data.NewValue);
		});
	
	if(UAuraAbilitySystemComponent* AuraASC = Cast<UAuraAbilitySystemComponent>(AbilitySystemComponent))
	{
		if (AuraASC->bStartupAbilitiesGiven)
		{
			OnInitalizeStartupAbilities(AuraASC);	
		}else
		{
			AuraASC->AbilitiesGivenDelegate.AddUObject(this, &UOverlayWidgetController::OnInitalizeStartupAbilities);
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

void UOverlayWidgetController::OnXPChanged(int32 NewXP) const
{
	const AAuraPlayerState* AuraPlayerState = CastChecked<AAuraPlayerState>(this->PlayerState);
	const ULevelUpInfo* LevelUpInfo = AuraPlayerState->LevelUpInfo;
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


void UOverlayWidgetController::OnInitalizeStartupAbilities(UAuraAbilitySystemComponent* AuraAbilitySystemComponent)
{

	//TODO 取得 所有取得的Ability , look up their Ability Info  透過MessageWidgetRowDelegate 廣播出去
	if(!AuraAbilitySystemComponent)return;
	if(!AuraAbilitySystemComponent->bStartupAbilitiesGiven) return;

	FForEachAbility Delegate;
	Delegate.BindLambda([this , AuraAbilitySystemComponent](const FGameplayAbilitySpec& AbilitySpec)->void
	{
		FAuraAbilityInfo AbilityInfo = AbilityWidgetDataTable->FindAbilityInfForTag(AuraAbilitySystemComponent->GetAbilityFromSpec(AbilitySpec));
		AbilityInfo.InputTag = AuraAbilitySystemComponent->GetInputTagFromSpec(AbilitySpec);
		AbilityInfoDelegate.Broadcast(AbilityInfo);
	});
	AuraAbilitySystemComponent->ForEachAbility(Delegate);
}


