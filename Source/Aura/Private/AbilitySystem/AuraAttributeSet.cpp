#include "AbilitySystem/AuraAttributeSet.h"
#include "AbilitySystemBlueprintLibrary.h"
#include "AuraGameplayTag.h"
#include "GameplayEffectExtension.h"
#include "AbilitySystem/AuraAbilitySystemLibrary.h"
#include "GameFramework/Character.h"
#include "GameplayEffectComponents/TargetTagsGameplayEffectComponent.h"
#include "Interaction/CombatInterface.h"
#include "Interaction/PlayerInterface.h"
#include "Net/UnrealNetwork.h"
#include "Player/AuraPlayerController.h"

UAuraAttributeSet::UAuraAttributeSet()
{
	const FAuraGameplayTags& AuraTags = FAuraGameplayTags::Get();
	/* Primary Attributes */
	TagsToAttributeMap.Add(AuraTags.AuraAttribute_Primary_Strength, GetStrengthAttribute);
	TagsToAttributeMap.Add(AuraTags.AuraAttribute_Primary_Vigor, GetVigorAttribute);
	TagsToAttributeMap.Add(AuraTags.AuraAttribute_Primary_Intelligence, GetIntelligenceAttribute);
	TagsToAttributeMap.Add(AuraTags.AuraAttribute_Primary_Resilience, GetResilienceAttribute);
	/* Secondary Attributes */
	TagsToAttributeMap.Add(AuraTags.AuraAttribute_Secondary_Armor, GetArmorAttribute);
	TagsToAttributeMap.Add(AuraTags.AuraAttribute_Secondary_ArmorPenetration, GetArmorPenetrationAttribute);
	TagsToAttributeMap.Add(AuraTags.AuraAttribute_Secondary_BlockChance, GetBlockChanceAttribute);
	TagsToAttributeMap.Add(AuraTags.AuraAttribute_Secondary_CriticalHitChance, GetCriticalHitChanceAttribute);
	TagsToAttributeMap.Add(AuraTags.AuraAttribute_Secondary_CriticalHitDamage, GetCriticalHitDamageAttribute);
	TagsToAttributeMap.Add(AuraTags.AuraAttribute_Secondary_CriticalHitResistance, GetCriticalHitResistanceAttribute);
	TagsToAttributeMap.Add(AuraTags.AuraAttribute_Secondary_HealthRegeneration, GetHealthRegenerationAttribute);
	TagsToAttributeMap.Add(AuraTags.AuraAttribute_Secondary_ManaRegeneration, GetManaRegenerationAttribute);
	TagsToAttributeMap.Add(AuraTags.AuraAttribute_Secondary_MaxHealth, GetMaxHealthAttribute);
	TagsToAttributeMap.Add(AuraTags.AuraAttribute_Secondary_MaxMana, GetMaxManaAttribute);
	/* 屬性抵抗 */
	TagsToAttributeMap.Add(AuraTags.Attributes_Resistance_Arcane, GetArcaneResistanceAttribute);
	TagsToAttributeMap.Add(AuraTags.Attributes_Resistance_Fire, GetFireResistanceAttribute);
	TagsToAttributeMap.Add(AuraTags.Attributes_Resistance_Lightning, GetLightningResistanceAttribute);
	TagsToAttributeMap.Add(AuraTags.Attributes_Resistance_Physical, GetPhysicalResistanceAttribute);
	
}

void UAuraAttributeSet::GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const
{
	Super::GetLifetimeReplicatedProps(OutLifetimeProps);
	/* secondary attributes */
	DOREPLIFETIME_CONDITION_NOTIFY(UAuraAttributeSet, Armor, COND_None, REPNOTIFY_Always);
	DOREPLIFETIME_CONDITION_NOTIFY(UAuraAttributeSet, ArmorPenetration, COND_None, REPNOTIFY_Always);
	DOREPLIFETIME_CONDITION_NOTIFY(UAuraAttributeSet, BlockChance, COND_None, REPNOTIFY_Always);
	DOREPLIFETIME_CONDITION_NOTIFY(UAuraAttributeSet, CriticalHitChance, COND_None, REPNOTIFY_Always);
	DOREPLIFETIME_CONDITION_NOTIFY(UAuraAttributeSet, CriticalHitDamage, COND_None, REPNOTIFY_Always);
	DOREPLIFETIME_CONDITION_NOTIFY(UAuraAttributeSet, CriticalHitResistance, COND_None, REPNOTIFY_Always);
	DOREPLIFETIME_CONDITION_NOTIFY(UAuraAttributeSet, HealthRegeneration, COND_None, REPNOTIFY_Always);
	DOREPLIFETIME_CONDITION_NOTIFY(UAuraAttributeSet, ManaRegeneration, COND_None, REPNOTIFY_Always);
	DOREPLIFETIME_CONDITION_NOTIFY(UAuraAttributeSet, MaxHealth, COND_None, REPNOTIFY_Always);
	DOREPLIFETIME_CONDITION_NOTIFY(UAuraAttributeSet, MaxMana, COND_None, REPNOTIFY_Always);
	/* 屬性抵抗 */
	DOREPLIFETIME_CONDITION_NOTIFY(UAuraAttributeSet, FireResistance, COND_None, REPNOTIFY_Always);
	DOREPLIFETIME_CONDITION_NOTIFY(UAuraAttributeSet, ArcaneResistance, COND_None, REPNOTIFY_Always);
	DOREPLIFETIME_CONDITION_NOTIFY(UAuraAttributeSet, LightningResistance, COND_None, REPNOTIFY_Always);
	DOREPLIFETIME_CONDITION_NOTIFY(UAuraAttributeSet, PhysicalResistance, COND_None, REPNOTIFY_Always);

	// primary attributes
	DOREPLIFETIME_CONDITION_NOTIFY(UAuraAttributeSet, Strength, COND_None, REPNOTIFY_Always);
	DOREPLIFETIME_CONDITION_NOTIFY(UAuraAttributeSet, Vigor, COND_None, REPNOTIFY_Always);
	DOREPLIFETIME_CONDITION_NOTIFY(UAuraAttributeSet, Intelligence, COND_None, REPNOTIFY_Always);
	DOREPLIFETIME_CONDITION_NOTIFY(UAuraAttributeSet, Resilience, COND_None, REPNOTIFY_Always);

	// 重要屬性
	DOREPLIFETIME_CONDITION_NOTIFY(UAuraAttributeSet, Health, COND_None, REPNOTIFY_Always);
	DOREPLIFETIME_CONDITION_NOTIFY(UAuraAttributeSet, Mana, COND_None, REPNOTIFY_Always);
}

void UAuraAttributeSet::PreAttributeChange(const FGameplayAttribute& Attribute, float& NewValue)
{
	Super::PreAttributeChange(Attribute, NewValue);

	if (Attribute == GetHealthAttribute())
	{
		NewValue = FMath::Clamp(NewValue, 0.f, GetMaxHealth());
	}
	else if (Attribute == GetManaAttribute())
	{
		NewValue = FMath::Clamp(NewValue, 0.f, GetMaxMana());
	}
	
}

void UAuraAttributeSet::PostGameplayEffectExecute(const FGameplayEffectModCallbackData& Data)
{
	Super::PostGameplayEffectExecute(Data);
	FEffectProperties Props;
	SetEffectProperties(Data, Props);

	if (Props.TargetCharacter->Implements<UCombatInterface>() && ICombatInterface::Execute_IsDead( Props.TargetCharacter))
	{
		return;
	}

	if (Data.EvaluatedData.Attribute == GetHealthAttribute())
	{
		SetHealth(FMath::Clamp(GetHealth(), 0.f, GetMaxHealth()));
	}
	if (Data.EvaluatedData.Attribute == GetManaAttribute())
	{
		SetMana(FMath::Clamp(GetMana(), 0.f, GetMaxMana()));
	}

	if(Data.EvaluatedData.Attribute == GetIncomingDamageAttribute())
	{
		HandleIncomingDamage( Props );
	}

	if (Data.EvaluatedData.Attribute == GetIncomingXPAttribute())
	{
		HandleIncomingXP( Props );
	}
	
}

void UAuraAttributeSet::Debuff(const FEffectProperties& Props)
{
	FGameplayEffectContextHandle EffectContext = Props.SourceAbilitySystemComponent->MakeEffectContext();
	EffectContext.AddSourceObject(Props.SourceAvatarActor);
	const FAuraGameplayTags& GameplayTags = FAuraGameplayTags::Get();
	const FGameplayTag DamageType = UAuraAbilitySystemLibrary::GetDamageType( Props.EffectContextHandle );
	const float DebuffDuration = UAuraAbilitySystemLibrary::GetDebuffDuration( Props.EffectContextHandle );
	const float DebuffFrequency = UAuraAbilitySystemLibrary::GetDebuffFrequency( Props.EffectContextHandle );
	const float DebuffDamage = UAuraAbilitySystemLibrary::GetDebuffDamage( Props.EffectContextHandle );

	const FString DebuffName = FString::Printf( TEXT( "DynmaicDebuff_%s" ) , *DamageType.ToString() );
	UGameplayEffect* Effect = NewObject<UGameplayEffect>( GetTransientPackage(), FName( *DebuffName ) );
	Effect->DurationPolicy = EGameplayEffectDurationType::HasDuration;
	Effect->Period = DebuffFrequency;
	Effect->DurationMagnitude = FScalableFloat( DebuffDuration );
	// 5.3已經捨去的方法
	// Effect->InheritableOwnedTagsContainer.AddTag( GameplayTags.DamageTypesToDebuffs[DamageType] );
	// 另一種做法
	// Effect->CachedGrantedTags.AddTag(GameplayTags.DamageTypesToDebuffs[DamageType]);
	// 以下這個做法會比安全
	UTargetTagsGameplayEffectComponent& AssetTagsComponent =  Effect->FindOrAddComponent<UTargetTagsGameplayEffectComponent>();
	FInheritedTagContainer InheritedTagContainer;
	const FGameplayTag DebuffTag = GameplayTags.DamageTypesToDebuffs [ DamageType ];
	InheritedTagContainer.Added.AddTag(DebuffTag);
	if(DebuffTag.MatchesTag( GameplayTags.Debuff_Stun ))
	{
		InheritedTagContainer.Added.AddTag(  GameplayTags.Player_Block_InputHeld );
		InheritedTagContainer.Added.AddTag(  GameplayTags.Player_Block_InputReleased );
		InheritedTagContainer.Added.AddTag(  GameplayTags.Player_Block_InputPressed );
		InheritedTagContainer.Added.AddTag(  GameplayTags.Player_Block_CursorTrace );
	}
	AssetTagsComponent.SetAndApplyTargetTagChanges(InheritedTagContainer);
	Effect->StackingType = EGameplayEffectStackingType::AggregateBySource;
	Effect->StackLimitCount = 1;

	const int32 Index = Effect->Modifiers.Num();
	Effect->Modifiers.Add( FGameplayModifierInfo() );
	FGameplayModifierInfo& Modifier = Effect->Modifiers [ Index ];
	Modifier.ModifierMagnitude = FScalableFloat( DebuffDamage );
	Modifier.ModifierOp = EGameplayModOp::Additive;
	Modifier.Attribute = GetIncomingDamageAttribute();


	if(FGameplayEffectSpec* MutableSpec = new FGameplayEffectSpec( Effect , EffectContext ,1.f))
	{
		FAuraGameplayEffectContext* AuraContext = static_cast<FAuraGameplayEffectContext*>(MutableSpec->GetContext().Get());
		TSharedRef<FGameplayTag> DebuffGameplayTag = MakeShareable<FGameplayTag>(new FGameplayTag(DamageType) );
		AuraContext->SetDamageType( DebuffGameplayTag );
		if(Props.TargetAbilitySystemComponent != nullptr)
		{
			Props.TargetAbilitySystemComponent->ApplyGameplayEffectSpecToSelf(*MutableSpec);
		}
	}
}

void UAuraAttributeSet::HandleIncomingDamage(const FEffectProperties& Props)
{
	const float LocalIncomingDamage = GetIncomingDamage();
	SetIncomingDamage(0.f);
	if(LocalIncomingDamage <= 0.f)return;
	const float NewHealth = GetHealth() - LocalIncomingDamage;
	SetHealth(FMath::Clamp(NewHealth, 0.f, GetMaxHealth()));
	//Fatal Damage = 致命傷害
	const float bFatal = NewHealth <= 0.f;
	if (bFatal)
	{
		if(ICombatInterface* CombatInterface = Cast<ICombatInterface>(Props.TargetAvatarActor))
		{
			FVector DeadthImpuse = UAuraAbilitySystemLibrary::GetDeathImpulse( Props.EffectContextHandle );
			CombatInterface->Die(DeadthImpuse);
		}
		SendXPEvent(Props);
	}
	else
	{
		if (Props.TargetCharacter->Implements<UCombatInterface>() && !ICombatInterface::Execute_IsBeingShocked(Props.TargetCharacter))
		{
			FGameplayTagContainer HitReactTagContainer;
			HitReactTagContainer.AddTag(FAuraGameplayTags::Get().Effects_HitReact);
			Props.TargetAbilitySystemComponent->TryActivateAbilitiesByTag(HitReactTagContainer);
		}

		const FVector& KnokbackForce = UAuraAbilitySystemLibrary::GetKnokbackForce( Props.EffectContextHandle );
		if(!KnokbackForce.IsNearlyZero(1.f))
		{
			Props.TargetCharacter->LaunchCharacter( KnokbackForce , true , true );
		}
	}

	const bool bIsBlockHit = UAuraAbilitySystemLibrary::IsBlockedHit(Props.EffectContextHandle);
	const bool bIsCriticalHit = UAuraAbilitySystemLibrary::IsCriticalHit(Props.EffectContextHandle);
	ShowFloatingText(Props, LocalIncomingDamage , bIsBlockHit , bIsCriticalHit);

	if (UAuraAbilitySystemLibrary::IsSuccessfulDebuff(Props.EffectContextHandle ))
	{
		Debuff( Props );
	}
}

void UAuraAttributeSet::HandleIncomingXP(const FEffectProperties& Props)
{
	const float LocalIncomingXP = GetIncomingXP();
	SetIncomingXP(0.f);

	// 來源角色 必須要有 GA_ListenEvent 並且 應用 GE_EventBasedEffect 來 Add meta XP 來源角色必須實作 IPlayerInterface
	if(Props.SourceCharacter->Implements<UPlayerInterface>() && Props.SourceCharacter->Implements<UCombatInterface>())
	{
		const int32 CurrentLevel = ICombatInterface::Execute_GetPlayerLevel(Props.SourceCharacter);
		const int32 CurrentXP = IPlayerInterface::Execute_GetXP(Props.SourceCharacter);
		const int32 NewLevel = IPlayerInterface::Execute_FindLevelForXP(Props.SourceCharacter, CurrentXP + LocalIncomingXP);
		const int32 NumLevelUps = NewLevel - CurrentLevel;
		if(NumLevelUps > 0)
		{
			IPlayerInterface::Execute_AddToPlayerLevel(Props.SourceCharacter, NumLevelUps);
			int32 AttributePointsReward = 0;
			int32 SpellPointsReward = 0;
			for(int32 i = 0; i < NumLevelUps; i++)
			{
				AttributePointsReward += IPlayerInterface::Execute_GetAttributePointsReward(Props.SourceCharacter, CurrentLevel + i);
				SpellPointsReward += IPlayerInterface::Execute_GetSpellPointsReward(Props.SourceCharacter, CurrentLevel + i);
			}
			IPlayerInterface::Execute_AddToAttributePoints(Props.SourceCharacter, AttributePointsReward);
			IPlayerInterface::Execute_AddToSpellPoints(Props.SourceCharacter, SpellPointsReward);
			bTopOffHealth = true;
			bTopOffMana = true;
			IPlayerInterface::Execute_LevelUp(Props.SourceCharacter);
		}
			
		IPlayerInterface::Execute_AddToXP(Props.SourceCharacter, LocalIncomingXP);
	}
}

void UAuraAttributeSet::PostAttributeChange(const FGameplayAttribute& Attribute, float OldValue, float NewValue)
{
	Super::PostAttributeChange(Attribute, OldValue, NewValue);
	if(Attribute == GetMaxHealthAttribute() && bTopOffHealth)
	{
		SetHealth(GetMaxHealth());
		bTopOffHealth = false;
	}

	if(Attribute == GetMaxManaAttribute() && bTopOffMana)
	{
		SetMana(GetMaxMana());
		bTopOffMana = false;
	}

	
}

void UAuraAttributeSet::ShowFloatingText(const FEffectProperties& Props, float Damage , bool bIsBlockHit , bool bIsCriticalHit) const
{
	if(Props.SourceCharacter != Props.TargetCharacter)
	{
		if(Props.SourceCharacter!=nullptr)
		{
			if(AAuraPlayerController* PC = Cast<AAuraPlayerController>(Props.SourceCharacter->Controller))
			{
				PC->ShowDamageNumber(Damage, Props.TargetCharacter ,bIsBlockHit , bIsCriticalHit );
				return;
			}
		}
		if (Props.TargetCharacter!=nullptr)
		{
			if(AAuraPlayerController* PC = Cast<AAuraPlayerController>(Props.TargetCharacter->Controller))
			{
				PC->ShowDamageNumber(Damage, Props.TargetCharacter ,bIsBlockHit , bIsCriticalHit );
			}
		}
	}
}

void UAuraAttributeSet::SendXPEvent(const FEffectProperties& Props)
{
	if(Props.TargetAvatarActor!=nullptr && Props.TargetAvatarActor->Implements<UCombatInterface>())
	{
		const int32 TargetLevel = ICombatInterface::Execute_GetPlayerLevel(Props.TargetAvatarActor);
		const ECharacterClass TargetClass = ICombatInterface::Execute_GetCharacterClass(Props.TargetCharacter); 
		const int32 XPValue = UAuraAbilitySystemLibrary::GetXpRewardForCharacterClassAndLevel(Props.TargetAbilitySystemComponent, TargetClass, TargetLevel);
		
		const FAuraGameplayTags& AuraTags = FAuraGameplayTags::Get();
		FGameplayEventData Payload;
		Payload.EventTag = AuraTags.Attributes_Meta_IncomingXP;
		Payload.Target = Props.TargetAvatarActor;
		Payload.Instigator = Props.SourceAvatarActor;
		Payload.EventMagnitude = XPValue;
		UAbilitySystemBlueprintLibrary::SendGameplayEventToActor(Props.SourceCharacter, AuraTags.Attributes_Meta_IncomingXP, Payload);
	}
	

}

void UAuraAttributeSet::SetEffectProperties(const FGameplayEffectModCallbackData& Data,	FEffectProperties& Props) const
{
	// Source = causer of the effect , Target = the target of the effect(owner of the attribute set)
	Props.EffectContextHandle = Data.EffectSpec.GetContext();
	Props.SourceAbilitySystemComponent = Props.EffectContextHandle.GetOriginalInstigatorAbilitySystemComponent();

	if(IsValid(Props.SourceAbilitySystemComponent) && Props.SourceAbilitySystemComponent->AbilityActorInfo.IsValid() && Props.SourceAbilitySystemComponent->AbilityActorInfo->AvatarActor.IsValid())
	{
		Props.SourceAvatarActor = Props.SourceAbilitySystemComponent->GetAvatarActor();
		Props.SourceController = Props.SourceAbilitySystemComponent->AbilityActorInfo->PlayerController.Get();
		if(Props.SourceController==nullptr && Props.SourceAvatarActor!=nullptr)
		{
			if(const APawn* Pawn = Cast<APawn>(Props.SourceAvatarActor))
			{
				Props.SourceController = Cast<APlayerController>(Pawn->GetController());
			}	
		}
		if (Props.SourceController)
		{
			Props.SourceCharacter = Cast<ACharacter>(Props.SourceController->GetPawn());
		}
	}
	if( Data.Target.AbilityActorInfo.IsValid() && Data.Target.AbilityActorInfo->AvatarActor.IsValid())
	{
		Props.TargetAvatarActor = Data.Target.GetAvatarActor();
		Props.TargetController = Data.Target.AbilityActorInfo->PlayerController.Get();
		Props.TargetCharacter = Cast<ACharacter>(Props.TargetAvatarActor);
		Props.TargetAbilitySystemComponent = UAbilitySystemBlueprintLibrary::GetAbilitySystemComponent(Props.TargetAvatarActor);
	}
}




void UAuraAttributeSet::OnRep_Health(const FGameplayAttributeData& OldHealth) const
{
	GAMEPLAYATTRIBUTE_REPNOTIFY(UAuraAttributeSet, Health , OldHealth)
}

void UAuraAttributeSet::OnRep_MaxHealth(const FGameplayAttributeData& OldHealth) const
{
	GAMEPLAYATTRIBUTE_REPNOTIFY(UAuraAttributeSet, MaxHealth, OldHealth);
}

void UAuraAttributeSet::OnRep_Mana(const FGameplayAttributeData& OldMana) const
{
	GAMEPLAYATTRIBUTE_REPNOTIFY(UAuraAttributeSet, Mana, OldMana);
}

void UAuraAttributeSet::OnRep_MaxMana(const FGameplayAttributeData& OldMaxMana) const
{
	GAMEPLAYATTRIBUTE_REPNOTIFY(UAuraAttributeSet, MaxMana, OldMaxMana);
}

void UAuraAttributeSet::OnRep_FireResistance(const FGameplayAttributeData& OldFireResistance) const
{
	GAMEPLAYATTRIBUTE_REPNOTIFY(UAuraAttributeSet, FireResistance, OldFireResistance);
}

void UAuraAttributeSet::OnRep_LightningResistance(const FGameplayAttributeData& OldLightningResistance) const
{
	GAMEPLAYATTRIBUTE_REPNOTIFY(UAuraAttributeSet, LightningResistance, OldLightningResistance);
}

void UAuraAttributeSet::OnRep_ArcaneResistance(const FGameplayAttributeData& OldArcaneResistance) const
{
	GAMEPLAYATTRIBUTE_REPNOTIFY(UAuraAttributeSet, ArcaneResistance, OldArcaneResistance);
}

void UAuraAttributeSet::OnRep_PhysicalResistance(const FGameplayAttributeData& OldPhysicalResistance) const
{
	GAMEPLAYATTRIBUTE_REPNOTIFY(UAuraAttributeSet, PhysicalResistance, OldPhysicalResistance);
}



void UAuraAttributeSet::OnRep_Strength(const FGameplayAttributeData& OldStrength) const
{
	GAMEPLAYATTRIBUTE_REPNOTIFY(UAuraAttributeSet, Strength, OldStrength);
}

void UAuraAttributeSet::OnRep_Resilience(const FGameplayAttributeData& OldResilience) const
{
	GAMEPLAYATTRIBUTE_REPNOTIFY(UAuraAttributeSet, Resilience, OldResilience);
}

void UAuraAttributeSet::OnRep_Intelligence(const FGameplayAttributeData& OldIntelligence) const
{
	GAMEPLAYATTRIBUTE_REPNOTIFY(UAuraAttributeSet, Intelligence, OldIntelligence);
}

void UAuraAttributeSet::OnRep_Vigor(const FGameplayAttributeData& OldVigor) const
{
	GAMEPLAYATTRIBUTE_REPNOTIFY(UAuraAttributeSet, Vigor, OldVigor);
}

void UAuraAttributeSet::OnRep_ArmorPenetration(const FGameplayAttributeData& OldArmorPenetration) const
{
	GAMEPLAYATTRIBUTE_REPNOTIFY(UAuraAttributeSet, ArmorPenetration, OldArmorPenetration);
}

void UAuraAttributeSet::OnRep_Armor(const FGameplayAttributeData& OldArmor) const
{
	GAMEPLAYATTRIBUTE_REPNOTIFY(UAuraAttributeSet, Armor, OldArmor);
}

void UAuraAttributeSet::OnRep_BlockChance(const FGameplayAttributeData& OldBlockChance) const
{
	GAMEPLAYATTRIBUTE_REPNOTIFY(UAuraAttributeSet, BlockChance, OldBlockChance);
}

void UAuraAttributeSet::OnRep_CriticalHitChance(const FGameplayAttributeData& OldCriticalHitChance) const
{
	GAMEPLAYATTRIBUTE_REPNOTIFY(UAuraAttributeSet, CriticalHitChance, OldCriticalHitChance);
}

void UAuraAttributeSet::OnRep_CriticalHitDamage(const FGameplayAttributeData& OldCriticalHitDamage) const
{
	GAMEPLAYATTRIBUTE_REPNOTIFY(UAuraAttributeSet, CriticalHitDamage, OldCriticalHitDamage);
}

void UAuraAttributeSet::OnRep_CriticalHitResistance(const FGameplayAttributeData& OldCriticalHitResistance) const
{
	GAMEPLAYATTRIBUTE_REPNOTIFY(UAuraAttributeSet, CriticalHitResistance, OldCriticalHitResistance);
}

void UAuraAttributeSet::OnRep_HealthRegeneration(const FGameplayAttributeData& OldHealthRegeneration) const
{
	GAMEPLAYATTRIBUTE_REPNOTIFY(UAuraAttributeSet, HealthRegeneration, OldHealthRegeneration);
}

void UAuraAttributeSet::OnRep_ManaRegeneration(const FGameplayAttributeData& OldManaRegeneration) const
{
	GAMEPLAYATTRIBUTE_REPNOTIFY(UAuraAttributeSet, ManaRegeneration, OldManaRegeneration);
}
