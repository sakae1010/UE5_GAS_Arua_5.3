// sakae's gas project


#include "Actor/AuraEnemySpawnPoint.h"

#include "Character/AuraEnemy.h"


void AAuraEnemySpawnPoint::SpawnEnemy()
{
	if (EnemyClass)
	{
		FActorSpawnParameters SpawnParams;
		SpawnParams.SpawnCollisionHandlingOverride = ESpawnActorCollisionHandlingMethod::AdjustIfPossibleButAlwaysSpawn;
		AAuraEnemy* Enemy = GetWorld()->SpawnActorDeferred<AAuraEnemy>(EnemyClass, GetActorTransform());
		Enemy->SetLevel( EnemyLevel); 
		Enemy->SetChracterClass(CharacterClass);
		Enemy->FinishSpawning(GetActorTransform());
		Enemy->SpawnDefaultController()	;
	}
}
