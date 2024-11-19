// sakae's gas project

#pragma once

#include "CoreMinimal.h"
#include "MVVMViewModelBase.h"
#include "MVVM_LoadScreen.generated.h"

class UMVVM_LoadSlot;
/**
 * 
 */
UCLASS()
class AURA_API UMVVM_LoadScreen : public UMVVMViewModelBase
{
	GENERATED_BODY()
public:
	void InitializeLoadSlots();
	
	UPROPERTY(EditDefaultsOnly, Category = "MVVM_LoadScreen")
	TSubclassOf<UMVVM_LoadSlot> LoadSlotViewModelClass;

	UFUNCTION (BlueprintPure)
	UMVVM_LoadSlot* GetLoadSlotViewModelByIndex(int32 Index) const;

	UFUNCTION(BlueprintCallable)
	void NewSlotButtonPressed(int32 SlotIndex,const FString& SlotName);
	
	UFUNCTION(BlueprintCallable)
	void NewGameButtonPressed(int32 SlotIndex);

	UFUNCTION(BlueprintCallable)
	void SelectSlotButtonPressed(int32 SlotIndex);
private:
	UPROPERTY()
	TMap<int32, UMVVM_LoadSlot*> LoadSlots;
	UPROPERTY()
	TObjectPtr<UMVVM_LoadSlot> LoadSlot_0;
	UPROPERTY()
	TObjectPtr<UMVVM_LoadSlot> LoadSlot_1;
	UPROPERTY()
	TObjectPtr<UMVVM_LoadSlot> LoadSlot_2;
};
