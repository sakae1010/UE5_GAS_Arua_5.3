﻿#pragma once

#include "GameplayEffectTypes.h"
#include "AuraAbilityTypes.generated.h"

class UGameplayEffect;

USTRUCT(BlueprintType)
struct FDamageEffectParams
{
	GENERATED_BODY()

	FDamageEffectParams(){}

	UPROPERTY(BlueprintReadWrite)
	TObjectPtr<UObject> WorldContextObject = nullptr;

	UPROPERTY(BlueprintReadWrite)
	TSubclassOf<UGameplayEffect> DamageEffectClass = nullptr;
	
	UPROPERTY(BlueprintReadWrite)
	TObjectPtr<UAbilitySystemComponent> SourceAbilitySystemComponent = nullptr;
	
	UPROPERTY(BlueprintReadWrite)
	TObjectPtr<UAbilitySystemComponent> TargetAbilitySystemComponent = nullptr;
	
	UPROPERTY(BlueprintReadWrite)
	float BaseDamage = 0.f;

	UPROPERTY(BlueprintReadWrite)
	float AbilityLevel = 1.f;

	UPROPERTY(BlueprintReadWrite)
	FGameplayTag DamageType  = FGameplayTag::EmptyTag;

	UPROPERTY(BlueprintReadWrite)
	float DebuffChance = 0.f;	//觸發機率

	UPROPERTY(BlueprintReadWrite)
	float DebuffDamage = 0.f; //傷害

	UPROPERTY(BlueprintReadWrite)
	float DebuffDuration = 0.f;	//持續時間

	UPROPERTY(BlueprintReadWrite)
	float DebuffFrequency = 0.f;//每秒

	UPROPERTY(BlueprintReadWrite)
	float DeathImpulseMagnitude = 0.f; //死亡時的衝量

	UPROPERTY(BlueprintReadWrite)
	FVector DeathImpulseVector = FVector::ZeroVector;

	UPROPERTY(BlueprintReadWrite)
	float KnockbackChance = 0.f;	//擊退機率

	UPROPERTY(BlueprintReadWrite)
	float KnockbackForceMagnitude = 0.f; //擊退力
	
	UPROPERTY(BlueprintReadWrite)
	FVector KnockbackForce = FVector::ZeroVector;

	// 使用範圍傷害
	UPROPERTY(BlueprintReadWrite)
	bool bIsRadialDamage = false;

	// 範圍傷害半徑
	UPROPERTY(BlueprintReadWrite)
	float RadialDamageRadius = 0.f;
	// 內圈傷害半徑
	UPROPERTY(BlueprintReadWrite)
	float RadialDamageInnerRadius = 0.f;
	// 外圈傷害半徑
	UPROPERTY(BlueprintReadWrite)
	float RadialDamageOuterRadius = 0.f;
	// 範圍傷害中心
	UPROPERTY(BlueprintReadWrite)
	FVector RadialDamageOrigin = FVector::ZeroVector;
};

USTRUCT(BlueprintType)
struct FAuraGameplayEffectContext : public FGameplayEffectContext
{
	GENERATED_BODY()
public:

	bool IsCriticalHit() const { return bIsCriticalHit; }
	
	bool IsBlockedHit() const { return bIsBlockedHit; }

	bool IsSuccessfulDebuff() const { return bSuccessfulDebuff; }

	float GetDebuffDamage() const { return DebuffDamage; }
	float GetDebuffDuration() const { return DebuffDuration; }
	float GetDebuffFrequency() const { return DebuffFrequency; }
	TSharedPtr<FGameplayTag> GetDamageType() const { return DamageType; }
	FVector GetDeathImpulse() const { return DeathImpulse; }
	FVector GetKnokbackForce() const { return KnokbackForce; }
	bool IsRadialDamage() const { return bIsRadialDamage; }
	float GetRadialDamageInnerRadius() const { return RadialDamageInnerRadius; }
	float GetRadialDamageOuterRadius() const { return RadialDamageOuterRadius; }
	FVector GetRadialDamageOrigin() const { return RadialDamageOrigin; }
	
	void SetCriticalHit(bool bInIsCriticalHit ) { bIsCriticalHit = bInIsCriticalHit; }
	void SetBlockedHit(bool bInIsBlockedHit );
	void SetSuccessfulDebuff(bool bInSuccessfulDebuff) { bSuccessfulDebuff = bInSuccessfulDebuff; }
	void SetDebuffDamage(float InDebuffDamage) { DebuffDamage = InDebuffDamage; }
	void SetDebuffDuration(float InDebuffDuration) { DebuffDuration = InDebuffDuration; }
	void SetDebuffFrequency(float InDebuffFrequency) { DebuffFrequency = InDebuffFrequency; }
	void SetDamageType(const TSharedPtr<FGameplayTag>& InDamageType) { DamageType = InDamageType; }
	void SetDeathImpulse(const FVector& InDeathImpulse) { DeathImpulse = InDeathImpulse; }
	void SetKnokbackForce(const FVector& InKnokbackForce) { KnokbackForce = InKnokbackForce; }
	// 範圍傷害
	void SetIsRadialDamage(const bool bInIsRadialDamage) { bIsRadialDamage = bInIsRadialDamage; }
	void SetRadialDamageInnerRadius(const float InRadialDamageInnerRadius) { RadialDamageInnerRadius = InRadialDamageInnerRadius; }
	void SetRadialDamageOuterRadius(const float InRadialDamageOuterRadius) { RadialDamageOuterRadius = InRadialDamageOuterRadius; }
	void SetRadialDamageOrigin(const FVector& InRadialDamageOrigin) { RadialDamageOrigin = InRadialDamageOrigin; }
	
	virtual UScriptStruct* GetScriptStruct() const override
	{
		return StaticStruct();
	}

	/** Creates a copy of this context, used to duplicate for later modifications */
	virtual FAuraGameplayEffectContext* Duplicate() const override
	{
		FAuraGameplayEffectContext* NewContext = new FAuraGameplayEffectContext();
		*NewContext = *this;
		if (GetHitResult())
		{
			// Does a deep copy of the hit result
			NewContext->AddHitResult(*GetHitResult(), true);
		}
		return NewContext;
	}
	
	virtual bool NetSerialize(FArchive& Ar, UPackageMap* Map, bool& bOutSuccess) override;
	
protected:
	UPROPERTY()
	bool bIsCriticalHit = false;
	
	UPROPERTY()
	bool bIsBlockedHit = false;

	UPROPERTY()
	bool bSuccessfulDebuff = false;
	
	UPROPERTY()
	float DebuffDamage = 0.f; //傷害

	UPROPERTY()
	float DebuffDuration = 0.f;	//持續時間
	
	UPROPERTY()
	float DebuffFrequency = 0.f;	//每秒

	TSharedPtr<FGameplayTag> DamageType;
	
	UPROPERTY()
	FVector DeathImpulse = FVector::ZeroVector;
	
	UPROPERTY()
	FVector KnokbackForce = FVector::ZeroVector;


	// 使用範圍傷害
	UPROPERTY()
	bool bIsRadialDamage = false;
	
	// 內圈傷害半徑
	UPROPERTY()
	float RadialDamageInnerRadius = 0.f;
	// 外圈傷害半徑
	UPROPERTY()
	float RadialDamageOuterRadius = 0.f;
	// 範圍傷害中心
	UPROPERTY()
	FVector RadialDamageOrigin = FVector::ZeroVector;
};

template<>
struct TStructOpsTypeTraits<FAuraGameplayEffectContext> : public TStructOpsTypeTraitsBase2<FAuraGameplayEffectContext>
{
	enum
	{
		WithNetSerializer = true,
		WithCopy = true,
	};
};
