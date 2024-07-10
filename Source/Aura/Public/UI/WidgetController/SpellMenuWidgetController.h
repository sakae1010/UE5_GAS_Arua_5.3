// sakae's gas project

#pragma once

#include "CoreMinimal.h"
#include "AuraWidgetController.h"
#include "SpellMenuWidgetController.generated.h"
DECLARE_DYNAMIC_MULTICAST_DELEGATE_TwoParams(FSpellGlobeSelectSignature, bool ,bSpendPointsButtonEnabled , bool, bEquipButtonEnabled);
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
	
	UFUNCTION(BlueprintCallable)
	void SpellGlobeSelected(const FGameplayTag& AbilityTag);
private:
	static void ShouldEnableButtons(const FGameplayTag& StatusTag ,const int32 SpellPoints ,bool& bEnableSpellPointButton , bool& bEnableEquipButton);
	
};
