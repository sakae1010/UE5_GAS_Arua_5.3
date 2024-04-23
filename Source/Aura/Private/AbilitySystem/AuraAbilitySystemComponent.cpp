// sakae's gas project


#include "AbilitySystem/AuraAbilitySystemComponent.h"

#include "AuraGameplayTag.h"
#include "AbilitySystem/Abilities/AuraGameAbility.h"


void UAuraAbilitySystemComponent::AbilityActorInfoInit()
{
	OnGameplayEffectAppliedDelegateToSelf.AddUObject(this, &UAuraAbilitySystemComponent::OnGameplayEffectAppliedCallback);
}

void UAuraAbilitySystemComponent::AddCharacterAbilities(TArray<TSubclassOf<UGameplayAbility>>& InAbilities)
{
	for (const TSubclassOf<UGameplayAbility> Ability : InAbilities)
	{
		if (!Ability)continue;
		FGameplayAbilitySpec AbilitySpec = FGameplayAbilitySpec(Ability, 1);\
		if(const UAuraGameAbility* AuraAbility = Cast<UAuraGameAbility>(AbilitySpec.Ability))
		{
			AbilitySpec.DynamicAbilityTags.AddTag(AuraAbility->StartInputTag);
			GiveAbility(AbilitySpec);
		}
	
		
		// GiveAbilityAndActivateOnce(AbilitySpec);
		
	}
}

void UAuraAbilitySystemComponent::AbilityInputHeld(const FGameplayTag& InputTag)
{
	if(!InputTag.IsValid())return;
	for ( FGameplayAbilitySpec& AbilitySpec : GetActivatableAbilities())
	{
		if(!AbilitySpec.DynamicAbilityTags.HasTagExact(InputTag))continue;
		AbilitySpecInputPressed(AbilitySpec);
		if(AbilitySpec.IsActive())continue;
		TryActivateAbility(AbilitySpec.Handle);
	}
}

void UAuraAbilitySystemComponent::AbilityInputReleased(const FGameplayTag& InputTag)
{
	if(!InputTag.IsValid())return;
	for ( FGameplayAbilitySpec& AbilitySpec : GetActivatableAbilities())
	{
		if(!AbilitySpec.DynamicAbilityTags.HasTagExact(InputTag))continue;
		AbilitySpecInputReleased(AbilitySpec);
	}
}

void UAuraAbilitySystemComponent::OnGameplayEffectAppliedCallback(UAbilitySystemComponent* AbilitySystemComponent,
                                                                  const FGameplayEffectSpec& Spec, FActiveGameplayEffectHandle Handle) const
{
	FGameplayTagContainer TagContainer;
	Spec.GetAllAssetTags(TagContainer);
	EffectAssetTags.Broadcast(TagContainer);
}
