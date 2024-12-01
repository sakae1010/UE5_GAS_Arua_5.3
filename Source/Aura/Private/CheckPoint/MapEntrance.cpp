// sakae's gas project


#include "CheckPoint/MapEntrance.h"

#include "Components/SphereComponent.h"
#include "Game/AuraGameModeBase.h"
#include "Interaction/PlayerInterface.h"
#include "Kismet/GameplayStatics.h"


AMapEntrance::AMapEntrance(const FObjectInitializer& ObjectInitializer) : Super(ObjectInitializer)
{
	Sphere->SetupAttachment( MoveToComponent );
}

void AMapEntrance::LoadActor_Implementation()
{
	// Do nothing when loading a map Entrance
	//不做任何事情
}

void AMapEntrance::HighlightActor_Implementation()
{
	CheckpointMesh->SetRenderCustomDepth( true );
}

void AMapEntrance::OnSphereOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor,
	UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
	if(OtherActor->Implements<UPlayerInterface>())
	{
		bReached = true;
		if(auto AuraGM = Cast<AAuraGameModeBase>( UGameplayStatics::GetGameMode(GetWorld()) ) )
		{
			AuraGM->SaveWorldState(GetWorld(), DestinationMap.GetAssetName());
		}
		IPlayerInterface::Execute_SaveProgress( OtherActor, PlayerStartTag );

		UGameplayStatics::OpenLevelBySoftObjectPtr( this, DestinationMap );
	}
}
