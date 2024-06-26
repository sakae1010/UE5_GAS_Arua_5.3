// sakae's gas project


#include "AbilitySystem/AuraAbilitySystemComponent.h"

#include "AuraGameplayTag.h"
#include "AbilitySystem/Abilities/AuraGameAbility.h"
#include "Aura/AuraLogChannels.h"


void UAuraAbilitySystemComponent::AbilityActorInfoInit()
{
	OnGameplayEffectAppliedDelegateToSelf.AddUObject(this, &UAuraAbilitySystemComponent::ClientEffectApplied);
}

void UAuraAbilitySystemComponent::AddCharacterAbilities(TArray<TSubclassOf<UGameplayAbility>>& InAbilities)
{
	for (const TSubclassOf<UGameplayAbility> Ability : InAbilities)
	{
		if (!Ability)continue;
		FGameplayAbilitySpec AbilitySpec = FGameplayAbilitySpec(Ability, 1);
		if(const UAuraGameAbility* AuraAbility = Cast<UAuraGameAbility>(AbilitySpec.Ability))
		{
			AbilitySpec.DynamicAbilityTags.AddTag(AuraAbility->StartInputTag);
			GiveAbility(AbilitySpec);
		}		
	}
	bStartupAbilitiesGiven = true;
	AbilitiesGivenDelegate.Broadcast(this);
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

void UAuraAbilitySystemComponent::ForEachAbility(const FForEachAbility& Delegate)
{
	FScopedAbilityListLock AbilityLock(*this);
	for ( FGameplayAbilitySpec& AbilitySpec : GetActivatableAbilities())
	{
		if(!Delegate.ExecuteIfBound(AbilitySpec))
		{
			UE_LOG(LogAura, Error, TEXT("ForEachAbility Delegate failed [%hs]"), __FUNCTION__ );
		}
	}
}

FGameplayTag UAuraAbilitySystemComponent::GetAbilityFromSpec(const FGameplayAbilitySpec& AbilitySpec)
{
	if(AbilitySpec.Ability)
	{
		for (const FGameplayTag Tag : AbilitySpec.Ability.Get()->AbilityTags)
		{
			if(Tag.MatchesTag(FGameplayTag::RequestGameplayTag(FName("Abilities"))))
			{
				return Tag;
			}
		
		}
		
	}
	UE_LOG(LogAura, Error, TEXT("GetAbilityFromSpec failed [%hs]"), __FUNCTION__ );
	return FGameplayTag::EmptyTag;
}

FGameplayTag UAuraAbilitySystemComponent::GetInputTagFromSpec(const FGameplayAbilitySpec& AbilitySpec)
{
	for (const FGameplayTag Tag : AbilitySpec.DynamicAbilityTags)
	{
		if(Tag.MatchesTag(FGameplayTag::RequestGameplayTag(FName("InputTag"))))
		{
			return Tag;
		}
		
	}
	UE_LOG(LogAura, Error, TEXT("GetAbilityFromSpec failed [%hs]"), __FUNCTION__ );
	return FGameplayTag::EmptyTag;
}

void UAuraAbilitySystemComponent::OnRep_ActivateAbilities()
{
	Super::OnRep_ActivateAbilities();
	if(bStartupAbilitiesGiven)return;
	bStartupAbilitiesGiven = true;
	AbilitiesGivenDelegate.Broadcast(this);
}

void UAuraAbilitySystemComponent::ClientEffectApplied_Implementation(UAbilitySystemComponent* AbilitySystemComponent,
                                                                     const FGameplayEffectSpec& Spec, FActiveGameplayEffectHandle Handle) const
{
	FGameplayTagContainer TagContainer;
	Spec.GetAllAssetTags(TagContainer);
	EffectAssetTags.Broadcast(TagContainer);
}
