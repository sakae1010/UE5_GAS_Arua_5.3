// sakae's gas project


#include "AuraGameplayTag.h"
#include "GameplayTagsManager.h"
FAuraGameplayTags FAuraGameplayTags::GameplayTags;

void FAuraGameplayTags::InitializeNativeGameplayTags()
{
	UGameplayTagsManager& GameplayTagsManager = UGameplayTagsManager::Get();

	/** Primary Attributes */
	
	GameplayTags.AuraAttribute_Primary_Strength = GameplayTagsManager.AddNativeGameplayTag(
		FName("Attributes.Primary.Strength"),
		FString("Increases Physical Damage, Health, and Armor"));
	GameplayTags.AuraAttribute_Primary_Resilience = GameplayTagsManager.AddNativeGameplayTag(
		FName("Attributes.Primary.Resilience"),
		FString("Increases Health, Health Regeneration, and Armor"));
	GameplayTags.AuraAttribute_Primary_Intelligence = GameplayTagsManager.AddNativeGameplayTag(
		FName("Attributes.Primary.Intelligence"),
		FString("Increases Magical Damage, Mana, and Mana Regeneration"));
	GameplayTags.AuraAttribute_Primary_Vigor = GameplayTagsManager.AddNativeGameplayTag(
		FName("Attributes.Primary.Vigor"),
		FString("Increases Health, Health Regeneration, and Physical Damage"));
	
	/** Secondary Attributes */
	
	GameplayTags.AuraAttribute_Secondary_Armor = GameplayTagsManager.AddNativeGameplayTag(
		FName("Attributes.Secondary.Armor"),
		FString("Reduces damage taken, improves Block chance"));
	
	GameplayTags.AuraAttribute_Secondary_ArmorPenetration = GameplayTagsManager.AddNativeGameplayTag(
		FName("Attributes.Secondary.ArmorPenetration"),
		FString("Ignores percentage of enemy Armor, increases Crit Hit Chance"));
	
	GameplayTags.AuraAttribute_Secondary_BlockChance = GameplayTagsManager.AddNativeGameplayTag(
		FName("Attributes.Secondary.BlockChance"),
		FString("Chance Chance to cut incoming damage in half"));
	
	GameplayTags.AuraAttribute_Secondary_CriticalHitChance = GameplayTagsManager.AddNativeGameplayTag(
		FName("Attributes.Secondary.CriticalHitChance"),
		FString("Chance to double damage plus critical hit bonus"));
	
	GameplayTags.AuraAttribute_Secondary_CriticalHitDamage = GameplayTagsManager.AddNativeGameplayTag(
		FName("Attributes.Secondary.CriticalHitDamage"),
		FString("Bonus damage added when a critical hit is scored"));
	
	GameplayTags.AuraAttribute_Secondary_CriticalHitResistance = GameplayTagsManager.AddNativeGameplayTag(
		FName("Attributes.Secondary.CriticalHitResistance"),
		FString("Reduces critical hit chance of attacking enemies"));
	
	GameplayTags.AuraAttribute_Secondary_HealthRegeneration = GameplayTagsManager.AddNativeGameplayTag(
		FName("Attributes.Secondary.HealthRegeneration"),
		FString("Amount of Health regenerated every 1 second"));
	
	GameplayTags.AuraAttribute_Secondary_ManaRegeneration = GameplayTagsManager.AddNativeGameplayTag(
		FName("Attributes.Secondary.ManaRegeneration"),
		FString("Amount of Mana regenerated every 1 second"));
	
	GameplayTags.AuraAttribute_Secondary_MaxHealth = GameplayTagsManager.AddNativeGameplayTag(
		FName("Attributes.Secondary.MaxHealth"),
		FString("Maximum amount of Health obtainable"));
	
	GameplayTags.AuraAttribute_Secondary_MaxMana = GameplayTagsManager.AddNativeGameplayTag(
		FName("Attributes.Secondary.MaxMana"),
		FString("Maximum amount of Mana obtainable"));

	/** Input Tags */
	
	GameplayTags.InputTag_LMB = GameplayTagsManager.AddNativeGameplayTag(
		FName("InputTag.LMB"),
		FString("Left Mouse Button"));

	
	GameplayTags.InputTag_RMB = GameplayTagsManager.AddNativeGameplayTag(
		FName("InputTag.RMB"),
		FString("Right Mouse Button"));

	GameplayTags.InputTag_1 = GameplayTagsManager.AddNativeGameplayTag(
		FName("InputTag.1"),
		FString("1 Key"));

	GameplayTags.InputTag_2 = GameplayTagsManager.AddNativeGameplayTag(
		FName("InputTag.2"),
		FString("2 Key"));

	GameplayTags.InputTag_3	= GameplayTagsManager.AddNativeGameplayTag(
		FName("InputTag.3"),
		FString("3 Key"));

	GameplayTags.InputTag_4 = GameplayTagsManager.AddNativeGameplayTag(
		FName("InputTag.4"),
		FString("4 Key"));

	GameplayTags.Damage = GameplayTagsManager.AddNativeGameplayTag(
	FName("Damage"),
	FString("Damage"));
	
	//HitReact 為受擊反應
	GameplayTags.Effects_HitReact = GameplayTagsManager.AddNativeGameplayTag(
		FName("Effects.HitReact"),
		FString("Tag Granted when Hit Reacting"));
	
	}
