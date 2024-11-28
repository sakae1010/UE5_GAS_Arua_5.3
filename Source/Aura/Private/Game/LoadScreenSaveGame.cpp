// sakae's gas project


#include "Game/LoadScreenSaveGame.h"

FSaveMap ULoadScreenSaveGame::GetSaveMapWithMapName(const FString& InMapName)
{
	for (FSaveMap& SaveMap : SaveMaps)
	{
		if (SaveMap.MapName == InMapName)
		{
			return SaveMap;
		}
	}
	return FSaveMap();
}

bool ULoadScreenSaveGame::HasMap(const FString& InMapName)
{
	for (FSaveMap& SaveMap : SaveMaps)
	{
		if (SaveMap.MapName == InMapName)
		{
			return true;
		}
	}
	return false;
}
