// sakae's gas project

#pragma once

#include "CoreMinimal.h"
#include "AbilitySystem/Abilities/AuraGameAbility.h"
#include "AuraSummonAbility.generated.h"

/**
 * 
 */
UCLASS()
class AURA_API UAuraSummonAbility : public UAuraGameAbility
{
	GENERATED_BODY()

public:

	UFUNCTION(BlueprintCallable, Category = "SummonIng")
	TArray<FVector> GetSpawnLocations();

	UPROPERTY(EditDefaultsOnly, Category = "SummonIng")
	int32 NumMinionsToSpawn = 1;

	UPROPERTY(EditDefaultsOnly, Category = "SummonIng")
	TArray<TSubclassOf<APawn>> MinionClasses;

	UPROPERTY(EditDefaultsOnly, Category = "SummonIng")
	float MinSpawnDistance = 50.0f;
	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "SummonIng")
	float MaxSpawnDistance = 250.0f;

	//spread 翻譯為散佈
	UPROPERTY(EditDefaultsOnly, Category = "SummonIng")
	float SpawnSpread = 90.0f;
};
