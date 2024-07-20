// sakae's gas project

#pragma once

#include "CoreMinimal.h"
#include "AbilitySystemComponent.h"
#include "AuraAbilitySystemComponent.generated.h"
class UAuraAbilitySystemComponent;
DECLARE_MULTICAST_DELEGATE_OneParam(FEffectAssetTags, const FGameplayTagContainer& /*AssetTags*/);
DECLARE_MULTICAST_DELEGATE(FAbilitiesGiven /*AbilitySystemComponent*/);
DECLARE_DELEGATE_OneParam(FForEachAbility, const FGameplayAbilitySpec&);
DECLARE_MULTICAST_DELEGATE_ThreeParams(FAbilityStatusChanged ,const  FGameplayTag& /* Ability Tag*/ ,const FGameplayTag& /* Ability Tag*/  , int32 /* AbilityLevel*/);
DECLARE_MULTICAST_DELEGATE_FourParams(FAbilityEquiped ,const  FGameplayTag& /* Ability Tag*/ ,const FGameplayTag& /* Status Tag*/  ,const FGameplayTag& /* Slot Tag*/  ,const FGameplayTag& /* PreSlot Tag*/);
/**
 * 
 */
UCLASS()
class AURA_API UAuraAbilitySystemComponent : public UAbilitySystemComponent
{
	GENERATED_BODY()
public:
	void AbilityActorInfoInit();

	FEffectAssetTags EffectAssetTags;
	FAbilitiesGiven AbilitiesGivenDelegate;
	FAbilityStatusChanged AbilityStatusChangedDelegate;
	FAbilityEquiped AbilityEquipedDelegate;
	bool bStartupAbilitiesGiven = false;
	
	void AddCharacterAbilities(TArray<TSubclassOf<UGameplayAbility>>& InAbilities);
	void AddCharacterPassvieAbilities(TArray<TSubclassOf<UGameplayAbility>>& InPassvieAbilities);
	void AbilityInputHeld(const FGameplayTag& InputTag);
	void AbilityInputReleased(const FGameplayTag& InputTag);
	void ForEachAbility(const FForEachAbility& Delegate);
    static FGameplayTag GetAbilityFromSpec(const FGameplayAbilitySpec& AbilitySpec);
	static FGameplayTag GetInputTagFromSpec(const FGameplayAbilitySpec& AbilitySpec);
	static FGameplayTag GetStatusTagFromSpec(const FGameplayAbilitySpec& AbilitySpec);
	FGameplayTag GetStatusTagFromAbilityTag(const FGameplayTag& AbilityTag);
	FGameplayTag GetInputTagFromAbilityTag(const FGameplayTag& AbilityTag);
	//取得已啟動能力
	FGameplayAbilitySpec* GetSpecFormAbilityTag(const FGameplayTag& AbilityTag);
	void UpgradeAttribute(const FGameplayTag& Tag);

	UFUNCTION(Server,Reliable)
	void ServerUpgradeAttribute(const FGameplayTag& AttributeTag);

	//檢查等級 條件通過就給與技能 尚未啟動狀態
	void UpdateAbilityStatuses(int32 Level);

	UFUNCTION(Server,Reliable)
	void ServerSpendSpellPoint(const FGameplayTag& AbilityTag);

	UFUNCTION(Server,Reliable)
	void ServerEquipAbility(const FGameplayTag& AbilityTag , const FGameplayTag& Slot);
	void ClientEquipAbility(const FGameplayTag& AbilityTag ,const FGameplayTag& StatusTag ,const FGameplayTag& Slot , const FGameplayTag& PreviousSlot);
	bool GetDescriptionsByAbilityTag(const FGameplayTag& AbilityTag , FString& OutDescription, FString& OutNextLevelDescription);

	void ClearSlot(FGameplayAbilitySpec* AbilitySpec);
	void ClearAbilityOfSlot(const FGameplayTag& Slot);
	

	static bool AbilityHasSlot(FGameplayAbilitySpec* AbilitySpec ,const FGameplayTag& Slot);
protected:
	virtual void OnRep_ActivateAbilities() override;	
	UFUNCTION(Client,Reliable)
	void ClientEffectApplied(UAbilitySystemComponent* AbilitySystemComponent ,  const FGameplayEffectSpec& Spec, FActiveGameplayEffectHandle Handle) const;
	
	UFUNCTION(Client,Reliable)
	void ClientUpdateAbilityStatus(const FGameplayTag& AbilityTag , const FGameplayTag& StatusTag , int32 AbiltyLevel);
};

