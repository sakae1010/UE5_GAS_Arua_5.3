// sakae's gas project


#include "Character/AuraCharacter.h"

#include "GameFramework/CharacterMovementComponent.h"
#include "Player/AuraPlayerController.h"
#include "Player/AuraPlayerState.h"
#include "UI/HUD/AuraHUD.h"

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

	CharacterClass = ECharacterClass::Elementalist;
}

void AAuraCharacter::PossessedBy(AController* NewController)
{
	Super::PossessedBy(NewController);

	// Init ability info for the Server
	InitAbilityActorInfo();
	AddCharacterAbilities();
}

void AAuraCharacter::OnRep_PlayerState()
{
	Super::OnRep_PlayerState();
	// Init ability info for the Client
	InitAbilityActorInfo();
	AddCharacterAbilities();
}

int32 AAuraCharacter::GetPlayerLevel_Implementation()
{
	const AAuraPlayerState* AuraPlayerState = GetPlayerState<AAuraPlayerState>();
	check(AuraPlayerState);
	return AuraPlayerState->GetPlayerLevel();
}

void AAuraCharacter::AddToXP_Implementation(int32 XP)
{
	AAuraPlayerState* AuraPlayerState = GetPlayerState<AAuraPlayerState>();
	check(AuraPlayerState);
	AuraPlayerState->AddXP(XP);
}

void AAuraCharacter::LevelUp_Implementation()
{
	
}

void AAuraCharacter::InitAbilityActorInfo()
{
	Super::InitAbilityActorInfo();
	AAuraPlayerState* AuraPlayerState = GetPlayerState<AAuraPlayerState>();
	check(AuraPlayerState);
	AbilitySystemComponent = AuraPlayerState->GetAbilitySystemComponent();
	Cast<UAuraAbilitySystemComponent>(AbilitySystemComponent)->AbilityActorInfoInit();
	
	AbilitySystemComponent->InitAbilityActorInfo(AuraPlayerState, this);
	AttributeSet = AuraPlayerState->GetAttributeSet();
	
	if (AAuraPlayerController* PlayerController = Cast<AAuraPlayerController>(GetController()))
	{
		if(AAuraHUD* AuraHUD = Cast<AAuraHUD>(PlayerController->GetHUD()))
		{
			AuraHUD->InitOverlayWidgetController(PlayerController, AuraPlayerState, AbilitySystemComponent, AttributeSet);
			
		}
	}
	InitializeDefaultAttributes();
}



