// sakae's gas project


#include "UI/ViewModel/MVVM_LoadSlot.h"

void UMVVM_LoadSlot::InitializeSlot()
{
	//TODO Check Slot Status base on loaded data
	SetWidgetSwitchIndex.Broadcast(2);

	
}

void UMVVM_LoadSlot::SetLoadSlotName(FString InLoadSlotName)
{
	UE_MVVM_SET_PROPERTY_VALUE( LoadSlotName , InLoadSlotName );
}

void UMVVM_LoadSlot::SetPlayerName(FString InPlayerName)
{
	UE_MVVM_SET_PROPERTY_VALUE( PlayerName , InPlayerName );
}
