﻿// sakae's gas project


#include "AbilitySystem/Data/LootTiers.h"

TArray<FLootItem> ULootTiers::GetLootItems()
{
	TArray<FLootItem> ReturnLootItems;

	for (const FLootItem& LootItem : LootItems)
	{
		for (int32 i = 0; i < LootItem.MaxNumberToSpawn; i++)
		{
			if ( FMath::FRandRange(1.f, 100.f) < LootItem.ChanceToSpawn)
			{
				FLootItem NewItem ;
				NewItem.LootClass = LootItem.LootClass;
				NewItem.ChanceToSpawn = LootItem.ChanceToSpawn;
				NewItem.MaxNumberToSpawn = LootItem.MaxNumberToSpawn;
				NewItem.bLootLevelOverride = LootItem.bLootLevelOverride;
				ReturnLootItems.Add(LootItem);
			}
		}
	}
	
	return LootItems;
}
