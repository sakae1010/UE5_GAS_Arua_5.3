// sakae's gas project


#include "AbilitySystem/Abilities/AuraDamageGameplayAbility.h"

#include "AbilitySystemBlueprintLibrary.h"
#include "AbilitySystem/AuraAbilitySystemLibrary.h"

void UAuraDamageGameplayAbility::CauseDamage(AActor* TargetActor)
{
	FGameplayEffectSpecHandle DamageSpecHandle = MakeOutgoingGameplayEffectSpec(DamageEffectClass, 1.f);
	const float ScaledDamage = Damage.GetValueAtLevel(GetAbilityLevel());
	UAbilitySystemBlueprintLibrary::AssignTagSetByCallerMagnitude(DamageSpecHandle, DamageType, ScaledDamage);
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
