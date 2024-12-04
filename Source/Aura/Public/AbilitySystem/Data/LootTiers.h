// sakae's gas project

#pragma once

#include "CoreMinimal.h"

#include "LootTiers.generated.h"

USTRUCT(BlueprintType)
struct FLootItem
{
	GENERATED_BODY()
	
	UPROPERTY(EditAnywhere,BlueprintReadOnly, Category = "Loot")
	TSubclassOf<AActor> LootClass;

	UPROPERTY(EditAnywhere, Category = "Loot")
	float ChanceToSpawn = 0.f;
	
	UPROPERTY(EditAnywhere, Category = "Loot")
	int32 MaxNumberToSpawn = 1;
	
	UPROPERTY(EditAnywhere,BlueprintReadOnly, Category = "Loot")
	bool bLootLevelOverride = true;
};


/**
 * 
 */
UCLASS()
class AURA_API ULootTiers : public UDataAsset
{
	GENERATED_BODY()


public:

	UFUNCTION(BlueprintCallable, Category = "Loot")
	TArray<FLootItem> GetLootItems() ;
	
	UPROPERTY(EditDefaultsOnly, Category = "Loot")
	TArray<FLootItem> LootItems;
};
