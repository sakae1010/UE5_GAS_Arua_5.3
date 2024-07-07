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

	//主要能力
	FGameplayTag AuraAttribute_Primary_Strength;
	FGameplayTag AuraAttribute_Primary_Resilience;
	FGameplayTag AuraAttribute_Primary_Intelligence;
	FGameplayTag AuraAttribute_Primary_Vigor;
	
	//次要能力
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

	FGameplayTag Attributes_Meta_IncomingXP;
	
	FGameplayTag InputTag_LMB;
	FGameplayTag InputTag_RMB;
	FGameplayTag InputTag_1;
	FGameplayTag InputTag_2;
	FGameplayTag InputTag_3;
	FGameplayTag InputTag_4;
	FGameplayTag InputTag_Passive_1;
	FGameplayTag InputTag_Passive_2;
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
	
	FGameplayTag Abilities_HitReact;

	/*技能狀態*/
	//locked 鎖定的 have not spell points
	FGameplayTag Abilities_Status_Locked;
	//eligible 可以點了~有資格的 can be unlocked
	FGameplayTag Abilities_Status_Eligible;
	//unlocked 已解鎖 can be equipped
	FGameplayTag Abilities_Status_Unlocked;
	//equipped 已裝備
	FGameplayTag Abilities_Status_Equipped;
	/*技能類型*/
	FGameplayTag Abilities_Type_Offensive;
	FGameplayTag Abilities_Type_Passive;
	FGameplayTag Abilities_Type_None;

	
	FGameplayTag Abilities_Fire_FireBolt;
	FGameplayTag Abilities_Lightning_Electrocute;
	/* Cooldown */
	FGameplayTag Cooldown_Fire_FireBolt;
	/* 特效通知 */
	TMap<FGameplayTag , FGameplayTag> DamageTypesToResistances;
	//HitReact 為受擊反應
	FGameplayTag Effects_HitReact;
	
 private :
  static FAuraGameplayTags GameplayTags;
};

 
