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
	DECLARE_ATTRIBUTE_CAPTUREDEF( Armor );
	DECLARE_ATTRIBUTE_CAPTUREDEF( ArmorPenetration );
	DECLARE_ATTRIBUTE_CAPTUREDEF( BlockChance );
	DECLARE_ATTRIBUTE_CAPTUREDEF( CriticalHitChance );
	DECLARE_ATTRIBUTE_CAPTUREDEF( CriticalHitDamage );
	DECLARE_ATTRIBUTE_CAPTUREDEF( CriticalHitResistance );

	DECLARE_ATTRIBUTE_CAPTUREDEF( FireResistance );
	DECLARE_ATTRIBUTE_CAPTUREDEF( LightningResistance );
	DECLARE_ATTRIBUTE_CAPTUREDEF( ArcaneResistance );
	DECLARE_ATTRIBUTE_CAPTUREDEF( PhysicalResistance );


	AuraDamageStatics()
	{
		DEFINE_ATTRIBUTE_CAPTUREDEF( UAuraAttributeSet , Armor , Target , false );
		DEFINE_ATTRIBUTE_CAPTUREDEF( UAuraAttributeSet , BlockChance , Target , false );
		DEFINE_ATTRIBUTE_CAPTUREDEF( UAuraAttributeSet , ArmorPenetration , Source , false );
		DEFINE_ATTRIBUTE_CAPTUREDEF( UAuraAttributeSet , CriticalHitChance , Source , false );
		DEFINE_ATTRIBUTE_CAPTUREDEF( UAuraAttributeSet , CriticalHitDamage , Source , false );
		DEFINE_ATTRIBUTE_CAPTUREDEF( UAuraAttributeSet , CriticalHitResistance , Target , false );


		DEFINE_ATTRIBUTE_CAPTUREDEF( UAuraAttributeSet , FireResistance , Target , false );
		DEFINE_ATTRIBUTE_CAPTUREDEF( UAuraAttributeSet , LightningResistance , Target , false );
		DEFINE_ATTRIBUTE_CAPTUREDEF( UAuraAttributeSet , ArcaneResistance , Target , false );
		DEFINE_ATTRIBUTE_CAPTUREDEF( UAuraAttributeSet , PhysicalResistance , Target , false );
	}
};

static const AuraDamageStatics& DamageStatics()
{
	static AuraDamageStatics DStatics;
	return DStatics;
}

UExecCalc_Damage::UExecCalc_Damage()
{
	RelevantAttributesToCapture.Add( DamageStatics().ArmorDef );
	RelevantAttributesToCapture.Add( DamageStatics().BlockChanceDef );
	RelevantAttributesToCapture.Add( DamageStatics().ArmorPenetrationDef );
	RelevantAttributesToCapture.Add( DamageStatics().CriticalHitChanceDef );
	RelevantAttributesToCapture.Add( DamageStatics().CriticalHitDamageDef );
	RelevantAttributesToCapture.Add( DamageStatics().CriticalHitResistanceDef );

	RelevantAttributesToCapture.Add( DamageStatics().FireResistanceDef );
	RelevantAttributesToCapture.Add( DamageStatics().LightningResistanceDef );
	RelevantAttributesToCapture.Add( DamageStatics().ArcaneResistanceDef );
	RelevantAttributesToCapture.Add( DamageStatics().PhysicalResistanceDef );
}

void UExecCalc_Damage::DetermineDebuff(const FGameplayEffectCustomExecutionParameters& ExecutionParams,
                                       const FGameplayEffectSpec& Spec,
                                       FAggregatorEvaluateParameters EvaluationParameters,
                                       const TMap<FGameplayTag, FGameplayEffectAttributeCaptureDefinition>& InTagToDefs) const
{
	//Debuffs
	const FAuraGameplayTags& GameplayTags = FAuraGameplayTags::Get();

	for ( TTuple<FGameplayTag, FGameplayTag> Pair : GameplayTags.DamageTypesToDebuffs )
	{
		const FGameplayTag& DamageType = Pair.Key;
		const FGameplayTag& Debuff = Pair.Value;
		const float TypeDamage = Spec.GetSetByCallerMagnitude( DamageType , false , -1 );
		if ( TypeDamage <= -0.5f ) // .5 padding for float point im percision //翻譯: .5 浮點數精度填充
			continue;
		//Determine if there was successful debuff // 確定是否有成功的debuff
		const float DebuffChance = Spec.GetSetByCallerMagnitude( GameplayTags.Debuff_Chance , false , -1 );

		float TargetDebuffResistance = 0.f;
		const FGameplayTag& ResistanceTag = GameplayTags.DamageTypesToResistances [ DamageType ];
		/**
			 * 嘗試根據指定的參數計算捕獲屬性的量值。如果遊戲規範中沒有屬性的有效捕獲，則可能會失敗。
			 * 
			 * @param InCaptureDef    要嘗試計算其量值的屬性定義
			 * @param InEvalParams    用於評估該屬性的參數
			 * @param OutMagnitude    [OUT] 計算出的量值
			 * 
			 * @return 如果量值成功計算，返回 true；如果失敗，返回 false
			 */
		//嘗試計算捕獲的屬性量值
		ExecutionParams.AttemptCalculateCapturedAttributeMagnitude(
			InTagToDefs [ ResistanceTag ] , EvaluationParameters , TargetDebuffResistance );
		TargetDebuffResistance = FMath::Max<float>( TargetDebuffResistance , 0 );
		//如果目標抵抗率大於隨機值，則不會有debuff
		const float EffectiveDebuffChance = DebuffChance * ( 100 - TargetDebuffResistance ) * 0.01f;
		if ( FMath::RandRange( 1 , 100 ) <= EffectiveDebuffChance )
		{
			FGameplayEffectContextHandle ContextHandle = Spec.GetContext();
			UAuraAbilitySystemLibrary::SetSuccessfulDebuff( ContextHandle , true );
			//設置damagetype
			UAuraAbilitySystemLibrary::SetDamageType( ContextHandle , DamageType );
			
			const float DebuffDamage = Spec.GetSetByCallerMagnitude( GameplayTags.Debuff_Damage , false , -1 );
			const float DebuffDuration = Spec.GetSetByCallerMagnitude( GameplayTags.Debuff_Duration , false , -1 );
			const float DebuffFrequency = Spec.GetSetByCallerMagnitude( GameplayTags.Debuff_Frequency , false , -1 );
			
			//設置debuff傷害	
			UAuraAbilitySystemLibrary::SetDebuffDamage( ContextHandle , DebuffDamage );
			//設置debuff持續時間
			UAuraAbilitySystemLibrary::SetDebuffDuration( ContextHandle , DebuffDuration );
			//設置debuff頻率
			UAuraAbilitySystemLibrary::SetDebuffFrequency( ContextHandle , DebuffFrequency );
		}
	}
}

void UExecCalc_Damage::Execute_Implementation(const FGameplayEffectCustomExecutionParameters& ExecutionParams,FGameplayEffectCustomExecutionOutput& OutExecutionOutput) const
{
	TMap<FGameplayTag, FGameplayEffectAttributeCaptureDefinition> TagsToCaptureDefs;
	const FAuraGameplayTags AuraGameplayTags = FAuraGameplayTags::Get();

	TagsToCaptureDefs.Add( AuraGameplayTags.AuraAttribute_Secondary_Armor                 , DamageStatics().ArmorDef );
	TagsToCaptureDefs.Add( AuraGameplayTags.AuraAttribute_Secondary_Armor                 , DamageStatics().ArmorDef );
	TagsToCaptureDefs.Add( AuraGameplayTags.AuraAttribute_Secondary_BlockChance           , DamageStatics().BlockChanceDef );
	TagsToCaptureDefs.Add( AuraGameplayTags.AuraAttribute_Secondary_ArmorPenetration      , DamageStatics().ArmorPenetrationDef );
	TagsToCaptureDefs.Add( AuraGameplayTags.AuraAttribute_Secondary_CriticalHitChance     , DamageStatics().CriticalHitChanceDef );
	TagsToCaptureDefs.Add( AuraGameplayTags.AuraAttribute_Secondary_CriticalHitDamage     , DamageStatics().CriticalHitDamageDef );
	TagsToCaptureDefs.Add( AuraGameplayTags.AuraAttribute_Secondary_CriticalHitResistance , DamageStatics().CriticalHitResistanceDef );

	TagsToCaptureDefs.Add( AuraGameplayTags.Attributes_Resistance_Fire , DamageStatics().FireResistanceDef );
	TagsToCaptureDefs.Add( AuraGameplayTags.Attributes_Resistance_Lightning , DamageStatics().LightningResistanceDef );
	TagsToCaptureDefs.Add( AuraGameplayTags.Attributes_Resistance_Arcane , DamageStatics().ArcaneResistanceDef );
	TagsToCaptureDefs.Add( AuraGameplayTags.Attributes_Resistance_Physical , DamageStatics().PhysicalResistanceDef );


	const UAbilitySystemComponent* SourceASC = ExecutionParams.GetSourceAbilitySystemComponent();
	const UAbilitySystemComponent* TargetASC = ExecutionParams.GetTargetAbilitySystemComponent();

	AActor* SourceActor = SourceASC ? SourceASC->GetAvatarActor() : nullptr;
	AActor* TargetActor = TargetASC ? TargetASC->GetAvatarActor() : nullptr;

	int32 SourcePlayerLevel = 1;
	if ( SourceActor->Implements<UCombatInterface>() )
	{
		SourcePlayerLevel = ICombatInterface::Execute_GetPlayerLevel( SourceActor );
	}
	int32 TargetPlayerLevel = 1;
	if ( TargetActor->Implements<UCombatInterface>() )
	{
		TargetPlayerLevel = ICombatInterface::Execute_GetPlayerLevel( TargetActor );
	}

	const FGameplayEffectSpec& Spec = ExecutionParams.GetOwningSpec();
	const FGameplayTagContainer* SourceTags = Spec.CapturedSourceTags.GetAggregatedTags();
	const FGameplayTagContainer* TargetTags = Spec.CapturedTargetTags.GetAggregatedTags();
	FAggregatorEvaluateParameters EvaluationParameters;
	EvaluationParameters.SourceTags = SourceTags;
	EvaluationParameters.TargetTags = TargetTags;

	DetermineDebuff( ExecutionParams  , Spec , EvaluationParameters  , TagsToCaptureDefs);


	// 取得 傷害計算的數值
	float Damage = 0.f;// Spec.GetSetByCallerMagnitude(FAuraGameplayTags::Get().Damage);
	for ( const TTuple<FGameplayTag, FGameplayTag>& Pair : FAuraGameplayTags::Get().DamageTypesToResistances )
	{
		const FGameplayTag DamageType = Pair.Key;
		const FGameplayTag ResistanceTag = Pair.Value;
		float DamageTypeValue = Spec.GetSetByCallerMagnitude( DamageType , false );
		if ( DamageTypeValue == 0 ) continue;
		checkf( TagsToCaptureDefs.Contains(ResistanceTag) ,
		        TEXT("TagsToCaptureDefs ddoesn't contain Tag [%s] ExecCalc_Damages ") , *ResistanceTag.ToString() );

		FGameplayEffectAttributeCaptureDefinition CaptureDef = TagsToCaptureDefs [ ResistanceTag ];

		float ResistanceValue = 0.f;
		ExecutionParams.AttemptCalculateCapturedAttributeMagnitude( CaptureDef , EvaluationParameters ,
		                                                            ResistanceValue );
		ResistanceValue = FMath::Clamp<float>( ResistanceValue , 0.f , 100.0f );
		DamageTypeValue *= 1 - ResistanceValue / 100;

		Damage += DamageTypeValue;
	}


	// 取得 目標 抵抗計算的數值

	float TargetBlockChance = 0.f;
	ExecutionParams.AttemptCalculateCapturedAttributeMagnitude( DamageStatics().BlockChanceDef , EvaluationParameters ,
	                                                            TargetBlockChance );
	TargetBlockChance = FMath::Max<float>( 0.f , TargetBlockChance );

	float SourceArmorPenetration = 0.f;
	ExecutionParams.AttemptCalculateCapturedAttributeMagnitude( DamageStatics().ArmorPenetrationDef ,
	                                                            EvaluationParameters , SourceArmorPenetration );
	SourceArmorPenetration = FMath::Max<float>( 0.f , SourceArmorPenetration );

	float TargetArmor = 0.f;
	ExecutionParams.AttemptCalculateCapturedAttributeMagnitude( DamageStatics().ArmorDef , EvaluationParameters ,
	                                                            TargetArmor );
	TargetArmor = FMath::Max<float>( 0.f , TargetArmor );

	UCharacterClassInfo* CharacterClassInfo = UAuraAbilitySystemLibrary::GetCharacterClassInfo( SourceActor );

	FGameplayEffectContextHandle EffectContextHandle = Spec.GetContext();


	const bool bBlocked = FMath::RandRange( 1 , 100 ) <= TargetBlockChance;
	UAuraAbilitySystemLibrary::SetBlockedHit( EffectContextHandle , bBlocked );

	// 如果被格擋 則傷害減半
	Damage = bBlocked ? Damage * 0.5f : Damage;
	// 取得 來源穿甲係數 
	FRealCurve* ArmorPenetrationCurve = CharacterClassInfo->DamageCalculationCoefficients->FindCurve(
		FName( "ArmorPenetration" ) , FString() );

	const float ArmorPenetrationCoefficient = ArmorPenetrationCurve->Eval( SourcePlayerLevel );
	// 有效穿透護甲計算
	const float EffectiveArmor = TargetArmor * ( 100.f - SourceArmorPenetration * ArmorPenetrationCoefficient ) * 0.01f;
	// 取得 有效護甲係數
	const FRealCurve* EffectiveArmorCurve = CharacterClassInfo->DamageCalculationCoefficients->FindCurve(
		FName( "EffectiveArmor" ) , FString() );
	const float ArmorCurveCoefficient = EffectiveArmorCurve->Eval( TargetPlayerLevel );
	// 計算勿穿最終傷害
	Damage *= ( 100.f - EffectiveArmor * ArmorCurveCoefficient ) * 0.01f;


	//攻擊者 爆擊機率
	float SourceCriticalHitChance = 0.f;
	ExecutionParams.AttemptCalculateCapturedAttributeMagnitude( DamageStatics().CriticalHitChanceDef ,
	                                                            EvaluationParameters , SourceCriticalHitChance );
	SourceCriticalHitChance = FMath::Max<float>( 0.f , SourceCriticalHitChance );
	//受傷者 爆擊抵抗
	float TargetCriticalHitResistance = 0.f;
	ExecutionParams.AttemptCalculateCapturedAttributeMagnitude( DamageStatics().CriticalHitResistanceDef ,
	                                                            EvaluationParameters , TargetCriticalHitResistance );
	TargetCriticalHitResistance = FMath::Max<float>( 0.f , TargetCriticalHitResistance );
	//攻擊者 爆擊傷害
	float SourceCriticalHitDamage = 0.f;
	ExecutionParams.AttemptCalculateCapturedAttributeMagnitude( DamageStatics().CriticalHitDamageDef ,
	                                                            EvaluationParameters , SourceCriticalHitDamage );
	SourceCriticalHitDamage = FMath::Max<float>( 0.f , SourceCriticalHitDamage );

	const FRealCurve* EffectiveCriticalHit = CharacterClassInfo->DamageCalculationCoefficients->FindCurve(
		FName( "CriticalHitResistance" ) , FString() );
	const float CriticalHitResistanceCoefficient = EffectiveCriticalHit->Eval( SourcePlayerLevel );

	const bool bCriticalHit = FMath::RandRange( 1 , 100 ) <= SourceCriticalHitChance - TargetCriticalHitResistance *
		CriticalHitResistanceCoefficient;
	Damage = bCriticalHit ? Damage * 2.0f + SourceCriticalHitDamage : Damage;
	UAuraAbilitySystemLibrary::SetCriticalHit( EffectContextHandle , bCriticalHit );

	const FGameplayModifierEvaluatedData EvaluatedData( UAuraAttributeSet::GetIncomingDamageAttribute() ,
	                                                    EGameplayModOp::Additive , Damage );
	OutExecutionOutput.AddOutputModifier( EvaluatedData );
}
