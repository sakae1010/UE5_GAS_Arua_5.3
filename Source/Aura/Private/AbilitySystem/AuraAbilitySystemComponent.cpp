// sakae's gas project


#include "AbilitySystem/AuraAbilitySystemComponent.h"

void UAuraAbilitySystemComponent::AbilityActorInfoInit()
{
	OnGameplayEffectAppliedDelegateToSelf.AddUObject(this, &UAuraAbilitySystemComponent::OnGameplayEffectAppliedCallback);
}

void UAuraAbilitySystemComponent::OnGameplayEffectAppliedCallback(UAbilitySystemComponent* AbilitySystemComponent,
                                                                  const FGameplayEffectSpec& Spec, FActiveGameplayEffectHandle Handle)
{
	GEngine->AddOnScreenDebugMessage(1,8.f,FColor::Red,TEXT("OnGameplayEffectAppliedCallback"));
}
