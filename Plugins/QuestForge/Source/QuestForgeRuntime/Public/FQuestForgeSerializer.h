#pragma once

#include "CoreMinimal.h"

class UQuestAsset;
class UWorldStateAsset;

class QUESTFORGERUNTIME_API FQuestForgeSerializer
{
	
public:

	FQuestForgeSerializer() = delete;

    static FString SerializeQuestAsset(const UQuestAsset* QuestAsset);
	static FString SerializeWorldStateAsset(const UWorldStateAsset* WorldStateAsset);
	
};
