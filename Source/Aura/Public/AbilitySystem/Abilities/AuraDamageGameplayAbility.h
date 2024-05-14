// sakae's gas project

#pragma once

#include "CoreMinimal.h"
#include "AuraGameAbility.h"
#include "AuraDamageGameplayAbility.generated.h"

/**
 * 
 */
UCLASS()
class AURA_API UAuraDamageGameplayAbility : public UAuraGameAbility
{
	GENERATED_BODY()

protected:
	UPROPERTY(EditDefaultsOnly , BlueprintReadOnly)
	TSubclassOf<UGameplayEffect> DamageEffectClass;
	
	UPROPERTY(EditDefaultsOnly ,Category="Combat")
	TMap<FGameplayTag , FScalableFloat> DamageTypes;
};
