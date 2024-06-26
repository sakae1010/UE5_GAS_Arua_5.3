// sakae's gas project

#pragma once

#include "CoreMinimal.h"
#include "GameplayTagContainer.h"

/**
 * AuraGameplayTags 
 * Singleton container native Gameplay Tags
 */
struct FAuraGameplayTags
{
public:
     static const FAuraGameplayTags& Get() {return GameplayTags;}
     static void InitializeNativeGameplayTags();

	FGameplayTag AuraAttribute_Primary_Strength;
	FGameplayTag AuraAttribute_Primary_Resilience;
	FGameplayTag AuraAttribute_Primary_Intelligence;
	FGameplayTag AuraAttribute_Primary_Vigor;
	
	
	FGameplayTag AuraAttribute_Secondary_Armor;
	FGameplayTag AuraAttribute_Secondary_ArmorPenetration;
	FGameplayTag AuraAttribute_Secondary_BlockChance;
	FGameplayTag AuraAttribute_Secondary_CriticalHitChance;
	FGameplayTag AuraAttribute_Secondary_CriticalHitDamage;
	FGameplayTag AuraAttribute_Secondary_CriticalHitResistance;
	FGameplayTag AuraAttribute_Secondary_HealthRegeneration;
	FGameplayTag AuraAttribute_Secondary_ManaRegeneration;
	FGameplayTag AuraAttribute_Secondary_MaxHealth;
	FGameplayTag AuraAttribute_Secondary_MaxMana;
	FGameplayTag Attributes_Resistance_Fire;
	FGameplayTag Attributes_Resistance_Lighting;
	FGameplayTag Attributes_Resistance_Arcane;
	FGameplayTag Attributes_Resistance_Physical;
	
	FGameplayTag InputTag_LMB;
	FGameplayTag InputTag_RMB;
	FGameplayTag InputTag_1;
	FGameplayTag InputTag_2;
	FGameplayTag InputTag_3;
	FGameplayTag InputTag_4;

	FGameplayTag Damage;
	FGameplayTag Damage_Fire;
	FGameplayTag Damage_Lighting;
	FGameplayTag Damage_Arcane;
	FGameplayTag Damage_Physical;
	//武器插槽的對應
	FGameplayTag CombatSocket_Weapon;
	FGameplayTag CombatSocket_RightHand;
	FGameplayTag CombatSocket_LeftHand;
	FGameplayTag CombatSocket_Tail;
	//攻擊動畫對應
	FGameplayTag Montage_Attack1;
	FGameplayTag Montage_Attack2;
	FGameplayTag Montage_Attack3;
	FGameplayTag Montage_Attack4;
	
	//受擊動畫對應
	
	//攻擊事件tag
	FGameplayTag Abilities_Attack;
	FGameplayTag Abilities_Summon;
	FGameplayTag Abilities_Fire_FireBolt;
	/* Cooldown */
	FGameplayTag Cooldown_Fire_FireBolt;
	/* 特效通知 */
	TMap<FGameplayTag , FGameplayTag> DamageTypesToResistances;
	//HitReact 為受擊反應
	FGameplayTag Effects_HitReact;
	
 private :
  static FAuraGameplayTags GameplayTags;
};

 
