// sakae's gas project

#pragma once

#include "CoreMinimal.h"
#include "GameplayTagContainer.h"

/**
 * AuraGameplayTags 
 * Singleton container native Gameplay Tags
 */
struct FAuraGameplayTags
{
public:
 static const FAuraGameplayTags& Get() {return GameplayTags;}
 static void InitializeNativeGameplayTags();
protected :

 private :
  static FAuraGameplayTags GameplayTags;
};

 
