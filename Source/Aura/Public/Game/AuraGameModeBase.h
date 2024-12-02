// sakae's gas project

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/GameModeBase.h"
#include "AuraGameModeBase.generated.h"

class ULoadScreenSaveGame;
class USaveGame;
class UMVVM_LoadSlot;
class UAbilityInfo;
class UCharacterClassInfo;

/**
 * 
 */
UCLASS()
class AURA_API AAuraGameModeBase : public AGameModeBase
{
	GENERATED_BODY()

public:
	UPROPERTY(EditDefaultsOnly, Category = "Character Class Defaults")
	TObjectPtr<UCharacterClassInfo> CharacterClassInfo;
	
	UPROPERTY(EditDefaultsOnly, Category = "Ability Info")
	TObjectPtr<UAbilityInfo> AbilityInfo;

	void SaveSlotData(UMVVM_LoadSlot* LoadSlot,int32 SlotIndex);
	ULoadScreenSaveGame* GetSlotData(const FString LoadSlotName,int32 SlotIndex) const;
	static void DeleteSlotData(const FString& LoadSlotName, const int32 SlotIndex);
	ULoadScreenSaveGame* RetrieveInGameSaveData();
	void SaveIngameProgressData(ULoadScreenSaveGame* SaveObject);

	// UFUNCTION()
	void SaveWorldState(UWorld* World , const FString& DestinationMapAssetName = FString("")) const;
	//UFUNCTION()
	void LoadWorldState(UWorld* World) const;
	
	void TravelingToMap(UMVVM_LoadSlot* LoadSlot);
	
	UPROPERTY(EditDefaultsOnly, Category = "Save Game")
	TSubclassOf<USaveGame> LoadScreenSaveGameClass;

	UPROPERTY(EditDefaultsOnly)
	FString DefaultMapName ;
	
	UPROPERTY(EditDefaultsOnly)
	TSoftObjectPtr<UWorld> DefaultMap;
	
	UPROPERTY(EditDefaultsOnly)
	FName DefaultPlayerStartTag ;
	
	UPROPERTY(EditDefaultsOnly)
	TMap<FString,TSoftObjectPtr<UWorld>> Maps;

	virtual AActor* ChoosePlayerStart_Implementation(AController* Player) override;

	FString GetMapManeFromMapAssetName(const FString& MapAssetName) const;

	void PlayerDie(ACharacter* DeadCharacter);
	
protected:
	virtual void BeginPlay() override;
};
