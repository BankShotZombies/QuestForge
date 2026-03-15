// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "QuestTypes.h"
#include "Engine/DataAsset.h"
#include "QuestAsset.generated.h"

UCLASS(BlueprintType)
class QUESTFORGERUNTIME_API UQuestAsset : public UDataAsset
{
	GENERATED_BODY()

public:
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Quest")
	FName QuestName;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category= "Quest")
	TArray<FQuestNode> Nodes;

	FQuestNode* FindNodeById(const FGuid& NodeId);
	const FQuestNode* FindNodeById(const FGuid& NodeId) const;
	
};
