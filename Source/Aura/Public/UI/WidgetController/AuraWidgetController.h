// sakae's gas project

#pragma once

#include "CoreMinimal.h"
#include "AbilitySystemComponent.h"
#include "UObject/NoExportTypes.h"
#include "AuraWidgetController.generated.h"

class AAuraPlayerController;
class UAuraAttributeSet;
class UAuraAbilitySystemComponent;
class AAuraPlayerState;
class UAbilityInfo;
class APlayerController;
class APlayerState;
class UAbilitySystemComponent;

class UAttributeSet;

USTRUCT(BlueprintType)
struct FWidgetControllerParams
{
	GENERATED_BODY()
	FWidgetControllerParams(){}
	FWidgetControllerParams(APlayerController* InPlayerController, APlayerState* InPlayerState, UAbilitySystemComponent* InAbilitySystemComponent, UAttributeSet* InAttributeSet)
	:	PlayerController(InPlayerController),
		PlayerState(InPlayerState),
		AbilitySystemComponent(InAbilitySystemComponent),
		AttributeSet(InAttributeSet)
	{}

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	TObjectPtr<APlayerController> PlayerController = nullptr;
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	TObjectPtr<APlayerState> PlayerState = nullptr;
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	TObjectPtr<UAbilitySystemComponent> AbilitySystemComponent = nullptr;
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	TObjectPtr<UAttributeSet> AttributeSet = nullptr;
	

};

DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FOnPlayerStateChangedSignature, int, NewValue);
DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FAbiliityInfoSignature, const FAuraAbilityInfo&, AbilityInfo);


/**
 * 
 */
UCLASS()
class AURA_API UAuraWidgetController : public UObject
{
	GENERATED_BODY()
public:
	UFUNCTION(BlueprintCallable, Category = "WidgetController")
	void SetWidgetControllerParams(const FWidgetControllerParams& InParams);\
	
	UFUNCTION(BlueprintCallable)
	virtual void BroadcastInitValues() ;
	virtual void BindCallBacksToDependencies();

	UPROPERTY(BlueprintAssignable, Category = "GAS|Messages")
	FAbiliityInfoSignature AbilityInfoDelegate;

	void BroadcastAbilityInfo();
protected:
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Widget Data")
	TObjectPtr<UAbilityInfo> AbilityWidgetDataTable;

	
	UPROPERTY(BlueprintReadOnly, Category = "WidgetController")
	TObjectPtr<APlayerController> PlayerController;
	
	UPROPERTY(BlueprintReadOnly, Category = "WidgetController")
	TObjectPtr<APlayerState> PlayerState;
	
	UPROPERTY(BlueprintReadOnly, Category = "WidgetController")
	TObjectPtr<UAbilitySystemComponent> AbilitySystemComponent;
	
	UPROPERTY(BlueprintReadOnly, Category = "WidgetController")
	TObjectPtr<UAttributeSet> AttributeSet;

	UPROPERTY(BlueprintReadOnly, Category = "WidgetController")
	TObjectPtr<AAuraPlayerController> AuraPlayerController;
	
	UPROPERTY(BlueprintReadOnly, Category = "WidgetController")
	TObjectPtr<AAuraPlayerState> AuraPlayerState;
	
	UPROPERTY(BlueprintReadOnly, Category = "WidgetController")
	TObjectPtr<UAuraAbilitySystemComponent> AuraAbilitySystemComponent;
	
	UPROPERTY(BlueprintReadOnly, Category = "WidgetController")
	TObjectPtr<UAuraAttributeSet> AuraAttributeSet;

	AAuraPlayerController* GetAuraPlayerController() ;
	AAuraPlayerState* GetAuraPlayerState() ;
	UAuraAbilitySystemComponent* GetAuraAbilitySystemComponent() ;
	UAuraAttributeSet* GetAuraAttributeSet() ;
	
	
};
