// sakae's gas project


#include "Game/AuraGameModeBase.h"

#include "Game/AuraGameInstance.h"
#include "Game/LoadScreenSaveGame.h"
#include "GameFramework/PlayerStart.h"
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
	SaveGameObject->PlayerName = LoadSlot->GetPlayerName();
	SaveGameObject->SaveSlotStatus = LoadSlot->SlotStatus;
	SaveGameObject->MapName = LoadSlot->GetMapName();
	SaveGameObject->PlayerStartTag = LoadSlot->PlayerStartTag;
	
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

void AAuraGameModeBase::TravelingToMap(UMVVM_LoadSlot* LoadSlot)
{
	const FString SlotName = LoadSlot->GetLoadSlotName();
	const int32 SlotIndex = LoadSlot->SlotIndex;
	
	UGameplayStatics::OpenLevelBySoftObjectPtr(LoadSlot,  Maps.FindChecked( LoadSlot->GetMapName() ));
	
}

AActor* AAuraGameModeBase::ChoosePlayerStart_Implementation(AController* Player)
{

	UAuraGameInstance* AuraGameInstance = Cast<UAuraGameInstance>(GetGameInstance() );
	
	TArray<AActor*> Actors;
	UGameplayStatics::GetAllActorsOfClass(GetWorld(), APlayerStart::StaticClass(), Actors);
	if(Actors.Num() > 0)
	{
		AActor* SelectActor = Actors [ 0 ];
		for (AActor* Actor : Actors)
		{
			if(const APlayerStart* PlayerStart = Cast<APlayerStart>(Actor))
			{
				if( PlayerStart->PlayerStartTag == AuraGameInstance->PlayerStartTag)
				{
					return  SelectActor = Actor;
				}
			}
		}
		return SelectActor;
	}
	return nullptr;
}

void AAuraGameModeBase::BeginPlay()
{
	Super::BeginPlay();
	Maps.Add(DefaultMapName, DefaultMap);
}

