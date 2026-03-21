// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "EdGraph/EdGraph.h"
#include "QuestGraph.generated.h"

/**
 * 
 */
UCLASS()
class QUESTFORGEEDITOR_API UQuestGraph : public UEdGraph
{
	GENERATED_BODY()

public:

	TWeakPtr<class FQuestAssetEditor> Editor;
	
};
