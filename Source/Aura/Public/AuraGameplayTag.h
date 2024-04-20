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
 
protected :

 private :
  static FAuraGameplayTags GameplayTags;
};

 
