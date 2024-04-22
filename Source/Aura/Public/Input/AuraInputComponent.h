// sakae's gas project

#pragma once

#include "CoreMinimal.h"
#include "EnhancedInputComponent.h"
#include "AuraInputConfig.h"
#include "AuraInputComponent.generated.h"


UCLASS(ClassGroup=(Custom), meta=(BlueprintSpawnableComponent))
class AURA_API UAuraInputComponent : public UEnhancedInputComponent
{
	GENERATED_BODY()

public:
	// Sets default values for this component's properties
	UAuraInputComponent();

protected:
	virtual void BeginPlay() override;

public:
	virtual void TickComponent(float DeltaTime, ELevelTick TickType,
	                           FActorComponentTickFunction* ThisTickFunction) override;

	template<typename UserClass , typename PressedFuncType , typename ReleaseFuncType , typename HeldFuncType>
	void BindAblilityAction(const UAuraInputConfig* InputConfig,UserClass* Object ,PressedFuncType PressedFunc ,ReleaseFuncType  ReleaseFunc ,HeldFuncType HeldFunc );
};

template <typename UserClass, typename PressedFuncType, typename ReleaseFuncType, typename HeldFuncType>
void UAuraInputComponent::BindAblilityAction(const UAuraInputConfig* InputConfig, UserClass* Object,PressedFuncType PressedFunc, ReleaseFuncType ReleaseFunc, HeldFuncType HeldFunc)
{
	check(InputConfig);
	for (FAuraInputAction Action : InputConfig->InputActions)
	{
		if (Action.InputAction != nullptr && Action.InputTag.IsValid())
		{
			if(PressedFunc)
			{
				BindAction(Action.InputAction, ETriggerEvent::Started, Object, PressedFunc , Action.InputTag);
			}
			if(ReleaseFunc)
			{
				BindAction(Action.InputAction, ETriggerEvent::Completed, Object, ReleaseFunc , Action.InputTag);
			}
			if(HeldFunc)
			{
				BindAction(Action.InputAction, ETriggerEvent::Triggered, Object, HeldFunc , Action.InputTag);
			}

		}
	}
}

