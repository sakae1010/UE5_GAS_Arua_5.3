// sakae's gas project

#pragma once

#include "CoreMinimal.h"
#include "Kismet/BlueprintAsyncActionBase.h"
#include "GameplayTagContainer.h"
#include "WaitCooldownChange.generated.h"

struct FActiveGameplayEffectHandle;
struct FGameplayEffectSpec;
struct FGameplayTag;
class UAbilitySystemComponent;
DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FCooldownChangeSignature, float, TimeRemaining);


/**
 * 
 */
UCLASS()
class AURA_API UWaitCooldownChange : public UBlueprintAsyncActionBase
{
	GENERATED_BODY()
public:
	// UPROPERTY(BlueprintAssignable)
	// FCooldownChangeSignature OnCooldownChange;
	UPROPERTY(BlueprintAssignable)
	FCooldownChangeSignature OnCooldownStart;
	UPROPERTY(BlueprintAssignable)
	FCooldownChangeSignature OnCooldownEnd;

	UFUNCTION(BlueprintCallable ,meta = (BlueprintInternalUseOnly = "true"))
	static UWaitCooldownChange* WaitCooldownChange(UAbilitySystemComponent* AbilitySystemComponent, const FGameplayTag InCooldownTags);
	
	UFUNCTION(BlueprintCallable)
	void EndTask();

protected:
	UPROPERTY()	
	TObjectPtr<UAbilitySystemComponent> ASC;

	FGameplayTag CooldownTag;

	void CooldownTagChange(const FGameplayTag InCooldownTag, int32 NewCount);

	void OnActiveEffectAdded( UAbilitySystemComponent* TargetAsc, const FGameplayEffectSpec& SpecApplied, FActiveGameplayEffectHandle ActiveHandle);
};
