// sakae's gas project


#include "Character/AuraCharacter.h"

#include "AuraGameplayTag.h"
#include "NiagaraComponent.h"
#include "AbilitySystem/AuraAbilitySystemLibrary.h"
#include "AbilitySystem/AuraAttributeSet.h"
#include "AbilitySystem/Data/LevelUpInfo.h"
#include "AbilitySystem/Debuff/DebuffNiagaraComponent.h"
#include "Camera/CameraComponent.h"
#include "Game/AuraGameModeBase.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "GameFramework/SpringArmComponent.h"
#include "Kismet/GameplayStatics.h"
#include "Player/AuraPlayerController.h"
#include "Player/AuraPlayerState.h"
#include "Game/AuraGameInstance.h"
#include "Game/LoadScreenSaveGame.h"
#include "UI/HUD/AuraHUD.h"

AAuraCharacter::AAuraCharacter()
{
	CameraBoom = CreateDefaultSubobject<USpringArmComponent>(TEXT("CameraBoom"));
	CameraBoom->SetupAttachment(RootComponent);
	CameraBoom->SetUsingAbsoluteRotation(true);
	CameraBoom->bDoCollisionTest = false;

	CameraComponent = CreateDefaultSubobject<UCameraComponent>(TEXT("CameraComponent"));
	CameraComponent->SetupAttachment(CameraBoom , USpringArmComponent::SocketName);
	CameraComponent->bUsePawnControlRotation = false;
	
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

	LevelUpNiagaraComponent = CreateDefaultSubobject<UNiagaraComponent>(TEXT("LevelUpNiagaraComponent"));
	LevelUpNiagaraComponent->SetupAttachment(RootComponent);
	LevelUpNiagaraComponent->bAutoActivate = false;
	CharacterClass = ECharacterClass::Elementalist;
}

void AAuraCharacter::PossessedBy(AController* NewController)
{
	Super::PossessedBy(NewController);

	// Init ability info for the Server
	InitAbilityActorInfo();
	LoadProgress();
}

void AAuraCharacter::LoadProgress()
{
	//Get game mode
	AAuraGameModeBase* AuraGameMode = Cast<AAuraGameModeBase>( UGameplayStatics::GetGameMode( GetWorld() ) );
	if (AuraGameMode)
	{
		//Get save data
		ULoadScreenSaveGame* SaveData = AuraGameMode->RetrieveInGameSaveData();
		if(SaveData == nullptr) return;

		if (SaveData->bFirstTimeLoadInGame)
		{
			InitializeDefaultAttributes();
			AddCharacterAbilities();
		}
		else //處理讀取存檔
		{
			//TODO : Load in Abilities from disk
		
			if(AAuraPlayerState* AuraPlayerState = Cast<AAuraPlayerState>( GetPlayerState() ) )
			{
				AuraPlayerState->SetLevel(SaveData->PlayerLevel , false);
				AuraPlayerState->SetXP(SaveData->XP);
				AuraPlayerState->SetAttriblePoints(SaveData->AttributePoints);
				AuraPlayerState->SetSpellPoints(SaveData->SpellPoints);
			}
			UAuraAbilitySystemLibrary::InitializeDefaultAttributesFormSaveData(this, AbilitySystemComponent, SaveData);
		}
	}
}


void AAuraCharacter::OnRep_PlayerState()
{
	Super::OnRep_PlayerState();
	// Init ability info for the Client
	InitAbilityActorInfo();
	AddCharacterAbilities();
}

void AAuraCharacter::OnRep_Stunned()
{
	if(UAuraAbilitySystemComponent* ASC = Cast<UAuraAbilitySystemComponent>( AbilitySystemComponent ))
	{
		const FAuraGameplayTags& GameplayTag = FAuraGameplayTags::Get();
		FGameplayTagContainer BlockedTags;
		BlockedTags.AddTag(GameplayTag.Player_Block_InputHeld);
		BlockedTags.AddTag(GameplayTag.Player_Block_InputPressed);
		BlockedTags.AddTag(GameplayTag.Player_Block_InputReleased);
		BlockedTags.AddTag(GameplayTag.Player_Block_CursorTrace);
		if(bIsStunned)
		{
			ASC->AddLooseGameplayTags(BlockedTags);
			StunDebuffNiagaraComponent->Activate();
		}
		else
		{
			ASC->RemoveLooseGameplayTags(BlockedTags);
			StunDebuffNiagaraComponent->Deactivate();
		}
	}
}

void AAuraCharacter::OnRep_Burned()
{
	if(bIsBurned)
	{
		// ASC->AddLooseGameplayTags(BlockedTags);
		BurnDebuffNiagaraComponent->Activate();
	}
	else
	{
		// ASC->RemoveLooseGameplayTags(BlockedTags);
		BurnDebuffNiagaraComponent->Deactivate();
	}
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
	// LevelUpNiagaraComponent
	MulticastLevelUpParticles();
}

void AAuraCharacter::MulticastLevelUpParticles_Implementation() const
{
	if (IsValid(LevelUpNiagaraComponent))
	{
		const FVector CameraLocation = CameraComponent->GetComponentLocation();
		const FVector LevelUpNiagaraLocation = LevelUpNiagaraComponent->GetComponentLocation();
		const FRotator SpawnRotator = (CameraLocation-LevelUpNiagaraLocation).Rotation();
		LevelUpNiagaraComponent->SetWorldRotation(SpawnRotator);
		LevelUpNiagaraComponent->Activate(true);
	}
	
}

int32 AAuraCharacter::GetXP_Implementation() const
{
	AAuraPlayerState* AuraPlayerState = GetPlayerState<AAuraPlayerState>();
	check(AuraPlayerState);
	return AuraPlayerState->GetXP();
}

int32 AAuraCharacter::FindLevelForXP_Implementation(int32 InXP)
{
	AAuraPlayerState* AuraPlayerState = GetPlayerState<AAuraPlayerState>();
	check(AuraPlayerState);
	return AuraPlayerState->LevelUpInfo->FindLevelForXP(InXP);
}

int32 AAuraCharacter::GetAttributePointsReward_Implementation(int32 InLevel) const
{
	AAuraPlayerState* AuraPlayerState = GetPlayerState<AAuraPlayerState>();
	check(AuraPlayerState);
	return AuraPlayerState->LevelUpInfo->LevelUpInfos[InLevel].AttributePointAward;
	
}

int32 AAuraCharacter::GetSpellPointsReward_Implementation(int32 InLevel) const
{
	AAuraPlayerState* AuraPlayerState = GetPlayerState<AAuraPlayerState>();
	check(AuraPlayerState);
	return AuraPlayerState->LevelUpInfo->LevelUpInfos[InLevel].SpellPointAward;
}

void AAuraCharacter::AddToPlayerLevel_Implementation(int32 InNumPlayerLevel)
{
	AAuraPlayerState* AuraPlayerState = GetPlayerState<AAuraPlayerState>();
	check(AuraPlayerState);
	AuraPlayerState->AddToLevel(InNumPlayerLevel);

	if(UAuraAbilitySystemComponent* AuraAbilitySystemComponent = Cast<UAuraAbilitySystemComponent>(GetAbilitySystemComponent()))
	{
		AuraAbilitySystemComponent->UpdateAbilityStatuses(AuraPlayerState->GetPlayerLevel());
	}
}

void AAuraCharacter::AddToAttributePoints_Implementation(int32 InAttributePoints)
{
	AAuraPlayerState* AuraPlayerState = GetPlayerState<AAuraPlayerState>();
	check(AuraPlayerState);
	AuraPlayerState->AddToAttributePoints(InAttributePoints);
}

int32 AAuraCharacter::GetSpellPoints_Implementation() const
{
	AAuraPlayerState* AuraPlayerState = GetPlayerState<AAuraPlayerState>();
	check(AuraPlayerState);
	return AuraPlayerState->GetSpellPoints();
}

void AAuraCharacter::AddToSpellPoints_Implementation(int32 InSpellPoints)
{
	AAuraPlayerState* AuraPlayerState = GetPlayerState<AAuraPlayerState>();
	check(AuraPlayerState);
	AuraPlayerState->AddToSpellPoints(InSpellPoints);
}

void AAuraCharacter::ShowMagicCircle_Implementation(UMaterialInterface* MagicCircleMaterial)
{
	if( AAuraPlayerController* AuraPlayerController = Cast<AAuraPlayerController>( GetController() ) )
	{
		AuraPlayerController->ShowMagicCircle(MagicCircleMaterial);
		AuraPlayerController->bShowMouseCursor = false;
	}
}

void AAuraCharacter::HideMagicCircle_Implementation()
{
	if( AAuraPlayerController* AuraPlayerController = Cast<AAuraPlayerController>(GetController()))
	{
		AuraPlayerController->HideMagicCircle();
		AuraPlayerController->bShowMouseCursor = true;
	}
}

void AAuraCharacter::SaveProgress_Implementation(const FName& CheckpointTag)
{
	AAuraGameModeBase* AuraGameMode = Cast<AAuraGameModeBase>( UGameplayStatics::GetGameMode( GetWorld() ) );
	if(AuraGameMode)
	{
		ULoadScreenSaveGame* SaveData = AuraGameMode->RetrieveInGameSaveData();
		if(SaveData == nullptr) return;
		SaveData->PlayerStartTag = CheckpointTag;
		
		if( AAuraPlayerState* AuraPlayerState = Cast<AAuraPlayerState>( GetPlayerState() ) )
		{
			SaveData->PlayerLevel = AuraPlayerState->GetPlayerLevel();
			SaveData->XP = AuraPlayerState->GetXP();
			SaveData->AttributePoints = AuraPlayerState->GetAttributePoints();
			SaveData->SpellPoints = AuraPlayerState->GetSpellPoints();
		}

		SaveData->Strength = UAuraAttributeSet::GetStrengthAttribute().GetNumericValue( GetAttributeSet() );
		SaveData->Intelligence = UAuraAttributeSet::GetIntelligenceAttribute().GetNumericValue( GetAttributeSet() );
		SaveData->Resilience = UAuraAttributeSet::GetResilienceAttribute().GetNumericValue( GetAttributeSet() );
		SaveData->Vigor = UAuraAttributeSet::GetVigorAttribute().GetNumericValue( GetAttributeSet() );
		SaveData->bFirstTimeLoadInGame = false;
		AuraGameMode->SaveIngamePrrogressData(SaveData);
	}
	
}


int32 AAuraCharacter::GetAttributePoints_Implementation() const
{
	AAuraPlayerState* AuraPlayerState = GetPlayerState<AAuraPlayerState>();
	check(AuraPlayerState);
	return AuraPlayerState->GetAttributePoints();
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
	OnAscRegisteredDelegate.Broadcast(AbilitySystemComponent );
	AbilitySystemComponent->RegisterGameplayTagEvent( FAuraGameplayTags::Get().Debuff_Stun, EGameplayTagEventType::NewOrRemoved).AddUObject(this, &AAuraCharacter::StunTagChanged);
	if (AAuraPlayerController* PlayerController = Cast<AAuraPlayerController>(GetController()))
	{
		if(AAuraHUD* AuraHUD = Cast<AAuraHUD>(PlayerController->GetHUD()))
		{
			AuraHUD->InitOverlayWidgetController(PlayerController, AuraPlayerState, AbilitySystemComponent, AttributeSet);
			
		}
	}
}




