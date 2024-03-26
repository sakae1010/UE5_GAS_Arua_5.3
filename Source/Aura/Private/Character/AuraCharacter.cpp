// sakae's gas project


#include "Character/AuraCharacter.h"

#include "GameFramework/CharacterMovementComponent.h"

AAuraCharacter::AAuraCharacter()
{

	//轉向移動
	GetCharacterMovement()->bOrientRotationToMovement = true;
	//轉向速率
	GetCharacterMovement()->RotationRate = FRotator(0.f,400.f,0.f);
	//只能在地板 因為top down game
	GetCharacterMovement()->bConstrainToPlane = true;
	GetCharacterMovement()->bSnapToPlaneAtStart = true;

	bUseControllerRotationPitch = false;
	bUseControllerRotationRoll = false;
	bUseControllerRotationYaw = false;
}
