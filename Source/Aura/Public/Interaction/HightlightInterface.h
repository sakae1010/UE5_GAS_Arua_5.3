// sakae's gas project

#pragma once

#include "CoreMinimal.h"

#include "HightlightInterface.generated.h"

// This class does not need to be modified.
UINTERFACE()
class UHightlightInterface : public UInterface
{
	GENERATED_BODY()
};

/**
 * 
 */
class AURA_API IHightlightInterface
{
	GENERATED_BODY()

	// Add interface functions to this class. This is the class that will be inherited to implement this interface.
public:

	virtual void HighlightActor() = 0;
	virtual void UnHighlightActor() = 0;
};
