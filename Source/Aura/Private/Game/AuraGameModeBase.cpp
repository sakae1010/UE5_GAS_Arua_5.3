// sakae's gas project


#include "Game/AuraGameModeBase.h"

#include "EngineUtils.h"
#include "Aura/AuraLogChannels.h"
#include "Game/AuraGameInstance.h"
#include "Game/LoadScreenSaveGame.h"
#include "GameFramework/Character.h"
#include "GameFramework/PlayerStart.h"
#include "Interaction/SaveInterface.h"
#include "Kismet/GameplayStatics.h"
#include "Serialization/ObjectAndNameAsStringProxyArchive.h"
#include "UI/ViewModel/MVVM_LoadSlot.h"
#include "GameFramework/Character.h"

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
	SaveGameObject->MapAssetName = LoadSlot->MapAssetName;
	
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

ULoadScreenSaveGame* AAuraGameModeBase::RetrieveInGameSaveData()
{
	UAuraGameInstance* AuraGameInstance = Cast<UAuraGameInstance>(GetGameInstance());
	const FString InGameSlotName = AuraGameInstance->LoadSlotName;
	const int32 InGameSlotIndex = AuraGameInstance->SlotIndex;
	return GetSlotData(InGameSlotName, InGameSlotIndex);
}

void AAuraGameModeBase::SaveIngameProgressData(ULoadScreenSaveGame* SaveObject)
{
	const FString InGameSlotName = SaveObject->SlotName;
	const int32 InGameSlotIndex = SaveObject->SlotIndex;
	UAuraGameInstance* AuraGameInstance = Cast<UAuraGameInstance>( GetGameInstance());
	AuraGameInstance->LoadSlotName = InGameSlotName;
	UGameplayStatics::SaveGameToSlot(SaveObject,InGameSlotName,InGameSlotIndex);
}

void AAuraGameModeBase::SaveWorldState(UWorld* World ,  const FString& DestinationMapAssetName) const
{
	if(!IsValid( World ))
	{
		return;
	}
	FString WorldName = World->GetMapName();
	WorldName.RemoveFromStart( World->StreamingLevelsPrefix );
	UAuraGameInstance* AuraGameInstance = Cast<UAuraGameInstance>(GetGameInstance());
	check( AuraGameInstance );
	const FString InGameSlotName = AuraGameInstance->LoadSlotName;
	const int32 InGameSlotIndex = AuraGameInstance->SlotIndex;

	if (ULoadScreenSaveGame* SaveGame = GetSlotData(InGameSlotName, InGameSlotIndex))
	{
		if (DestinationMapAssetName != FString(""))
		{
			SaveGame->MapAssetName = DestinationMapAssetName;
			SaveGame->MapName = GetMapManeFromMapAssetName(DestinationMapAssetName);
		}
		
		if (!SaveGame->HasMap( WorldName ))
		{
			FSaveMap SaveMap;
			SaveMap.MapName = WorldName;
			SaveGame->SaveMaps.Add(SaveMap);
		}

		FSaveMap SaveMap = SaveGame->GetSaveMapWithMapName( WorldName );
		SaveMap.SaveActors.Empty();
		for (TActorIterator<AActor> ActorItr(World); ActorItr; ++ActorItr)
		{
			AActor* Actor = *ActorItr;
			if (!IsValid(Actor) || !Actor->Implements<USaveInterface>()) continue;
			
			FSaveActor SaveActor;
			SaveActor.ActorName = Actor->GetFName();
			SaveActor.ActorTransform = Actor->GetActorTransform();

			// 記憶體寫入
			FMemoryWriter MemoryWriter(SaveActor.Bytes);
			// 進行序列化
			FObjectAndNameAsStringProxyArchive Archive(MemoryWriter, true);
			// 設定為儲存遊戲
			Archive.ArIsSaveGame = true;
			// 序列化actor
			Actor->Serialize(Archive);
			SaveMap.SaveActors.Add(SaveActor);
		}

		for(FSaveMap& SavedMap : SaveGame->SaveMaps)
		{
			if (SavedMap.MapName == WorldName)
			{
				SavedMap = SaveMap;
				break;
			}
		}
		UGameplayStatics::SaveGameToSlot(SaveGame, InGameSlotName, InGameSlotIndex);
	}
}

void AAuraGameModeBase::LoadWorldState(UWorld* World) const
{
	if(!World)
	{
		return;
	}
	FString WorldName = World->GetMapName();
	WorldName.RemoveFromStart( World->StreamingLevelsPrefix );
	UAuraGameInstance* AuraGameInstance = Cast<UAuraGameInstance>(GetGameInstance());
	check( AuraGameInstance );

	const FString InGameSlotName = AuraGameInstance->LoadSlotName;
	const int32 InGameSlotIndex = AuraGameInstance->SlotIndex;

	if (UGameplayStatics::DoesSaveGameExist( InGameSlotName , InGameSlotIndex))
	{
		ULoadScreenSaveGame* SaveGame = Cast<ULoadScreenSaveGame>(UGameplayStatics::LoadGameFromSlot( InGameSlotName , InGameSlotIndex ) );
		if (!SaveGame)
		{
			UE_LOG(LogAura, Error, TEXT("Faild to load save game from slot %s"), *InGameSlotName);
			return;
		}
		if (!SaveGame->HasMap( WorldName ))
		{
			UE_LOG(LogAura, Error, TEXT("Save game does not have map %s"), *WorldName);
			return;
		}
		FSaveMap SaveMap = SaveGame->GetSaveMapWithMapName( WorldName );
		
		for (TActorIterator<AActor> ActorItr(World); ActorItr; ++ActorItr)
		{
			AActor* Actor = *ActorItr;
			if (!IsValid(Actor) || !Actor->Implements<USaveInterface>()) continue;
			for (FSaveActor& SaveActor : SaveMap.SaveActors)
			{
				if (Actor->GetFName() == SaveActor.ActorName)
				{
					if (ISaveInterface::Execute_ShouldLoadTransform( Actor ))
					{
						Actor->SetActorTransform(SaveActor.ActorTransform);
					}
					
					// 記憶體讀取
					FMemoryReader MemoryReader(SaveActor.Bytes);
					// 進行反序列化
					FObjectAndNameAsStringProxyArchive Archive(MemoryReader, true);
					// 設定為讀取遊戲
					Archive.ArIsSaveGame = true;
					// 反序列化actor
					Actor->Serialize(Archive);

					ISaveInterface::Execute_LoadActor( Actor );
					break;
				}
			}
		}
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

FString AAuraGameModeBase::GetMapManeFromMapAssetName(const FString& MapAssetName) const
{
	for (const TPair<FString, TSoftObjectPtr<UWorld>>& Pair : Maps)
	{
		if (Pair.Value.ToSoftObjectPath().GetAssetFName() == MapAssetName)
		{
			return Pair.Key;
		}
	}
	return FString();
}



void AAuraGameModeBase::BeginPlay()
{
	Super::BeginPlay();
	Maps.Add(DefaultMapName, DefaultMap);
}

void AAuraGameModeBase::PlayerDie(ACharacter* DeadCharacter)
{
	ULoadScreenSaveGame* SaveGame = RetrieveInGameSaveData();
	if (!IsValid(SaveGame)) return;

	UGameplayStatics::OpenLevel(DeadCharacter, FName(SaveGame->MapAssetName));
}
