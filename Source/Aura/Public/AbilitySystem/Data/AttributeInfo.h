// sakae's gas project

#pragma once

#include "CoreMinimal.h"
#include "GameplayTagContainer.h"
#include "Engine/DataAsset.h"
#include "AttributeInfo.generated.h"


USTRUCT(BlueprintType)
struct FAuraAttributeInfo
{
	GENERATED_BODY()
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "AttributeInfo")
	FGameplayTag AttributeTag;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "AttributeInfo")
	FText AttributeName = FText();

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "AttributeInfo")
	FText AttributeDescription = FText();

	UPROPERTY(BlueprintReadOnly, Category = "AttributeInfo")
	float AttributeValue = 0.0f;
};

/**
 * 
 */
UCLASS()
class AURA_API UAttributeInfo : public UDataAsset
{
	GENERATED_BODY()

public:
	FAuraAttributeInfo FindAttributeInfo( const FGameplayTag InAttributeTag , bool  bLogNotFund  = false);
	
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "AttributeInfo")
	TArray<FAuraAttributeInfo> AttributeInfos;
};
