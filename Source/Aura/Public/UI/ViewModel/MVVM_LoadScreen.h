// sakae's gas project

#pragma once

#include "CoreMinimal.h"
#include "MVVMViewModelBase.h"
#include "MVVM_LoadScreen.generated.h"
DECLARE_DYNAMIC_MULTICAST_DELEGATE( FSlotSelected);
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
	UPROPERTY(BlueprintAssignable)
	FSlotSelected SlotSelected;
	
	UPROPERTY(EditDefaultsOnly, Category = "MVVM_LoadScreen")
	TSubclassOf<UMVVM_LoadSlot> LoadSlotViewModelClass;

	UFUNCTION (BlueprintPure)
	UMVVM_LoadSlot* GetLoadSlotViewModelByIndex(int32 Index) const;

	UFUNCTION(BlueprintCallable)
	void NewSlotButtonPressed(int32 SlotIndex,const FString& EnterName);
	
	UFUNCTION(BlueprintCallable)
	void NewGameButtonPressed(int32 SlotIndex);

	UFUNCTION(BlueprintCallable)
	void SelectSlotButtonPressed(int32 SlotIndex);

	UFUNCTION(BlueprintCallable)
	void DeleteSlotButtonPressed();
	
	void LoadData();

	
	void SetNumLoadSlots(int32 InSlotCount);
	int32 GetNumLoadSlots() const { return NumLoadSlots; }
private:
	UPROPERTY()
	TMap<int32, UMVVM_LoadSlot*> LoadSlots;
	UPROPERTY()
	TObjectPtr<UMVVM_LoadSlot> LoadSlot_0;
	UPROPERTY()
	TObjectPtr<UMVVM_LoadSlot> LoadSlot_1;
	UPROPERTY()
	TObjectPtr<UMVVM_LoadSlot> LoadSlot_2;
	
	UPROPERTY(EditAnywhere,BlueprintReadWrite,FieldNotify,Setter,Getter , meta = (AllowPrivateAccess = "true"))
	int32 NumLoadSlots;
	
	UPROPERTY()
	TObjectPtr<UMVVM_LoadSlot> SelectedSlot;
};
