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

FDamageEffectParams UAuraDamageGameplayAbility::MakeDamageEffectParams(AActor* TargetActor,
	FVector InRadialDamageOrigin, bool bOverrideKnockbackDirection, FVector KnockbackDirectionOverride,
	bool bOverrideDeathImpulse, FVector DeathImpulseDirection, bool bOverridePitch, float PitchOverride) const
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
	DamageEffectParams.KnockbackForceMagnitude = KnockbackForceMagnitude;
	DamageEffectParams.KnockbackChance = KnockbackChance;
	
	if(IsValid( TargetActor ))
	{
		FRotator Rotation = ( TargetActor->GetActorLocation() - GetAvatarActorFromActorInfo()->GetActorLocation() ).Rotation();
		if (bOverridePitch)
		{
			Rotation.Pitch = PitchOverride;
		}

		const FVector ToTarget = Rotation.Vector();
		if(!bOverrideKnockbackDirection)
		{
			DamageEffectParams.KnockbackForce = ToTarget * KnockbackForceMagnitude;
		}
		if(!bOverrideDeathImpulse)
		{
			DamageEffectParams.DeathImpulseVector = ToTarget * DeathImpulseMagnitude;
		}
	}

	if(bOverrideKnockbackDirection)
	{
		KnockbackDirectionOverride.Normalize();
		DamageEffectParams.KnockbackForce = KnockbackDirectionOverride * KnockbackForceMagnitude;
		if (bOverridePitch)
		{
			FRotator KnockbackRotation = KnockbackDirectionOverride.Rotation();
			KnockbackRotation.Pitch = PitchOverride;
			DamageEffectParams.KnockbackForce = KnockbackRotation.Vector() * KnockbackForceMagnitude;
		}
	}
	if(bOverrideDeathImpulse)
	{
		DeathImpulseDirection.Normalize();
		DamageEffectParams.DeathImpulseVector = DeathImpulseDirection * DeathImpulseMagnitude;
		if (bOverridePitch)
		{
			FRotator DeathImpulseRotation = DeathImpulseDirection.Rotation();
			DeathImpulseRotation.Pitch = PitchOverride;
			DamageEffectParams.DeathImpulseVector = DeathImpulseRotation.Vector() * DeathImpulseMagnitude;
		}
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
