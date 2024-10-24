﻿// sakae's gas project

#pragma once

#include "CoreMinimal.h"
#include "AbilitySystem/Data/AttributeInfo.h"
#include "AuraWidgetController.h"
#include "AttributeMenuWidgetController.generated.h"

struct FAuraAttributeInfo;
class UAttributeInfo;
struct FGameplayTag;
DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FAttributeInfoSingnture,const  FAuraAttributeInfo, Info);
/**
 * 
 */
UCLASS(Blueprintable, BlueprintType)
class AURA_API UAttributeMenuWidgetController : public UAuraWidgetController
{
	GENERATED_BODY()
public:
	UFUNCTION(BlueprintCallable)
	virtual void BroadcastInitValues() override;
	virtual void BindCallBacksToDependencies() override;

	UPROPERTY(BlueprintAssignable, Category = "AttributeMenuWidgetController")
	FAttributeInfoSingnture AttributeInfoDelegate;
	
	UPROPERTY(BlueprintAssignable, Category = "GAS|Player|AttributePoints")
	FOnPlayerStateChangedSignature OnAttributePointsChanged;
	
	UFUNCTION(BlueprintCallable)
	void UpgradeAttribute(const FGameplayTag& Tag);
	
protected:
	UPROPERTY(EditDefaultsOnly)
	TObjectPtr<UAttributeInfo> AttributeInfo;

private:
	void BroadcastAttributeInfo(const FGameplayTag& Tag, const FGameplayAttribute& Attribute) const;
};
