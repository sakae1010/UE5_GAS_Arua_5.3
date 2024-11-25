// sakae's gas project


#include "UI/ViewModel/MVVM_LoadScreen.h"

#include "Game/AuraGameInstance.h"
#include "Game/AuraGameModeBase.h"
#include "Kismet/GameplayStatics.h"
#include "UI/ViewModel/MVVM_LoadSlot.h"

void UMVVM_LoadScreen::InitializeLoadSlots()
{
	LoadSlot_0 = NewObject<UMVVM_LoadSlot>(this, LoadSlotViewModelClass);
	LoadSlot_0->SetLoadSlotName(FString("LoadSlot_0"));
	LoadSlot_0->SlotIndex = 0;
	LoadSlots.Add(0, LoadSlot_0);
	LoadSlot_1 = NewObject<UMVVM_LoadSlot>(this, LoadSlotViewModelClass);
	LoadSlot_1->SetLoadSlotName(FString("LoadSlot_1"));
	LoadSlot_1->SlotIndex = 1;
	LoadSlots.Add(1, LoadSlot_1);
	LoadSlot_2 = NewObject<UMVVM_LoadSlot>(this, LoadSlotViewModelClass);
	LoadSlot_2->SetLoadSlotName(FString("LoadSlot_2"));
	LoadSlot_2->SlotIndex = 2;
	LoadSlots.Add(2, LoadSlot_2);

	SetNumLoadSlots( LoadSlots.Num() );
}

UMVVM_LoadSlot* UMVVM_LoadScreen::GetLoadSlotViewModelByIndex(int32 Index) const
{
	return LoadSlots.FindChecked(Index);
}

void UMVVM_LoadScreen::NewSlotButtonPressed(int32 SlotIndex, const FString& EnterName)
{
	AAuraGameModeBase* AuraGameMode = Cast<AAuraGameModeBase>(UGameplayStatics::GetGameMode(this)  ) ;
	UAuraGameInstance* AuraGameInstance = Cast<UAuraGameInstance>( AuraGameMode->GetGameInstance() );
	LoadSlots[SlotIndex]->SetPlayerName( EnterName );
	LoadSlots[SlotIndex]->SlotStatus = Taken;
	LoadSlots[SlotIndex]->SetMapName(AuraGameMode->DefaultMapName);
	LoadSlots[SlotIndex]->PlayerStartTag = FName( AuraGameMode->DefaultPlayerStartTag);
	
	AuraGameMode->SaveSlotData(LoadSlots[SlotIndex], SlotIndex);
	LoadSlots[SlotIndex]->InitializeSlot();

	AuraGameInstance->SlotIndex = SlotIndex;
	AuraGameInstance->LoadSlotName = LoadSlots[SlotIndex]->GetLoadSlotName();
	AuraGameInstance->PlayerStartTag =  AuraGameMode->DefaultPlayerStartTag;
	
}

void UMVVM_LoadScreen::NewGameButtonPressed(int32 SlotIndex)
{
	LoadSlots[SlotIndex]->SetPlayerName( FString() );
	LoadSlots[SlotIndex]->SetWidgetSwitchIndex.Broadcast(1);
}

void UMVVM_LoadScreen::SelectSlotButtonPressed(int32 SlotIndex)
{
	for ( TTuple<int, UMVVM_LoadSlot*> LoadSlot : LoadSlots)
	{
		LoadSlot.Value->EnableSelectSlotButton.Broadcast(LoadSlot.Key != SlotIndex);
	}
	SlotSelected.Broadcast();
	SelectedSlot = LoadSlots[SlotIndex];
}

void UMVVM_LoadScreen::DeleteSlotButtonPressed()
{
	if(IsValid(SelectedSlot))
	{
		AAuraGameModeBase::DeleteSlotData(SelectedSlot->GetLoadSlotName() ,SelectedSlot->SlotIndex);
		SelectedSlot->SetPlayerName(FString());
		SelectedSlot->SlotStatus = Vacant;
		SelectedSlot->InitializeSlot();
		SelectedSlot->EnableSelectSlotButton.Broadcast(true	);
		SelectedSlot = nullptr;
		
	}
}

void UMVVM_LoadScreen::PlayButtonPressed()
{
	if(!IsValid(  SelectedSlot ))
	{
		return;
	}
	if (SelectedSlot->SlotStatus != Taken)
	{
		return;
	}
	AAuraGameModeBase* AuraGameModeBase = Cast<AAuraGameModeBase>(UGameplayStatics::GetGameMode(this)) ;
	UAuraGameInstance* AuraGameInstance = Cast<UAuraGameInstance>( AuraGameModeBase->GetGameInstance() );
	AuraGameInstance->PlayerStartTag = SelectedSlot->PlayerStartTag;
	AuraGameInstance->LoadSlotName = SelectedSlot->GetLoadSlotName();
	AuraGameInstance->SlotIndex = SelectedSlot->SlotIndex;
	AuraGameModeBase->TravelingToMap(SelectedSlot);
}


void UMVVM_LoadScreen::LoadData()
{
	AAuraGameModeBase* AuraGameModeBase = Cast<AAuraGameModeBase>(UGameplayStatics::GetGameMode(this)) ;
	for ( TTuple<int, UMVVM_LoadSlot*> LoadSlot : LoadSlots)
	{
		ULoadScreenSaveGame* SaveObject = AuraGameModeBase->GetSlotData(LoadSlot.Value->GetLoadSlotName() , LoadSlot.Key );
		const FString PlayerName = SaveObject->PlayerName;
		const TEnumAsByte<ESaveSlotStatus> SaveSlotStatus = SaveObject->SaveSlotStatus;
		LoadSlot.Value->SetPlayerName(PlayerName);
		LoadSlot.Value->SlotStatus = SaveSlotStatus;
		LoadSlot.Value->SetMapName(SaveObject->MapName);
		LoadSlot.Value->PlayerStartTag = SaveObject->PlayerStartTag;
		LoadSlot.Value->InitializeSlot();
		
	}
}

void UMVVM_LoadScreen::SetNumLoadSlots(int32 InSlotCount)
{
	UE_MVVM_SET_PROPERTY_VALUE(NumLoadSlots, InSlotCount);
}

