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

	UFUNCTION( BlueprintPure )
	FDamageEffectParams MakeDamageEffectParams(AActor* TargetActor = nullptr ,FVector InRadialDamageOrigin = FVector::ZeroVector) const;
	
	UFUNCTION( BlueprintPure )
	float GetDamageAtLevel() const;
protected:
	UPROPERTY( EditDefaultsOnly , BlueprintReadOnly )
	TSubclassOf<UGameplayEffect> DamageEffectClass;

	UPROPERTY( EditDefaultsOnly , Category="Damage" )
	FGameplayTag DamageType;
	UPROPERTY( EditDefaultsOnly , Category="Damage" )
	FScalableFloat Damage;

	UPROPERTY( EditDefaultsOnly , Category="Damage" )
	float DebuffChance = 20.f;	//觸發機率


	
	UPROPERTY( EditDefaultsOnly , Category="Damage" )
	float DebuffDamage = 5.f;
	UPROPERTY( EditDefaultsOnly , Category="Damage" )
	float DebuffFrequency = 1.f;//每秒
	UPROPERTY( EditDefaultsOnly , Category="Damage" )
	float DebuffDuration = 5.f;	//持續時間

	UPROPERTY( EditDefaultsOnly , Category="Damage" )
	float DeathImpulseMagnitude = 1000.f; //死亡時的衝量

	UPROPERTY( EditDefaultsOnly , Category="Damage" )
	float KnokbackForceMagnitude = 1000.f; //擊退力
	
	UPROPERTY( EditDefaultsOnly , Category="Damage" )
	float KnokbackChance = 0.f;

	UPROPERTY( EditDefaultsOnly , Category="Damage.Radial" )
	bool bIsRadialDamage = false;
	// 內圈傷害半徑
	UPROPERTY( EditDefaultsOnly  ,BlueprintReadOnly , Category="Damage.Radial" )
	float RadialDamageInnerRadius = 0.f;
	// 外圈傷害半徑
	UPROPERTY( EditDefaultsOnly , BlueprintReadOnly ,Category="Damage.Radial" )
	float RadialDamageOuterRadius = 0.f;
	
	UFUNCTION( BlueprintPure )
	FTaggedMontage GetRandomAttackMontageForArray(const TArray<FTaggedMontage>& TaggedMontages);

	
};
