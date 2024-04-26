// sakae's gas project


#include "AuraAssetManager.h"

#include "AuraGameplayTag.h"
#include "AbilitySystemGlobals.h"

const UAuraAssetManager& UAuraAssetManager::Get()
{
	check(GEngine)
	return *Cast<UAuraAssetManager>(GEngine->AssetManager);

	
}

void UAuraAssetManager::StartInitialLoading()
{
	Super::StartInitialLoading();

	FAuraGameplayTags::InitializeNativeGameplayTags();
	// This is required to use Target Data!
	UAbilitySystemGlobals::Get().InitGlobalData();
}
