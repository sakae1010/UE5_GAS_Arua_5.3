// sakae's gas project


#include "Player/AuraPlayerController.h"
#include "AbilitySystemBlueprintLibrary.h"
#include "AuraGameplayTag.h"
#include "EnhancedInputSubsystems.h"
#include "NavigationPath.h"
#include "NavigationSystem.h"
#include "NiagaraFunctionLibrary.h"
#include "AbilitySystem/AuraAbilitySystemComponent.h"
#include "Actor/MagicCircle.h"
#include "Components/SplineComponent.h"
#include "GameFramework/Character.h"
#include "Input/AuraInputComponent.h"
#include "Interaction/EnemyInterface.h"
#include "UI/Widget/DamageTextComponent.h"

AAuraPlayerController::AAuraPlayerController()
{
	bReplicates = true;
	SplineComponent = CreateDefaultSubobject<USplineComponent>(TEXT("SplineComponent"));
}

void AAuraPlayerController::PlayerTick(float DeltaTime)
{
	Super::PlayerTick(DeltaTime);
	CursorTrace();
	AutoRun();
	UpdateMagicCircleLocation();
}

void AAuraPlayerController::ShowMagicCircle()
{
	if(!MagicCircleClass) return;
	if(!IsValid(MagicCircle))
	{
		MagicCircle = GetWorld()->SpawnActor<AMagicCircle>(MagicCircleClass);
		if(MagicCircle)
		{
			MagicCircle->SetActorLocation(GetPawn()->GetActorLocation());
		}
	}
}

void AAuraPlayerController::HideMagicCircle()
{
	if(IsValid(MagicCircle))
	{
		MagicCircle->Destroy();
		MagicCircle = nullptr;
	}
}

void AAuraPlayerController::ShowDamageNumber_Implementation(float DamageAmount, ACharacter* TargetCharacter,  bool bIsBlockHit , bool bIsCriticalHit )
{
	if(IsLocalController() && IsValid(TargetCharacter) && DamageTextComponentClass )
	{
		if(UDamageTextComponent* DamageText = NewObject<UDamageTextComponent>(TargetCharacter , DamageTextComponentClass))
		{
			DamageText->RegisterComponent();
			DamageText->AttachToComponent(TargetCharacter->GetRootComponent(),FAttachmentTransformRules::KeepRelativeTransform);
			DamageText->DetachFromComponent(FDetachmentTransformRules::KeepWorldTransform);
			DamageText->SetDamageText(DamageAmount , bIsBlockHit , bIsCriticalHit);
			// DamageTextComponent->SetCritical(bCritical);
		}
	}

}

void AAuraPlayerController::AutoRun()
{
	if(!bAutoRunning) return;
	if(APawn* ControllerPawn = GetPawn<APawn>())
    {
    	const FVector LocationOnSpline = SplineComponent->FindLocationClosestToWorldLocation(ControllerPawn->GetActorLocation(), ESplineCoordinateSpace::World);
    	const FVector Direction = SplineComponent->FindDirectionClosestToWorldLocation(LocationOnSpline, ESplineCoordinateSpace::World);
    	ControllerPawn->AddMovementInput(Direction);
    	const float DistanceToDestination = (LocationOnSpline-CachedDestination).Length();
    	if( DistanceToDestination < AutoRunAcceptanceRadius)
    	{
    		bAutoRunning = false;
    	}
    }
}

void AAuraPlayerController::UpdateMagicCircleLocation()
{
	if(IsValid(MagicCircle))
	{
		MagicCircle->SetActorLocation(CursorHit.ImpactPoint);
	}
}

void AAuraPlayerController::BeginPlay()
{
	Super::BeginPlay();
	check(AuraContext);

	UEnhancedInputLocalPlayerSubsystem* Subsystem = ULocalPlayer::GetSubsystem<UEnhancedInputLocalPlayerSubsystem>(GetLocalPlayer());
	if(Subsystem)
	{
		Subsystem->AddMappingContext(AuraContext,0);
	}
	//設定滑鼠相關
	bShowMouseCursor = true;
	DefaultMouseCursor = EMouseCursor::Default;

	FInputModeGameAndUI InputModeData;
	InputModeData.SetLockMouseToViewportBehavior(EMouseLockMode:: DoNotLock);
	InputModeData.SetHideCursorDuringCapture(false);
	SetInputMode(InputModeData);
}

void AAuraPlayerController::SetupInputComponent()
{
	Super::SetupInputComponent();

	UAuraInputComponent* AuraInputComponent = CastChecked<UAuraInputComponent>(InputComponent);
	AuraInputComponent->BindAction(MoveAction,ETriggerEvent::Triggered,this,&AAuraPlayerController::Move);
	AuraInputComponent->BindAction(ShiftAction,ETriggerEvent::Started,this,&AAuraPlayerController::ShiftPressed);
	AuraInputComponent->BindAction(ShiftAction,ETriggerEvent::Completed,this,&AAuraPlayerController::ShiftReleased);
	AuraInputComponent->BindAbilityActions(InputConfig,this,&AAuraPlayerController::InputTagPressed,&AAuraPlayerController::InputTagReleased,&AAuraPlayerController::InputTagHeld);
	
}

void AAuraPlayerController::Move(const FInputActionValue& InputActionValue)
{
	if(GetAuraAbilitySystemComponent() &&
		GetAuraAbilitySystemComponent()->HasMatchingGameplayTag( FAuraGameplayTags::Get().Player_Block_InputPressed))
	{
		return;
	}
	const FVector2D InputAxisVector = InputActionValue.Get<FVector2D>();
	const FRotator Rotation = GetControlRotation();
	const FRotator YawRotation(0,Rotation.Yaw,0);
	const FVector ForwardDirection = FRotationMatrix(YawRotation).GetUnitAxis(EAxis::X);
	const FVector RightDirection = FRotationMatrix(YawRotation).GetUnitAxis(EAxis::Y);

	if(APawn* ControlledPawn = GetPawn<APawn>())
	{
		ControlledPawn->AddMovementInput(ForwardDirection,InputAxisVector.Y);
		ControlledPawn->AddMovementInput(RightDirection,InputAxisVector.X);
	}
}


void AAuraPlayerController::CursorTrace()
{
	if(GetAuraAbilitySystemComponent() &&
		GetAuraAbilitySystemComponent()->HasMatchingGameplayTag( FAuraGameplayTags::Get().Player_Block_CursorTrace))
	{
		if (LastActor)
		{
			LastActor->UnHighlightActor();
		}
		if (ThisActor)
		{
			ThisActor->UnHighlightActor();
		}
		ThisActor = nullptr;
		LastActor = nullptr;
		return;
	}
	GetHitResultUnderCursor(ECC_Visibility, false, CursorHit);
	if (!CursorHit.bBlockingHit)return;
	LastActor = ThisActor;
	ThisActor = Cast<IEnemyInterface>(CursorHit.GetActor());

	if (LastActor != ThisActor)
	{
		if (LastActor)
		{
			LastActor->UnHighlightActor();
		}
		if (ThisActor)
		{
			ThisActor->HighlightActor();
		}
	}
}

void AAuraPlayerController::InputTagPressed(const FGameplayTag InputTag)
{
	if(GetAuraAbilitySystemComponent() &&
		GetAuraAbilitySystemComponent()->HasMatchingGameplayTag( FAuraGameplayTags::Get().Player_Block_InputPressed))
	{
		return;
	}
	if (InputTag.MatchesTagExact(FAuraGameplayTags::Get().InputTag_LMB))
	{
		bTargeting = ThisActor ? true : false;
		bAutoRunning = false;	
	}
	if(GetAuraAbilitySystemComponent() )
	{
		GetAuraAbilitySystemComponent()->AbilityInputPressed(InputTag);
	}
}

void AAuraPlayerController::InputTagReleased(const FGameplayTag InputTag)
{
	if(GetAuraAbilitySystemComponent() &&
		GetAuraAbilitySystemComponent()->HasMatchingGameplayTag( FAuraGameplayTags::Get().Player_Block_InputReleased))
	{
		return;
	}
	if (!InputTag.MatchesTagExact(FAuraGameplayTags::Get().InputTag_LMB))
	{
		if(GetAuraAbilitySystemComponent() )
		{
			GetAuraAbilitySystemComponent()->AbilityInputReleased(InputTag);
		}
		return;
	}
	GetAuraAbilitySystemComponent()->AbilityInputReleased(InputTag);
	if(!bTargeting && !bShiftKeyDown)
	{
		if(const APawn* ControllerPawn = GetPawn<APawn>(); FollowTime<= ShortPressThreshold && ControllerPawn)
		{
			if(const UNavigationPath* NavPath = UNavigationSystemV1::FindPathToLocationSynchronously(
				this, ControllerPawn->GetActorLocation(), CachedDestination))
			{
				TArray<FVector> Paths = NavPath->PathPoints;
				SplineComponent ->ClearSplinePoints();
				for (const FVector& Point : Paths)
				{
					SplineComponent->AddSplinePoint(Point,ESplineCoordinateSpace::World);
				}
				if(Paths.Num()>0)
				{
					CachedDestination = Paths.Last();
					bAutoRunning = true;
				}
			}
			
			if(ClickEffect && ( GetAuraAbilitySystemComponent() && !GetAuraAbilitySystemComponent()->HasMatchingGameplayTag( FAuraGameplayTags::Get().Player_Block_InputPressed)))
			{
				UNiagaraFunctionLibrary::SpawnSystemAtLocation(GetWorld(), ClickEffect, CachedDestination);
			}
		}
	}
	// GEngine->AddOnScreenDebugMessage(-1, 5.f, FColor::Red, FString::Printf(TEXT("FollowTime: %f"), FollowTime));
	FollowTime = 0.0f;
	bTargeting = false;
}

void AAuraPlayerController::InputTagHeld(const FGameplayTag InputTag)
{
	UAuraAbilitySystemComponent* AbilitySystemComponent = GetAuraAbilitySystemComponent();
	if(AbilitySystemComponent &&
		AbilitySystemComponent->HasMatchingGameplayTag( FAuraGameplayTags::Get().Player_Block_InputHeld))
	{
		return;
	}
	if(!InputTag.MatchesTagExact(FAuraGameplayTags::Get().InputTag_LMB))
	{
		if(AbilitySystemComponent)
		{
			AbilitySystemComponent->AbilityInputHeld(InputTag);
		}
		return;
	}

	if(bTargeting || bShiftKeyDown)
	{
		if(AbilitySystemComponent )
		{
			AbilitySystemComponent->AbilityInputHeld(InputTag);
		}
	}else
	{
		FollowTime += GetWorld()->GetDeltaSeconds();

	    if(	CursorHit.bBlockingHit)
		{
			CachedDestination = CursorHit.ImpactPoint;
		}

		if(APawn* ControllerPawn = GetPawn<APawn>())
		{
			const FVector Direction = (CachedDestination - ControllerPawn->GetActorLocation()).GetSafeNormal();
			ControllerPawn->AddMovementInput(Direction);
		}
	}
}

UAuraAbilitySystemComponent* AAuraPlayerController::GetAuraAbilitySystemComponent()
{
	if(AuraAbilitySystemComponent == nullptr)
	{
		AuraAbilitySystemComponent = Cast<UAuraAbilitySystemComponent>( UAbilitySystemBlueprintLibrary::GetAbilitySystemComponent(GetPawn<APawn>()));
	}
	return AuraAbilitySystemComponent;
}

