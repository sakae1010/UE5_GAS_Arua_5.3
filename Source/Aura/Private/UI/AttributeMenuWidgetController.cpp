// sakae's gas project


#include "UI/AttributeMenuWidgetController.h"

#include "AuraGameplayTag.h"
#include "AbilitySystem/AuraAttributeSet.h"

void UAttributeMenuWidgetController::BroadcastInitValues()
{
	UAuraAttributeSet* AuraAttributeSet = Cast<UAuraAttributeSet>(AttributeSet);
	check(AttributeInfo)
	for (const auto Pair : AuraAttributeSet->TagsToAttributeMap)
	{
		FAuraAttributeInfo Info = AttributeInfo->FindAttributeInfo(Pair.Key);
		Info.AttributeValue = Pair.Value().GetNumericValue(AuraAttributeSet);
		AttributeInfoDelegate.Broadcast(Info);
	}
		
	// FAuraAttributeInfo Info = AttributeInfo->FindAttributeInfo(FAuraGameplayTags::Get().AuraAttribute_Primary_Strength);
	// Info.AttributeValue = AuraAttributeSet->GetStrength();
	// AttributeInfoDelegate.Broadcast(Info);
	
}

void UAttributeMenuWidgetController::BindCallBacksToDependencies()
{

}
