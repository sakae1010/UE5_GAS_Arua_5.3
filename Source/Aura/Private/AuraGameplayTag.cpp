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


	/*   Meta Attributes  */
	GameplayTags.Attributes_Meta_IncomingXP = GameplayTagsManager.AddNativeGameplayTag(
		FName("Attributes.Meta.IncomingXP"),
		FString("Incoming XP Meta Attributes")
		);
	
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

	GameplayTags.InputTag_Passive_1 = GameplayTagsManager.AddNativeGameplayTag(
		FName("InputTag.Passive.1"),
		FString("Input Tag Passive 1 Ability"));

	GameplayTags.InputTag_Passive_2 = GameplayTagsManager.AddNativeGameplayTag(
		FName("InputTag.Passive.2"),
		FString("Input Tag Passive 2 Ability"));


// DamageTypes vvvvvv
	GameplayTags.Damage = GameplayTagsManager.AddNativeGameplayTag(
		FName("Damage"),
		FString("Damage")
		);
	GameplayTags.Damage_Fire = GameplayTagsManager.AddNativeGameplayTag(
		FName("Damage.Fire"),
		FString("Fire Damage Type")
		);
	GameplayTags.Damage_Lightning = GameplayTagsManager.AddNativeGameplayTag(
		FName("Damage.Lightning"),
		FString("Lightning Damage Type")
		);
	GameplayTags.Damage_Arcane = GameplayTagsManager.AddNativeGameplayTag(
		FName("Damage.Arcane"),
		FString("Arcane Damage Type")
		);
	GameplayTags.Damage_Physical = GameplayTagsManager.AddNativeGameplayTag(
		FName("Damage.Physical"),
		FString("Physical Damage Type")
		);
// DamageTypes ^^^^^^^

	/*
	* DamageType Resistances vvvvvv
	*/
	GameplayTags.Attributes_Resistance_Fire = GameplayTagsManager.AddNativeGameplayTag(
		FName("Attributes.Resistance.Fire"),
		FString("Resistance to Fire Type")
		);

	GameplayTags.Attributes_Resistance_Lightning = GameplayTagsManager.AddNativeGameplayTag(
		FName("Attributes.Resistance.Lightning"),
		FString("Resistance to Lightning  Type")
		);
	
	GameplayTags.Attributes_Resistance_Arcane = GameplayTagsManager.AddNativeGameplayTag(
		FName("Attributes.Resistance.Arcane"),
		FString("Resistance to Arcane  Type")
		);
	
	GameplayTags.Attributes_Resistance_Physical = GameplayTagsManager.AddNativeGameplayTag(
		FName("Attributes.Resistance.Physical"),
		FString("Resistance to Physical  Type")
		);
// DamageType Resistances ^^^^^^

	// DebuffTag
	GameplayTags.Debuff_Burn = GameplayTagsManager.AddNativeGameplayTag(
		FName("Debuff.Burn"),
		FString("Debuff for Fire Type")
		);

	GameplayTags.Debuff_Stun = GameplayTagsManager.AddNativeGameplayTag(
		FName("Debuff.Stun"),
		FString("Debuff to Lightning  Type")
		);
	
	GameplayTags.Debuff_Arcane = GameplayTagsManager.AddNativeGameplayTag(
		FName("Debuff.Arcane"),
		FString("Debuff to Arcane  Type")
		);
	
	GameplayTags.Debuff_Physical = GameplayTagsManager.AddNativeGameplayTag(
		FName("Debuff.Physical"),
		FString("Debuff to Physical  Type")
		);

	GameplayTags.Debuff_Chance = GameplayTagsManager.AddNativeGameplayTag(
		FName("Debuff.Chance"),
		FString("Debuff Chance")
		);
	GameplayTags.Debuff_Damage = GameplayTagsManager.AddNativeGameplayTag(
		FName("Debuff.Damage"),
		FString("Debuff Damage")
		);
	GameplayTags.Debuff_Frequency = GameplayTagsManager.AddNativeGameplayTag(
		FName("Debuff.Frequency"),
		FString("Debuff Frequency")
		);
	GameplayTags.Debuff_Duration = GameplayTagsManager.AddNativeGameplayTag(
		FName("Debuff.Duration"),
		FString("Debuff Duration")
		);
	/*
	 *map to Damage to Resistance 
	 */
	GameplayTags.DamageTypesToResistances.Add(GameplayTags.Damage_Arcane, GameplayTags.Attributes_Resistance_Arcane);
	GameplayTags.DamageTypesToResistances.Add(GameplayTags.Damage_Lightning, GameplayTags.Attributes_Resistance_Lightning);
	GameplayTags.DamageTypesToResistances.Add(GameplayTags.Damage_Physical, GameplayTags.Attributes_Resistance_Physical);
	GameplayTags.DamageTypesToResistances.Add(GameplayTags.Damage_Fire, GameplayTags.Attributes_Resistance_Fire);

	/*
	 * Map to Damage to Debuff
	 */

	GameplayTags.DamageTypesToDebuffs.Add(GameplayTags.Damage_Arcane, GameplayTags.Debuff_Arcane);
	GameplayTags.DamageTypesToDebuffs.Add(GameplayTags.Damage_Lightning, GameplayTags.Debuff_Stun);
	GameplayTags.DamageTypesToDebuffs.Add(GameplayTags.Damage_Physical, GameplayTags.Debuff_Physical);
	GameplayTags.DamageTypesToDebuffs.Add(GameplayTags.Damage_Fire, GameplayTags.Debuff_Burn);
	/*
	 *HitReact 為受擊反應
	 */
	GameplayTags.Effects_HitReact = GameplayTagsManager.AddNativeGameplayTag(
		FName("Effects.HitReact"),
		FString("Tag Granted when Hit Reacting"));

	/*
	 *  Abilities
	 */
	GameplayTags.Abilities_Attack = GameplayTagsManager.AddNativeGameplayTag(
		FName("Abilities.Attack"),
		FString("Attack Abilities Tag"));

	GameplayTags.Abilities_Summon = GameplayTagsManager.AddNativeGameplayTag(
		FName("Abilities.Summon"),
		FString("Summon Abilities Tag"));

	GameplayTags.Abilities_None  = GameplayTagsManager.AddNativeGameplayTag(
		FName("Abilities.None"),
		FString("No Ability - like this nullptr for Ability Tags "));

	/*
	 * Offensive Spell
	 */
	
	GameplayTags.Abilities_Fire_FireBolt = GameplayTagsManager.AddNativeGameplayTag(
		FName("Abilities.Fire.FireBolt"),
	FString("FireBolt Ability Tag"));

	GameplayTags.Abilities_Fire_FireBlast = GameplayTagsManager.AddNativeGameplayTag(
		FName("Abilities.Fire.FireBlast"),
		FString("FireBlast Ability Tag"));

	GameplayTags.Abilities_Lightning_Electrocute = GameplayTagsManager.AddNativeGameplayTag(
		FName("Abilities.Lightning.Electrocute"),
		FString("Electrocute Ability Tag"));

	GameplayTags.Abilities_Arcane_ArcaneShards = GameplayTagsManager.AddNativeGameplayTag(
		FName("Abilities.Arcane.ArcaneShards"),
		FString("ArcaneShards Ability Tag"));
	
	/* Passive Spell */
	
	GameplayTags.Abilities_Passive_HaloOfProtection = GameplayTagsManager.AddNativeGameplayTag(
		FName("Abilities.Passive.HaloOfProtection"),
		FString("HaloOfProtection Passive Abilities Tag"));

	GameplayTags.Abilities_Passive_LifeSiphon = GameplayTagsManager.AddNativeGameplayTag(
		FName("Abilities.Passive.LifeSiphon"),
		FString("LifeSiphon Passive Abilities Tag"));

	GameplayTags.Abilities_Passive_ManaSiphon = GameplayTagsManager.AddNativeGameplayTag(
		FName("Abilities.Passive.ManaSiphon"),
		FString("ManaSiphon Passive Abilities Tag"));

	/* HitReact */

	GameplayTags.Abilities_HitReact = GameplayTagsManager.AddNativeGameplayTag(
		FName("Abilities.HitReact"),
		FString("HitReact Abilities"));

	/* ability stauts*/
	GameplayTags.Abilities_Status_Locked = GameplayTagsManager.AddNativeGameplayTag(
		FName("Abilities.Status.Locked"),
		FString("Locked Status"));
	
	GameplayTags.Abilities_Status_Eligible = GameplayTagsManager.AddNativeGameplayTag(
		FName("Abilities.Status.Eligible"),
		FString("Eligible Status"));
	
	GameplayTags.Abilities_Status_Unlocked = GameplayTagsManager.AddNativeGameplayTag(
		FName("Abilities.Status.Unlocked"),
		FString("Unlocked Status"));
	
	GameplayTags.Abilities_Status_Equipped = GameplayTagsManager.AddNativeGameplayTag(
		FName("Abilities.Status.Equipped"),
		FString("Equipped Status"));

	/* ability type */
	GameplayTags.Abilities_Type_Offensive = GameplayTagsManager.AddNativeGameplayTag(
		FName("Abilities.Type.Offensive"),
		FString("Offensive Abilities Type Tag"));

	GameplayTags.Abilities_Type_Passive = GameplayTagsManager.AddNativeGameplayTag(
		FName("Abilities.Type.Passive"),
		FString("Passive Abilities Type Tag"));

	GameplayTags.Abilities_Type_None = GameplayTagsManager.AddNativeGameplayTag(
		FName("Abilities.Type.None"),
	FString("None Abilities Type Tag"));
	
	/*
	 * Cooldown Tags
	*/
	GameplayTags.Cooldown_Fire_FireBolt = GameplayTagsManager.AddNativeGameplayTag(
		FName("Cooldown.Fire.FireBolt"),
		FString("FireBolt Cooldown Tag"));
	GameplayTags.Cooldown_Lightning_Electrocute = GameplayTagsManager.AddNativeGameplayTag(
		FName("Cooldown.Lightning.Electrocute"),
		FString("Electrocute Cooldown Tag"));
	GameplayTags.Cooldown_Arcane_ArcaneShards = GameplayTagsManager.AddNativeGameplayTag(
		FName("Cooldown.Arcane.ArcaneShards"),
		FString("ArcaneShards Cooldown Tag"));
	/*
	 *Socket Tag
	*/
	
	GameplayTags.CombatSocket_Weapon = GameplayTagsManager.AddNativeGameplayTag(
		FName("CombatSocket.Weapon"),
		FString("Weapon"));
	
	GameplayTags.CombatSocket_RightHand = GameplayTagsManager.AddNativeGameplayTag(
		FName("CombatSocket.RightHand"),
		FString("RightHand"));
	
	GameplayTags.CombatSocket_LeftHand = GameplayTagsManager.AddNativeGameplayTag(
		FName("CombatSocket.LeftHand"),
		FString("LeftHand"));
	GameplayTags.CombatSocket_Tail = GameplayTagsManager.AddNativeGameplayTag(
		FName("CombatSocket.Tail"),
		FString("Tail"));
	/*
	*Montage Tags
	*/

	GameplayTags.Montage_Attack1 = GameplayTagsManager.AddNativeGameplayTag(
		FName("Montage.Attack.1"),
		FString("Attack Montage 1"));

	GameplayTags.Montage_Attack2 = GameplayTagsManager.AddNativeGameplayTag(
		FName("Montage.Attack.2"),
		FString("Attack Montage 2"));

	GameplayTags.Montage_Attack3 = GameplayTagsManager.AddNativeGameplayTag(
		FName("Montage.Attack.3"),
		FString("Attack Montage 3"));

	GameplayTags.Montage_Attack4 = GameplayTagsManager.AddNativeGameplayTag(
		FName("Montage.Attack.4"),
		FString("Attack Montage 4"));


	/*
	 Player Tag
	 */
	GameplayTags.Player_Block_CursorTrace = GameplayTagsManager.AddNativeGameplayTag(
	FName("Player.Block.CursorTrace"),
	FString("Block Tracing under the cursor"));
	
	GameplayTags.Player_Block_InputPressed = GameplayTagsManager.AddNativeGameplayTag(
		FName("Player.Block.InputPressed"),
		FString("Block Input Pressed callback for input Input Held"));
	
	GameplayTags.Player_Block_InputReleased = GameplayTagsManager.AddNativeGameplayTag(
		FName("Player.Block.InputReleased"),
		FString("Block Input Released callback for input Input Released"));
	
	GameplayTags.Player_Block_InputHeld = GameplayTagsManager.AddNativeGameplayTag(
		FName("Player.Block.InputHeld"),
		FString("Block Input Held callback for input Input Held"));

	}
