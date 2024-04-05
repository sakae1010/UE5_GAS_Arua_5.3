// sakae's gas project

#pragma once

#include "CoreMinimal.h"
#include "AbilitySystemComponent.h"
#include "AuraAbilitySystemComponent.generated.h"

/**
 * 
 */
UCLASS()
class AURA_API UAuraAbilitySystemComponent : public UAbilitySystemComponent
{
	GENERATED_BODY()
public:
	void AbilityActorInfoInit();
protected:
	void OnGameplayEffectAppliedCallback(UAbilitySystemComponent* AbilitySystemComponent ,  const FGameplayEffectSpec& Spec, FActiveGameplayEffectHandle Handle);
};
