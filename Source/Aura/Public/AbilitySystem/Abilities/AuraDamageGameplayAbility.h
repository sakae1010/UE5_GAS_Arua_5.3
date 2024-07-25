// sakae's gas project

#pragma once

#include "CoreMinimal.h"
#include "AuraGameAbility.h"
#include "Interaction/CombatInterface.h"
#include "AuraDamageGameplayAbility.generated.h"

struct FDamageEffectParams;
/**
 * 
 */
UCLASS()
class AURA_API UAuraDamageGameplayAbility : public UAuraGameAbility
{
	GENERATED_BODY()

public:
	UFUNCTION( BlueprintCallable )
	void CauseDamage(AActor* TargetActor);

	FDamageEffectParams MakeDamageEffectParams(AActor* TargetActor = nullptr) const;
protected:
	UPROPERTY( EditDefaultsOnly , BlueprintReadOnly )
	TSubclassOf<UGameplayEffect> DamageEffectClass;

	UPROPERTY( EditDefaultsOnly , Category="Combat" )
	FGameplayTag DamageType;
	UPROPERTY( EditDefaultsOnly , Category="Combat" )
	FScalableFloat Damage;

	UPROPERTY( EditDefaultsOnly , Category="Combat" )
	float DebuffChance = 20.f;	//觸發機率
	UPROPERTY( EditDefaultsOnly , Category="Combat" )
	float DebuffDamage = 5.f;
	UPROPERTY( EditDefaultsOnly , Category="Combat" )
	float DebuffFrequency = 1.f;//每秒
	UPROPERTY( EditDefaultsOnly , Category="Combat" )
	float DebuffDuration = 5.f;	//持續時間

	UPROPERTY( EditDefaultsOnly , Category="Combat" )
	float DeathImpulseMagnitude = 60.f; //死亡時的衝量
	

	UFUNCTION( BlueprintPure )
	FTaggedMontage GetRandomAttackMontageForArray(const TArray<FTaggedMontage>& TaggedMontages);
};
