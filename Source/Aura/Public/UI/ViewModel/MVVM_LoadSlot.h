// sakae's gas project

#pragma once

#include "CoreMinimal.h"
#include "MVVMViewModelBase.h"
#include "Game/LoadScreenSaveGame.h"
#include "MVVM_LoadSlot.generated.h"

DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam( FSetWidgetSwitchIndex , int32 , WidgetSwitchIndex );
DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam( FEnableSelectSlotButton , bool , bEnable);

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

	UPROPERTY(BlueprintAssignable)
	FEnableSelectSlotButton EnableSelectSlotButton;
	void InitializeSlot();
	
	UPROPERTY()
	TEnumAsByte<ESaveSlotStatus> SlotStatus;


	UPROPERTY()
	int32 SlotIndex;
	
	UPROPERTY()
	FName PlayerStartTag = FName();
	
	/*Feild FieldNotifies*/

	void SetLoadSlotName(FString InLoadSlotName);
	void SetPlayerName(FString InPlayerName);
	void SetMapName(FString InMapName);
	
	FString GetLoadSlotName() const { return LoadSlotName; }
	FString GetPlayerName() const { return PlayerName; }
	FString GetMapName() const { return MapName; }
	
private:

	UPROPERTY(EditAnywhere,BlueprintReadWrite,FieldNotify,Setter,Getter , meta = (AllowPrivateAccess = "true"))
	FString LoadSlotName;
	
	UPROPERTY(EditAnywhere,BlueprintReadWrite,FieldNotify,Setter,Getter, meta = (AllowPrivateAccess = "true"))
	FString PlayerName;
	
	UPROPERTY(EditAnywhere,BlueprintReadWrite,FieldNotify,Setter,Getter , meta = (AllowPrivateAccess = "true"))
	FString MapName;



	
};
