// sakae's gas project


#include "UI/AttributeMenuWidgetController.h"

#include "AuraGameplayTag.h"
#include "AbilitySystem/AuraAttributeSet.h"

void UAttributeMenuWidgetController::BroadcastInitValues()
{
	UAuraAttributeSet* AuraAttributeSet = Cast<UAuraAttributeSet>(AttributeSet);
	check(AttributeInfo)


	FAuraAttributeInfo Info = AttributeInfo->
		FindAttributeInfo(FAuraGameplayTags::Get().AuraAttribute_Primary_Strength);
	Info.AttributeValue = AuraAttributeSet->GetStrength();

	AttributeInfoDelegate.Broadcast(Info);
	
}

void UAttributeMenuWidgetController::BindCallBacksToDependencies()
{

}
