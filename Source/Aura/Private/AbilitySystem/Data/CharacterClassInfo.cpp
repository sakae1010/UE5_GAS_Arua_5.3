// sakae's gas project


#include "AbilitySystem/Data/CharacterClassInfo.h"

FCharacterClassDefaultInfo UCharacterClassInfo::GetCharacterClassInfo(ECharacterClass CharacterClass)
{
	return  CharacterClassInformation.FindChecked(CharacterClass);
}
