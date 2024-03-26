// sakae's gas project


#include "Character/AuraEnemy.h"

void AAuraEnemy::HighlightActor()
{
	// DrawDebugSphere(GetWorld(),GetActorLocation(),5.0f,20,FColor::Red,false,1);
	bHighlight = true;
}

void AAuraEnemy::UnHighlightActor()
{
	bHighlight = false;
}
