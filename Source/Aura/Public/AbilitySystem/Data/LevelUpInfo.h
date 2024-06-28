// sakae's gas project

#pragma once

#include "CoreMinimal.h"
#include "Engine/DataAsset.h"
#include "LevelUpInfo.generated.h"


USTRUCT(BlueprintType)
struct FAuraLevelUpInfo
{
	GENERATED_BODY()

	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "Level Award")
	int32 Exp;

	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "Level Award")
	int32 AttributePointAward = 1;

	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "Level Award")
	int32 SpellPointAward = 1;
};
/**
 * 
 */
UCLASS()
class AURA_API ULevelUpInfo : public UDataAsset
{
	GENERATED_BODY()
public:
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "LevelUpInfo")
	TArray<FAuraLevelUpInfo> LevelUpInfos;

	int32 FindLevelForXP(int32 InXP) const;
	
};
