// sakae's gas project

#pragma once

#include "CoreMinimal.h"
#include "Engine/DataAsset.h"
#include "CharacterClassInfo.generated.h"
class UGameplayAbility;

UENUM(BlueprintType)
enum class ECharacterClass : uint8
{
	Eleentalist,
	Warrior,
	Ranger,
};

USTRUCT()
struct FCharacterClassDefaultInfo
{
	GENERATED_BODY()

	UPROPERTY(EditDefaultsOnly, Category = "CharacterClass")
	TSubclassOf<UGameplayAbility> PrimaryAttributes;
};

/**
 * 
 */
UCLASS()
class AURA_API UCharacterClassInfo : public UDataAsset
{
	GENERATED_BODY()
	
	UPROPERTY(EditDefaultsOnly, Category = "Character Class Defaults")
	TMap<ECharacterClass,FCharacterClassDefaultInfo> CharacterClassInformation;

	UPROPERTY(EditDefaultsOnly, Category = "Common Class Defaults")
	TSubclassOf<UGameplayAbility> SeconderAttributes;

	UPROPERTY(EditDefaultsOnly, Category = "Common Class Defaults")
	TSubclassOf<UGameplayAbility> VitalAttributes;


	FCharacterClassDefaultInfo GetCharacterClassInfo(ECharacterClass CharacterClass);
};
