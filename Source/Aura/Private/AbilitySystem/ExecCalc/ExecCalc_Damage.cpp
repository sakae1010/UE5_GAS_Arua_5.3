﻿// sakae's gas project


#include "AbilitySystem/ExecCalc/ExecCalc_Damage.h"
#include "AbilitySystem/AuraAttributeSet.h"
#include "AbilitySystemComponent.h"
#include "AuraGameplayTag.h"

//只在CPP使用 不需要宣告 UStruct
struct AuraDamageStatics	
{
	DECLARE_ATTRIBUTE_CAPTUREDEF(Armor);
	DECLARE_ATTRIBUTE_CAPTUREDEF(BlockChance);
	AuraDamageStatics()
	{
		DEFINE_ATTRIBUTE_CAPTUREDEF(UAuraAttributeSet,Armor,Target ,false);
		DEFINE_ATTRIBUTE_CAPTUREDEF(UAuraAttributeSet,BlockChance,Target ,false);
	}
};

static const AuraDamageStatics& DamageStatics()
{
	static  AuraDamageStatics DStatics;
	return DStatics;
	
}

UExecCalc_Damage::UExecCalc_Damage()
{
	RelevantAttributesToCapture.Add(DamageStatics().ArmorDef);
	RelevantAttributesToCapture.Add(DamageStatics().BlockChanceDef);
}

void UExecCalc_Damage::Execute_Implementation(const FGameplayEffectCustomExecutionParameters& ExecutionParams,	FGameplayEffectCustomExecutionOutput& OutExecutionOutput) const
{
	const UAbilitySystemComponent* SourceASC = ExecutionParams.GetSourceAbilitySystemComponent();
	const UAbilitySystemComponent* TargetASC = ExecutionParams.GetTargetAbilitySystemComponent();

	const AActor* SourceActor = SourceASC ? SourceASC->GetAvatarActor() : nullptr;
	const AActor* TargetActor = TargetASC ? TargetASC->GetAvatarActor() : nullptr;

	const FGameplayEffectSpec& Spec = ExecutionParams.GetOwningSpec();
	const FGameplayTagContainer* SourceTags = Spec.CapturedSourceTags.GetAggregatedTags();
	const FGameplayTagContainer* TargetTags = Spec.CapturedTargetTags.GetAggregatedTags();
	FAggregatorEvaluateParameters EvaluationParameters;
	EvaluationParameters.SourceTags = SourceTags;
	EvaluationParameters.TargetTags = TargetTags;

	// 取得 傷害計算的數值
	float Damage = Spec.GetSetByCallerMagnitude(FAuraGameplayTags::Get().Damage);
	// 取得 目標 抵抗計算的數值
	float TargetBlockChance = 0.f;
	ExecutionParams.AttemptCalculateCapturedAttributeMagnitude(DamageStatics().BlockChanceDef, EvaluationParameters,TargetBlockChance);
	TargetBlockChance = FMath::Max<float>(0.f, TargetBlockChance);
	const bool bBlocked = FMath::RandRange(1,100) <= TargetBlockChance;
	// 如果被格擋 則傷害減半
	Damage = bBlocked ? Damage * 0.5f : Damage;
	// 取得 目標 護甲计算的数值 
	
	// float Armor = 0.f;
	// ExecutionParams.AttemptCalculateCapturedAttributeMagnitude(DamageStatics().ArmorDef, EvaluationParameters,Armor);
	// Armor = FMath::Max<float>(0.f, Armor);
	const FGameplayModifierEvaluatedData EvaluatedData(UAuraAttributeSet::GetIncomingDamageAttribute(), EGameplayModOp::Additive, Damage);
	OutExecutionOutput.AddOutputModifier(EvaluatedData);
}
