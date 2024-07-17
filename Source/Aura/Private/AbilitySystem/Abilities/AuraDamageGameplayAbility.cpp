// sakae's gas project


#include "AbilitySystem/Abilities/AuraDamageGameplayAbility.h"

#include "AbilitySystemBlueprintLibrary.h"
#include "AbilitySystem/AuraAbilitySystemLibrary.h"

void UAuraDamageGameplayAbility::CauseDamage(AActor* TargetActor)
{
	FGameplayEffectSpecHandle DamageSpecHandle = MakeOutgoingGameplayEffectSpec(DamageEffectClass, 1.f);
	for (TTuple<FGameplayTag, FScalableFloat> Pair : DamageTypes)
	{
		const float ScaledDamage = Pair.Value.GetValueAtLevel(GetAbilityLevel());
		UAbilitySystemBlueprintLibrary::AssignTagSetByCallerMagnitude(DamageSpecHandle, Pair.Key, ScaledDamage);
	}
	GetAbilitySystemComponentFromActorInfo()->ApplyGameplayEffectSpecToTarget(*DamageSpecHandle.Data.Get(), UAbilitySystemBlueprintLibrary::GetAbilitySystemComponent(TargetActor));
	
}

FTaggedMontage UAuraDamageGameplayAbility::GetRandomAttackMontageForArray(const TArray<FTaggedMontage>& TaggedMontages)
{
	if(TaggedMontages.Num()>0)
	{
		const int32 SelectIndex = TaggedMontages.Num()>1?FMath::RandRange(0,TaggedMontages.Num()-1):0;
		return TaggedMontages[SelectIndex];
	}
	return FTaggedMontage();
}

float UAuraDamageGameplayAbility::GetDamageByDamageType(int InLevel, const FGameplayTag& Tag) const
{
	checkf(DamageTypes.Contains(Tag), TEXT("GameplayAbilit [%s] does not contain DamageType [%s]"), *GetNameSafe(this), *Tag.ToString());
	return DamageTypes[Tag].GetValueAtLevel(InLevel);
}
