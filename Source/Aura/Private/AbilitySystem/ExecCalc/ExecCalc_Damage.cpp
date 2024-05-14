// sakae's gas project


#include "AbilitySystem/ExecCalc/ExecCalc_Damage.h"

#include <AuraAbilityTypes.h>

#include "AbilitySystem/AuraAttributeSet.h"
#include "AbilitySystemComponent.h"
#include "AuraGameplayTag.h"
#include "AbilitySystem/AuraAbilitySystemLibrary.h"
#include "Interaction/CombatInterface.h"

//只在CPP使用 不需要宣告 UStruct
struct AuraDamageStatics	
{
	DECLARE_ATTRIBUTE_CAPTUREDEF(Armor);
	DECLARE_ATTRIBUTE_CAPTUREDEF(ArmorPenetration);
	DECLARE_ATTRIBUTE_CAPTUREDEF(BlockChance);
	DECLARE_ATTRIBUTE_CAPTUREDEF(CriticalHitChance);
	DECLARE_ATTRIBUTE_CAPTUREDEF(CriticalHitDamage);
	DECLARE_ATTRIBUTE_CAPTUREDEF(CriticalHitResistance);
	
	
	AuraDamageStatics()
	{
		DEFINE_ATTRIBUTE_CAPTUREDEF(UAuraAttributeSet,Armor,Target ,false);
		DEFINE_ATTRIBUTE_CAPTUREDEF(UAuraAttributeSet,BlockChance,Target ,false);
		DEFINE_ATTRIBUTE_CAPTUREDEF(UAuraAttributeSet,ArmorPenetration,Source ,false);
		DEFINE_ATTRIBUTE_CAPTUREDEF(UAuraAttributeSet,CriticalHitChance,Source ,false);
		DEFINE_ATTRIBUTE_CAPTUREDEF(UAuraAttributeSet,CriticalHitDamage,Source ,false);
		DEFINE_ATTRIBUTE_CAPTUREDEF(UAuraAttributeSet,CriticalHitResistance,Target ,false);
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
	RelevantAttributesToCapture.Add(DamageStatics().ArmorPenetrationDef);
	RelevantAttributesToCapture.Add(DamageStatics().CriticalHitChanceDef);
	RelevantAttributesToCapture.Add(DamageStatics().CriticalHitDamageDef);
	RelevantAttributesToCapture.Add(DamageStatics().CriticalHitResistanceDef);
}

void UExecCalc_Damage::Execute_Implementation(const FGameplayEffectCustomExecutionParameters& ExecutionParams,	FGameplayEffectCustomExecutionOutput& OutExecutionOutput) const
{
	const UAbilitySystemComponent* SourceASC = ExecutionParams.GetSourceAbilitySystemComponent();
	const UAbilitySystemComponent* TargetASC = ExecutionParams.GetTargetAbilitySystemComponent();

	AActor* SourceActor = SourceASC ? SourceASC->GetAvatarActor() : nullptr;
	AActor* TargetActor = TargetASC ? TargetASC->GetAvatarActor() : nullptr;
	ICombatInterface* SourceCombatInterface = Cast<ICombatInterface>(SourceActor);
	ICombatInterface* TargetCombatInterface = Cast<ICombatInterface>(TargetActor);

	const FGameplayEffectSpec& Spec = ExecutionParams.GetOwningSpec();
	const FGameplayTagContainer* SourceTags = Spec.CapturedSourceTags.GetAggregatedTags();
	const FGameplayTagContainer* TargetTags = Spec.CapturedTargetTags.GetAggregatedTags();
	FAggregatorEvaluateParameters EvaluationParameters;
	EvaluationParameters.SourceTags = SourceTags;
	EvaluationParameters.TargetTags = TargetTags;
		
	// 取得 傷害計算的數值
	float Damage = 0.f ;// Spec.GetSetByCallerMagnitude(FAuraGameplayTags::Get().Damage);
	for (FGameplayTag DamageTypeTag : FAuraGameplayTags::Get().DamageTypes)
	{
		const float DamageTypeValue = Spec.GetSetByCallerMagnitude(DamageTypeTag);
		Damage += DamageTypeValue;
	}

	
	// 取得 目標 抵抗計算的數值
	
	float TargetBlockChance = 0.f;
	ExecutionParams.AttemptCalculateCapturedAttributeMagnitude(DamageStatics().BlockChanceDef, EvaluationParameters,TargetBlockChance);
	TargetBlockChance = FMath::Max<float>(0.f, TargetBlockChance);
	
	float SourceArmorPenetration = 0.f;
	ExecutionParams.AttemptCalculateCapturedAttributeMagnitude(DamageStatics().ArmorPenetrationDef, EvaluationParameters,SourceArmorPenetration);
	SourceArmorPenetration = FMath::Max<float>(0.f, SourceArmorPenetration);
	
	float TargetArmor = 0.f;
	ExecutionParams.AttemptCalculateCapturedAttributeMagnitude(DamageStatics().ArmorDef, EvaluationParameters,TargetArmor);
	TargetArmor = FMath::Max<float>(0.f, TargetArmor);

	UCharacterClassInfo* CharacterClassInfo = UAuraAbilitySystemLibrary::GetCharacterClassInfo(SourceActor);

	FGameplayEffectContextHandle EffectContextHandle = Spec.GetContext();

	
	const bool bBlocked = FMath::RandRange(1,100) <= TargetBlockChance;
	UAuraAbilitySystemLibrary::SetBlockedHit( EffectContextHandle, bBlocked);
	
	// 如果被格擋 則傷害減半
	Damage = bBlocked ? Damage * 0.5f : Damage;
	// 取得 來源穿甲係數 
	FRealCurve* ArmorPenetrationCurve = CharacterClassInfo->DamageCalculationCoefficients->FindCurve(FName("ArmorPenetration"), FString());

	const float ArmorPenetrationCoefficient = ArmorPenetrationCurve->Eval(SourceCombatInterface->GetPlayerLevel());
	// 有效穿透護甲計算
	const float EffectiveArmor = TargetArmor * (100.f - SourceArmorPenetration * ArmorPenetrationCoefficient) * 0.01f;
	// 取得 有效護甲係數
	const FRealCurve* EffectiveArmorCurve = CharacterClassInfo->DamageCalculationCoefficients->FindCurve(FName("EffectiveArmor"), FString());
	const float ArmorCurveCoefficient = EffectiveArmorCurve->Eval(TargetCombatInterface->GetPlayerLevel());
	// 計算勿穿最終傷害
	Damage *= (100.f - EffectiveArmor * ArmorCurveCoefficient) * 0.01f;



	//攻擊者 爆擊機率
	float SourceCriticalHitChance = 0.f;
	ExecutionParams.AttemptCalculateCapturedAttributeMagnitude(DamageStatics().CriticalHitChanceDef, EvaluationParameters,SourceCriticalHitChance);
	SourceCriticalHitChance = FMath::Max<float>(0.f, SourceCriticalHitChance);
	//受傷者 爆擊抵抗
	float TargetCriticalHitResistance = 0.f;
	ExecutionParams.AttemptCalculateCapturedAttributeMagnitude(DamageStatics().CriticalHitResistanceDef, EvaluationParameters, TargetCriticalHitResistance);
	TargetCriticalHitResistance = FMath::Max<float>(0.f,  TargetCriticalHitResistance);
	//攻擊者 爆擊傷害
	float SourceCriticalHitDamage = 0.f;
	ExecutionParams.AttemptCalculateCapturedAttributeMagnitude(DamageStatics().CriticalHitDamageDef, EvaluationParameters,SourceCriticalHitDamage);
	SourceCriticalHitDamage = FMath::Max<float>(0.f, SourceCriticalHitDamage);
	
	const FRealCurve* EffectiveCriticalHit = CharacterClassInfo->DamageCalculationCoefficients->FindCurve(FName("CriticalHitResistance"), FString());
	const float CriticalHitResistanceCoefficient = EffectiveCriticalHit->Eval(SourceCombatInterface->GetPlayerLevel());
	
	const bool bCriticalHit = FMath::RandRange(1,100) <= SourceCriticalHitChance - TargetCriticalHitResistance * CriticalHitResistanceCoefficient ; 
	Damage = bCriticalHit? Damage * 2.0f + SourceCriticalHitDamage : Damage;
	UAuraAbilitySystemLibrary::SetCriticalHit( EffectContextHandle, bCriticalHit);
	
	const FGameplayModifierEvaluatedData EvaluatedData(UAuraAttributeSet::GetIncomingDamageAttribute(), EGameplayModOp::Additive, Damage);
	OutExecutionOutput.AddOutputModifier(EvaluatedData);
}
