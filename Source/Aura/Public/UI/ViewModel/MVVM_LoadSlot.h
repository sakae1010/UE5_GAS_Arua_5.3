// sakae's gas project

#pragma once

#include "CoreMinimal.h"
#include "MVVMViewModelBase.h"
#include "Game/LoadScreenSaveGame.h"
#include "MVVM_LoadSlot.generated.h"

DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam( FSetWidgetSwitchIndex , int32 , WidgetSwitchIndex );


/**
 * 
 */
UCLASS()
class AURA_API UMVVM_LoadSlot : public UMVVMViewModelBase
{
	GENERATED_BODY()
public:
	
	UPROPERTY(BlueprintAssignable)
	FSetWidgetSwitchIndex SetWidgetSwitchIndex;

	void InitializeSlot();
	
	UPROPERTY()
	TEnumAsByte<ESaveSlotStatus> SlotStatus;


	UPROPERTY()
	FString SlotIndex;

	/*Feild FieldNotifies*/

	UPROPERTY(EditAnywhere,BlueprintReadWrite,FieldNotify,Setter,Getter)
	FString PlayerName;
	void SetLoadSlotName(FString InLoadSlotName);
	void SetPlayerName(FString InPlayerName);
	
	FString GetLoadSlotName() const { return LoadSlotName; }
	FString GetPlayerName() const { return PlayerName; }
private:

	UPROPERTY(EditAnywhere,BlueprintReadWrite,FieldNotify,Setter,Getter , meta = (AllowPrivateAccess = "true"))
	FString LoadSlotName;
};
