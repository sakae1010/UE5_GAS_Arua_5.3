// sakae's gas project


#include "AbilitySystem/Abilities/AuraDamageGameplayAbility.h"

#include "AbilitySystemBlueprintLibrary.h"
#include "AuraAbilityTypes.h"
#include "AbilitySystem/AuraAbilitySystemLibrary.h"

void UAuraDamageGameplayAbility::CauseDamage(AActor* TargetActor)
{
	FGameplayEffectSpecHandle DamageSpecHandle = MakeOutgoingGameplayEffectSpec(DamageEffectClass, 1.f);
	const float ScaledDamage = Damage.GetValueAtLevel(GetAbilityLevel());
	UAbilitySystemBlueprintLibrary::AssignTagSetByCallerMagnitude(DamageSpecHandle, DamageType, ScaledDamage);
	GetAbilitySystemComponentFromActorInfo()->ApplyGameplayEffectSpecToTarget(*DamageSpecHandle.Data.Get(), UAbilitySystemBlueprintLibrary::GetAbilitySystemComponent(TargetActor));
	
}

FDamageEffectParams UAuraDamageGameplayAbility::MakeDamageEffectParams(AActor* TargetActor , FVector InRadialDamageOrigin) const
{
	FDamageEffectParams DamageEffectParams;
	DamageEffectParams.WorldContextObject = GetAvatarActorFromActorInfo();
	DamageEffectParams.DamageEffectClass = DamageEffectClass;
	DamageEffectParams.SourceAbilitySystemComponent = GetAbilitySystemComponentFromActorInfo();
	DamageEffectParams.TargetAbilitySystemComponent = UAbilitySystemBlueprintLibrary::GetAbilitySystemComponent(TargetActor);
	DamageEffectParams.BaseDamage = Damage.GetValueAtLevel(GetAbilityLevel());
	DamageEffectParams.AbilityLevel = GetAbilityLevel();
	DamageEffectParams.DamageType = DamageType;
	DamageEffectParams.DebuffChance = DebuffChance;
	DamageEffectParams.DebuffDamage = DebuffDamage;
	DamageEffectParams.DebuffDuration = DebuffDuration;
	DamageEffectParams.DebuffFrequency = DebuffFrequency;
	DamageEffectParams.DeathImpulseMagnitude = DeathImpulseMagnitude;
	DamageEffectParams.KnokbackForceMagnitude = KnokbackForceMagnitude;
	DamageEffectParams.KnokbackChance = KnokbackChance;
	if(IsValid( TargetActor ))
	{
		FRotator Rotation = ( TargetActor->GetActorLocation() - GetAvatarActorFromActorInfo()->GetActorLocation() ).Rotation();
		Rotation.Pitch = 45.f;
		const FVector ToTarget = Rotation.Vector();
		DamageEffectParams.DeathImpulseVector = ToTarget * DeathImpulseMagnitude;
		DamageEffectParams.KnokbackForce = ToTarget * KnokbackForceMagnitude;
	}
	if(bIsRadialDamage)
	{
		DamageEffectParams.bIsRadialDamage = bIsRadialDamage;
		DamageEffectParams.RadialDamageOrigin = InRadialDamageOrigin;
		DamageEffectParams.RadialDamageInnerRadius = RadialDamageInnerRadius;
		DamageEffectParams.RadialDamageOuterRadius = RadialDamageOuterRadius;
	}
	return DamageEffectParams;
}

float UAuraDamageGameplayAbility::GetDamageAtLevel() const
{
	return  Damage.GetValueAtLevel(GetAbilityLevel());
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
