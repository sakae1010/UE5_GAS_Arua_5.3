// sakae's gas project


#include "Game/AuraGameModeBase.h"

#include "Game/LoadScreenSaveGame.h"
#include "Kismet/GameplayStatics.h"
#include "UI/ViewModel/MVVM_LoadSlot.h"

void AAuraGameModeBase::SaveSlotData(UMVVM_LoadSlot* LoadSlot, int32 SlotIndex)
{
	if( UGameplayStatics::DoesSaveGameExist( LoadSlot->GetLoadSlotName(), SlotIndex))
	{
		UGameplayStatics::DeleteGameInSlot(LoadSlot->GetLoadSlotName(), SlotIndex);
	}

	ULoadScreenSaveGame* SaveGameObject = Cast<ULoadScreenSaveGame>(UGameplayStatics::CreateSaveGameObject(LoadScreenSaveGameClass) );
	SaveGameObject->SlotName = LoadSlot->GetLoadSlotName();
	SaveGameObject->SlotIndex = SlotIndex;
	SaveGameObject->PlayerName = LoadSlot->GetPlayerName();
	SaveGameObject->SaveSlotStatus = LoadSlot->SlotStatus;
	
	UGameplayStatics::SaveGameToSlot(SaveGameObject, SaveGameObject->SlotName, SaveGameObject->SlotIndex);
}

ULoadScreenSaveGame* AAuraGameModeBase::GetSlotData(const FString LoadSlotName, int32 SlotIndex) const
{
	USaveGame* SaveGameObject;
	if( UGameplayStatics::DoesSaveGameExist( LoadSlotName, SlotIndex))
	{
		SaveGameObject = UGameplayStatics::LoadGameFromSlot( LoadSlotName, SlotIndex);
	}else
	{
		SaveGameObject = UGameplayStatics::CreateSaveGameObject(LoadScreenSaveGameClass);
	}
	ULoadScreenSaveGame* LoadScreenSaveGame = Cast<ULoadScreenSaveGame>( SaveGameObject );
	return LoadScreenSaveGame;
}

void AAuraGameModeBase::DeleteSlotData(const FString& LoadSlotName, const int32 SlotIndex)
{
	if( UGameplayStatics::DoesSaveGameExist( LoadSlotName, SlotIndex))
	{
		UGameplayStatics::DeleteGameInSlot(LoadSlotName, SlotIndex);
	}
}

void AAuraGameModeBase::BeginPlay()
{
	Super::BeginPlay();
	Maps.Add(DefaultMapName, DefaultMap);
}

