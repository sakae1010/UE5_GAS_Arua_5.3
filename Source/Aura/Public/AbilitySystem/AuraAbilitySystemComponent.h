// sakae's gas project

#pragma once

#include "CoreMinimal.h"
#include "AbilitySystemComponent.h"
#include "AuraAbilitySystemComponent.generated.h"
DECLARE_MULTICAST_DELEGATE_OneParam(FEffectAssetTags, const FGameplayTagContainer& /*AssetTags*/);

/**
 * 
 */
UCLASS()
class AURA_API UAuraAbilitySystemComponent : public UAbilitySystemComponent
{
	GENERATED_BODY()
public:
	void AbilityActorInfoInit();

	FEffectAssetTags EffectAssetTags;
	void AddCharacterAbilities(TArray<TSubclassOf<UGameplayAbility>>& InAbilities);

	void AbilityInputHeld(const FGameplayTag& InputTag);
	void AbilityInputReleased(const FGameplayTag& InputTag);
protected:
	
	UFUNCTION(Client,Reliable)
	void ClientEffectApplied(UAbilitySystemComponent* AbilitySystemComponent ,  const FGameplayEffectSpec& Spec, FActiveGameplayEffectHandle Handle) const;
};
