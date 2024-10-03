// sakae's gas project


#include "Actor/PointCollection.h"

#include "AbilitySystem/AuraAbilitySystemLibrary.h"
#include "Kismet/KismetMathLibrary.h"


APointCollection::APointCollection()
{
	PrimaryActorTick.bCanEverTick = false;

	Pt_0 = CreateDefaultSubobject<USceneComponent>("Pt_0");
	SetRootComponent( Pt_0 );
	ImmutablePoints.Add( Pt_0 );

	Pt_1 = CreateDefaultSubobject<USceneComponent>("Pt_1");
	ImmutablePoints.Add( Pt_1 );
	Pt_1->SetupAttachment( GetRootComponent() );

	Pt_2 = CreateDefaultSubobject<USceneComponent>("Pt_2");
	ImmutablePoints.Add(Pt_2);
	Pt_2->SetupAttachment(GetRootComponent());

	Pt_3 = CreateDefaultSubobject<USceneComponent>("Pt_3");
	ImmutablePoints.Add(Pt_3);
	Pt_3->SetupAttachment(GetRootComponent());

	Pt_4 = CreateDefaultSubobject<USceneComponent>("Pt_4");
	ImmutablePoints.Add(Pt_4);
	Pt_4->SetupAttachment(GetRootComponent());

	Pt_5 = CreateDefaultSubobject<USceneComponent>("Pt_5");
	ImmutablePoints.Add(Pt_5);
	Pt_5->SetupAttachment(GetRootComponent());

	Pt_6 = CreateDefaultSubobject<USceneComponent>("Pt_6");
	ImmutablePoints.Add(Pt_6);
	Pt_6->SetupAttachment(GetRootComponent());

	Pt_7 = CreateDefaultSubobject<USceneComponent>("Pt_7");
	ImmutablePoints.Add(Pt_7);
	Pt_7->SetupAttachment(GetRootComponent());

	Pt_8 = CreateDefaultSubobject<USceneComponent>("Pt_8");
	ImmutablePoints.Add(Pt_8);
	Pt_8->SetupAttachment(GetRootComponent());

	Pt_9 = CreateDefaultSubobject<USceneComponent>("Pt_9");
	ImmutablePoints.Add(Pt_9);
	Pt_9->SetupAttachment(GetRootComponent());

	Pt_10 = CreateDefaultSubobject<USceneComponent>("Pt_10");
	ImmutablePoints.Add(Pt_10);
	Pt_10->SetupAttachment(GetRootComponent());
	


}

TArray<USceneComponent*> APointCollection::GetGroundPoints(const FVector& GroundLocation, int NumPoints, float YawOverride)
{
	//Attempted to access ImmutablePts out of Bounds.
	//嘗試超出不可變點的範圍。
	checkf( NumPoints <= ImmutablePoints.Num() , TEXT("Attempted to access ImmutablePts out of Bounds.") );
	TArray<USceneComponent*> ArrayCopy;

	// 遍歷所有不可變點
	for ( USceneComponent* Pt : ImmutablePoints)
	{
		// 如果已經達到所需的點數，則跳出迴圈
		if (ArrayCopy.Num() >= NumPoints) break;
		
		// 如果當前點不是 Pt_0，則進行位置和旋轉的調整
		if(Pt!=Pt_0)
		{
			FVector ToPoint = Pt->GetComponentLocation() - Pt_0->GetComponentLocation();
			ToPoint = ToPoint.RotateAngleAxis(YawOverride, FVector::UpVector);
			Pt->SetWorldLocation( Pt_0->GetComponentLocation() + ToPoint );
		}

		// 獲取當前點的位置
		const FVector PtLocation = Pt->GetComponentLocation();
		// 設置向上和向下的向量，用於線性追蹤
		const FVector RasisedVector = FVector( PtLocation.X , PtLocation.Y , PtLocation.Z +500.f );
		const FVector LoweredVector = FVector( PtLocation.X , PtLocation.Y , PtLocation.Z -500.f );
		
		// 進行線性追蹤，獲取碰撞結果
		FHitResult HitResult;
		TArray<AActor*> IngoreActors;
		UAuraAbilitySystemLibrary::GetLivePlayerWithinRadius( this , IngoreActors ,TArray<AActor*>(),1500.f,GetActorLocation());
		FCollisionQueryParams Params;
		Params.AddIgnoredActors(IngoreActors);
		GetWorld()->LineTraceSingleByProfile(HitResult,RasisedVector,LoweredVector,FName("BlockAll"),Params);

		// 調整當前點的位置和旋轉
		const FVector AdjustLocation = FVector(PtLocation.X , PtLocation.Y , HitResult.ImpactPoint.Z);
		Pt->SetWorldLocation( AdjustLocation );
		Pt->SetWorldRotation( UKismetMathLibrary::MakeRotFromZ( HitResult.ImpactNormal ) );
		// 將當前點添加到結果陣列中
		ArrayCopy.Add( Pt );
	}

	return ArrayCopy;
	
}

void APointCollection::BeginPlay()
{
	Super::BeginPlay();
	
}


