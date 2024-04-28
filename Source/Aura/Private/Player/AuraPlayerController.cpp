// sakae's gas project


#include "Player/AuraPlayerController.h"
#include "AbilitySystemBlueprintLibrary.h"
#include "AuraGameplayTag.h"
#include "EnhancedInputSubsystems.h"
#include "NavigationPath.h"
#include "NavigationSystem.h"
#include "AbilitySystem/AuraAbilitySystemComponent.h"
#include "Components/SplineComponent.h"
#include "Input/AuraInputComponent.h"
#include "Interaction/EnemyInterface.h"

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
	if (InputTag.MatchesTagExact(FAuraGameplayTags::Get().InputTag_LMB))
	{
		bTargeting = ThisActor ? true : false;
		bAutoRunning = false;	
	}
}

void AAuraPlayerController::InputTagReleased(const FGameplayTag InputTag)
{
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
				CachedDestination = Paths.Last();
				bAutoRunning = true;
			}
		}
	}
	// GEngine->AddOnScreenDebugMessage(-1, 5.f, FColor::Red, FString::Printf(TEXT("FollowTime: %f"), FollowTime));
	FollowTime = 0.0f;
	bTargeting = false;
}

void AAuraPlayerController::InputTagHeld(const FGameplayTag InputTag)
{
	if(!InputTag.MatchesTagExact(FAuraGameplayTags::Get().InputTag_LMB))
	{
		if(GetAuraAbilitySystemComponent() )
		{
			GetAuraAbilitySystemComponent()->AbilityInputHeld(InputTag);
		}
		return;
	}

	if(bTargeting || bShiftKeyDown)
	{
		if(GetAuraAbilitySystemComponent() )
		{
			GetAuraAbilitySystemComponent()->AbilityInputHeld(InputTag);
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
		AuraAbilitySystemComponent =  Cast<UAuraAbilitySystemComponent>( UAbilitySystemBlueprintLibrary::GetAbilitySystemComponent(GetPawn<APawn>()));
	}
	return AuraAbilitySystemComponent;
}

