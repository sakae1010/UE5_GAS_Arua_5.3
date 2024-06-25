// sakae's gas project

#pragma once

#include "CoreMinimal.h"
#include "GameplayTagContainer.h"
#include "Engine/DataAsset.h"
#include "AbilityInfo.generated.h"

USTRUCT(Blueprintable)
struct FAuraAbilityInfo
{
	GENERATED_BODY()

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "AbilityInfo")
	FGameplayTag AbilityTag = FGameplayTag::EmptyTag;
	
	UPROPERTY(BlueprintReadOnly, Category = "AbilityInfo")
	FGameplayTag InputTag = FGameplayTag::EmptyTag;
	
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "AbilityInfo")
	TObjectPtr<const UTexture2D> Icon = nullptr;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "AbilityInfo")
	TObjectPtr<const UMaterialInstance> BackgrounMaterial = nullptr;
	
	
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
