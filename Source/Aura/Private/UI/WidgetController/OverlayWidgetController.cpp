// sakae's gas project


#include "UI/WidgetController/OverlayWidgetController.h"

#include "AbilitySystem/AuraAbilitySystemComponent.h"
#include "AbilitySystem/AuraAttributeSet.h"

void UOverlayWidgetController::BroadcastInitValues()
{
	const UAuraAttributeSet* AuraAttributeSet = CastChecked<UAuraAttributeSet>(this->AttributeSet);
	OnHealthChanged.Broadcast(AuraAttributeSet->GetHealth());
	OnMaxHealthChanged.Broadcast(AuraAttributeSet->GetMaxHealth());


	OnManaChanged.Broadcast(AuraAttributeSet->GetMana());
	OnMaxManaChanged.Broadcast(AuraAttributeSet->GetMaxMana());
	
}

void UOverlayWidgetController::BindCallBacksToDependencies()
{
	//bind to health
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

void UOverlayWidgetController::OnInitalizeStartupAbilities(UAuraAbilitySystemComponent* AuraAbilitySystemComponent)
{

	//TODO 取得 所有取得的Ability , look up their Ability Info  透過MessageWidgetRowDelegate 廣播出去
	if(!AuraAbilitySystemComponent)return;
	if(!AuraAbilitySystemComponent->bStartupAbilitiesGiven) return;
	// for (const FGameplayAbilitySpec& AbilitySpec : AuraAbilitySystemComponent->GetActivatableAbilities())
	// {
	// 	if(const UAuraGameAbility* AuraAbility = Cast<UAuraGameAbility>(AbilitySpec.Ability))
	// 	{
	// 		if(const FUIWidgetRow* Row = GetDataTable<FUIWidgetRow>(AbilityWidgetDataTable, AuraAbility->StartInputTag))
	// 		{
	// 			MessageWidgetRowDelegate.Broadcast(*Row);
	// 		}
	// 	}
	// }
}


