// sakae's gas project

#pragma once

#include "CoreMinimal.h"
#include "GameplayTagContainer.h"
#include "GameFramework/SaveGame.h"
#include "LoadScreenSaveGame.generated.h"


class UGameplayAbility;

UENUM(Blueprintable)
enum ESaveSlotStatus
{
	Vacant,
	EnterName,
	Taken
};

USTRUCT()
struct FSaveActor
{
	GENERATED_BODY()
	
	UPROPERTY()
	FString ActorName = FString();

	UPROPERTY()
	FString ActorClass = FString();

	UPROPERTY()
	FTransform ActorTransform = FTransform();
	
	// Serialize vaiable from the actor - only those marked with SaveGame specifier
	// 從actor序列化變數 - 只有標記有SaveGame的變數
	UPROPERTY()
	TArray<uint8> Bytes;
	
	
};


inline bool operator==(const FSaveActor& Left, const FSaveActor& Right)
{
	return Left.ActorName == Right.ActorName;
}
USTRUCT()
struct FSaveMap
{
	GENERATED_BODY()

	UPROPERTY()
	FString MapName = FString();
	
	UPROPERTY()
	TArray<FSaveActor> SaveActors;
};


USTRUCT(BlueprintType)
struct FSaveAbility
{
	GENERATED_BODY()

	UPROPERTY(EditDefaultsOnly , BlueprintReadOnly , Category = "AbilityClass")
	TSubclassOf<UGameplayAbility> GameplayAbility;
	
	UPROPERTY(EditDefaultsOnly , BlueprintReadOnly)
	FGameplayTag AbilityTag = FGameplayTag();

	UPROPERTY(EditDefaultsOnly , BlueprintReadOnly)
	FGameplayTag AbilityStatus = FGameplayTag();

	UPROPERTY(EditDefaultsOnly , BlueprintReadOnly)
	FGameplayTag AbilitySlot = FGameplayTag();
	
	UPROPERTY(EditDefaultsOnly , BlueprintReadOnly)
	FGameplayTag AbilityType = FGameplayTag();
	
	UPROPERTY(EditDefaultsOnly , BlueprintReadOnly)
	FGameplayTag AbilityInputTag = FGameplayTag();
	
	UPROPERTY(EditDefaultsOnly , BlueprintReadOnly)
	int AbilityLevel = 1;
	
};

inline bool operator==(const FSaveAbility& LHS, const FSaveAbility& RHS)
{
	return LHS.AbilityTag.MatchesTagExact( RHS.AbilityTag );
}

/**
 * 
 */
UCLASS()
class AURA_API ULoadScreenSaveGame : public USaveGame
{
	GENERATED_BODY()

public:
	UPROPERTY()
	FString SlotName = FString();
	UPROPERTY()
	int32 SlotIndex = 0;

	UPROPERTY()
	FString PlayerName = FString("Default Player Name");
	
	UPROPERTY()
	FString MapName = FString("Default Map Name");

	UPROPERTY()
	FName PlayerStartTag = FName();

	UPROPERTY()
	TEnumAsByte<ESaveSlotStatus> SaveSlotStatus = Vacant;

	UPROPERTY()
	bool bFirstTimeLoadInGame = true;
	
	/* Player */
	UPROPERTY()
	int32 PlayerLevel = 1;
	
	UPROPERTY()
	int32 XP = 0;

	UPROPERTY()
	int32 SpellPoints = 0;
	
	UPROPERTY()
	int32 AttributePoints = 0;

	// player attributes primary
	UPROPERTY()
	float Strength = 0;

	UPROPERTY()
	float Intelligence = 0;

	UPROPERTY()
	float Resilience = 0;
	
	UPROPERTY()
	float Vigor = 0;

	/* player abilities*/
	UPROPERTY()
	TArray<FSaveAbility> SaveAbilityList;

	UPROPERTY()
	TArray<FSaveMap> SaveMaps;


	FSaveMap GetSaveMapWithMapName(const FString& InMapName);
	bool HasMap(const FString& InMapName);
	// FSaveActor* GetSaveActor(const FString& InActorName , FSaveMap& InMap);
};
