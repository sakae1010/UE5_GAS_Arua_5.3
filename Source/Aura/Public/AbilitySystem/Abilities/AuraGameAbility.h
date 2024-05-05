// sakae's gas project

#pragma once

#include "CoreMinimal.h"
#include "Abilities/GameplayAbility.h"
#include "AuraGameAbility.generated.h"

/**
 * 
 */
UCLASS()
class AURA_API UAuraGameAbility : public UGameplayAbility
{
	GENERATED_BODY()
public:
	UPROPERTY(EditDefaultsOnly,Category="Input")
	FGameplayTag StartInputTag;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly ,Category="Damage")
	FScalableFloat Damage;
};
