// sakae's gas project

#pragma once

#include "CoreMinimal.h"
#include "AuraGameAbility.h"
#include "Interaction/CombatInterface.h"
#include "AuraDamageGameplayAbility.generated.h"

/**
 * 
 */
UCLASS()
class AURA_API UAuraDamageGameplayAbility : public UAuraGameAbility
{
	GENERATED_BODY()
public:
	UFUNCTION(BlueprintCallable)
	void CauseDamage(AActor* TargetActor);
protected:
	UPROPERTY(EditDefaultsOnly , BlueprintReadOnly)
	TSubclassOf<UGameplayEffect> DamageEffectClass;
	
	UPROPERTY(EditDefaultsOnly ,Category="Combat")
	TMap<FGameplayTag , FScalableFloat> DamageTypes;

	UFUNCTION(BlueprintPure)
	FTaggedMontage GetRandomAttackMontageForArray(const TArray<FTaggedMontage>& TaggedMontages );

	float GetDamageByDamageType(int InLevel , const FGameplayTag& Tag) const;
};
