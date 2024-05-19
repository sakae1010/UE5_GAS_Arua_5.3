// sakae's gas project


#include "AI/AuraAIController.h"

#include "BehaviorTree/BehaviorTreeComponent.h"
#include "BehaviorTree/BlackboardComponent.h"


// Sets default values
AAuraAIController::AAuraAIController()
{
	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;
	Blackboard = CreateDefaultSubobject<UBlackboardComponent>("BlackboardComponent");
	check(Blackboard);
	BehaviorTreeComponent = CreateDefaultSubobject<UBehaviorTreeComponent>("BehaviorTreeComponent");
	check(BehaviorTreeComponent);
}

// Called when the game starts or when spawned
void AAuraAIController::BeginPlay()
{
	Super::BeginPlay();
	
}

// Called every frame
void AAuraAIController::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
}

