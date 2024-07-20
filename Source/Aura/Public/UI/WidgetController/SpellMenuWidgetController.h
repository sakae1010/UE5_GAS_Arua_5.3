// sakae's gas project

#pragma once

#include "CoreMinimal.h"
#include "AuraGameplayTag.h"
#include "AuraWidgetController.h"
#include "SpellMenuWidgetController.generated.h"
struct FAuraGameplayTags;
DECLARE_DYNAMIC_MULTICAST_DELEGATE_FourParams(FSpellGlobeSelectSignature, bool, bSpendPointsButtonEnabled, bool, bEquipButtonEnabled ,FString, Description, FString, NextLevelDescription);
DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FWaitForEquipSelectionSignature , const FGameplayTag& , AbilityType);
DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FSpellGlobeReassignedSignature , const FGameplayTag& , AbilityTag);

struct FSelectAbilty
{
	FGameplayTag AbilityTag = FGameplayTag();
	FGameplayTag StatusTag = FGameplayTag();
};

/**
 * 
 */
UCLASS(BlueprintType, Blueprintable)
class AURA_API USpellMenuWidgetController : public UAuraWidgetController
{
	GENERATED_BODY()
public:
	UFUNCTION(BlueprintCallable)
	virtual void BroadcastInitValues() override ;
	virtual void BindCallBacksToDependencies() override ;

	UPROPERTY(BlueprintAssignable, Category = "GAS|Player|AttributePoints")
	FOnPlayerStateChangedSignature OnSpellPointChanged;
	
	UPROPERTY(BlueprintAssignable)
	FSpellGlobeSelectSignature SpellGlobeSelectDelegate;
	
	UPROPERTY(BlueprintAssignable)
	FWaitForEquipSelectionSignature WaitForEquipSelectionDelegate;
	
	UPROPERTY(BlueprintAssignable)
	FWaitForEquipSelectionSignature StopWaittingForEquipDelegate;

	UPROPERTY(BlueprintAssignable)
	FSpellGlobeReassignedSignature SpellGlobeReassignedDelegate;
	
	UFUNCTION(BlueprintCallable)
	void SpellGlobeSelected(const FGameplayTag& AbilityTag);

	UFUNCTION(BlueprintCallable)
	void SpendButtonClick();
	UFUNCTION(BlueprintCallable)
	void GlobeDeselect();
	UFUNCTION(BlueprintCallable)
	void EquipButtonPressed();
	UFUNCTION(BlueprintCallable)
	void SpellRowGlobePressed(const FGameplayTag& SlotTag , const FGameplayTag& AbilityType);

	void OnAbilityEquiped(const FGameplayTag& AbilityTag, const FGameplayTag& StatusTag,const FGameplayTag& Slot, const FGameplayTag& PreviousSlot);
private:
	static void ShouldEnableButtons(const FGameplayTag& StatusTag ,const int32 SpellPoints ,bool& bEnableSpellPointButton , bool& bEnableEquipButton);

	FSelectAbilty SelectAbilty = { FAuraGameplayTags::Get().Abilities_None, FAuraGameplayTags::Get().Abilities_Status_Unlocked };
	int32 CurrentSpellPoints = 0;
	
	void ChangeButtonState(const FGameplayTag AbilityTag ,const FGameplayTag StatusTag ,const int32 SpellPoints );

	bool bWaitingForEquipSelection = false;

	FGameplayTag SelectSlot;
	
};
