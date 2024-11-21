// sakae's gas project

#pragma once

#include "CoreMinimal.h"
#include "MVVMViewModelBase.h"
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
	FString SlotIndex;

	void SetLoadSlotName(FString InLoadSlotName);

	FString GetLoadSlotName() const { return LoadSlotName; }

	UPROPERTY(EditAnywhere,BlueprintReadWrite,FieldNotify,Setter,Getter)
	FString PlayerName;
	
	void SetPlayerName(FString InPlayerName);
	FString GetPlayerName() const { return PlayerName; }
private:

	UPROPERTY(EditAnywhere,BlueprintReadWrite,FieldNotify,Setter,Getter , meta = (AllowPrivateAccess = "true"))
	FString LoadSlotName;
};
