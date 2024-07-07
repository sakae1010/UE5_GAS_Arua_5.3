// sakae's gas project

#pragma once

#include "CoreMinimal.h"
#include "GameplayTagContainer.h"
#include "Engine/DataAsset.h"
#include "AbilityInfo.generated.h"

class UGameplayAbility;

USTRUCT(Blueprintable)
struct FAuraAbilityInfo
{
	GENERATED_BODY()

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "AbilityInfo")
	FGameplayTag AbilityTag = FGameplayTag::EmptyTag;
	
	UPROPERTY(BlueprintReadOnly, Category = "AbilityInfo")
	FGameplayTag InputTag = FGameplayTag::EmptyTag;

	UPROPERTY(BlueprintReadOnly, Category = "AbilityInfo")
	FGameplayTag StatusTag = FGameplayTag::EmptyTag;

	UPROPERTY(EditDefaultsOnly ,BlueprintReadOnly, Category = "AbilityInfo")
	FGameplayTag CooldownTag = FGameplayTag::EmptyTag;
	
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "AbilityInfo")
	TObjectPtr<const UTexture2D> Icon = nullptr;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "AbilityInfo")
	TObjectPtr<const UMaterialInstance> BackgrounMaterial = nullptr;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "AbilityInfo")
	int32 LevelRequirement = 1;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "AbilityInfo")
	TSubclassOf<UGameplayAbility> Ability = nullptr;
	
};

/**
 * 
 */
UCLASS()
class AURA_API UAbilityInfo : public UDataAsset
{
	GENERATED_BODY()

public:
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "AbilityInfo")
	TArray<FAuraAbilityInfo> AuraAbilityInformations;

	FAuraAbilityInfo FindAbilityInfForTag(const FGameplayTag InAbilityTag, bool bLogNotFund = false) const;
};
