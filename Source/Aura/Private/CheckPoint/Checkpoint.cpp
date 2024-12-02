// sakae's gas project


#include "CheckPoint/Checkpoint.h"
#include "Components/SphereComponent.h"
#include "Game/AuraGameModeBase.h"
#include "Interaction/PlayerInterface.h"
#include "Kismet/GameplayStatics.h"


ACheckpoint::ACheckpoint(const FObjectInitializer& ObjectInitializer):Super(ObjectInitializer)
{
	PrimaryActorTick.bCanEverTick = false;
	
	
	CheckpointMesh = CreateDefaultSubobject<UStaticMeshComponent>("CheckpointMesh");
	CheckpointMesh->SetupAttachment( GetRootComponent() );
	CheckpointMesh->SetCollisionEnabled(ECollisionEnabled::QueryAndPhysics);

	CheckpointMesh->SetCustomDepthStencilValue( CustomDepthStencilOverride  );
	// CheckpointMesh->SetRenderCustomDepth( false );
	CheckpointMesh->MarkRenderStateDirty();
	
	Sphere = CreateDefaultSubobject<USphereComponent>("Sphere");
	Sphere->SetCollisionResponseToChannels(ECR_Block);
	Sphere->SetupAttachment( CheckpointMesh );
	Sphere->SetCollisionEnabled(ECollisionEnabled::QueryOnly);
	Sphere->SetCollisionResponseToChannels(ECR_Ignore);
	Sphere->SetCollisionResponseToChannel(ECC_Pawn, ECR_Overlap);

	MoveToComponent = CreateDefaultSubobject<USceneComponent>("MoveToComponent");
	MoveToComponent->SetupAttachment( GetRootComponent() );
}

void ACheckpoint::LoadActor_Implementation()
{
	if(bReached)
	{
		HandleGlowEffect();
	}
}

void ACheckpoint::HighlightActor_Implementation()
{
	if(bReached)return;
	CheckpointMesh->SetRenderCustomDepth( true );
}

void ACheckpoint::UnHighlightActor_Implementation()
{
	CheckpointMesh->SetRenderCustomDepth( false );
}

void ACheckpoint::MoveToLocation_Implementation(FVector& OutLocation)
{
	OutLocation =MoveToComponent->GetComponentLocation();
	
}

void ACheckpoint::OnSphereOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor,	UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
	if(OtherActor->Implements<UPlayerInterface>())
	{
		bReached = true;
		if( AAuraGameModeBase* AuraGM = Cast<AAuraGameModeBase>( UGameplayStatics::GetGameMode( GetWorld() ) ) )
		{
			const UWorld* World = GetWorld();
			FString	MapName  = World->GetMapName();
			MapName.RemoveFromStart( World->StreamingLevelsPrefix );
			AuraGM->SaveWorldState(GetWorld() , MapName);
		}
		IPlayerInterface::Execute_SaveProgress( OtherActor, PlayerStartTag );
		HandleGlowEffect();
	}
}

void ACheckpoint::BeginPlay()
{
	Super::BeginPlay();
	if(bBindOverlapCallBack)
	{
		Sphere->OnComponentBeginOverlap.AddDynamic(this, &ACheckpoint::OnSphereOverlap);
	}		
}

void ACheckpoint::HandleGlowEffect()
{
	// 禁用球體組件的碰撞
	Sphere->SetCollisionEnabled(ECollisionEnabled::NoCollision);
	// 為檢查點網格創建動態材質實例
	UMaterialInstanceDynamic* MaterialInstance = UMaterialInstanceDynamic::Create(CheckpointMesh->GetMaterial(0), this);
	// 設置檢查點網格的動態材質實例
	CheckpointMesh->SetMaterial(0, MaterialInstance);
	// 觸發檢查點到達的藍圖事件
	CheckpointReached(MaterialInstance);

}
