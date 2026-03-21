// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "QuestNodeDetailsProxy.generated.h"

UCLASS()
class QUESTFORGEEDITOR_API UQuestNodeDetailsProxy : public UObject
{
	GENERATED_BODY()

public:

	/** Called when a property on this proxy is edited in the details panel. */
	virtual void PostEditChangeProperty(FPropertyChangedEvent& PropertyChangedEvent) override;

	/* Loads this proxy's editable fields from the matching node in the quest asset. */
	void LoadFromNode(class UQuestAsset* InQuestAsset, const FGuid& InNodeId);

	/* Saves this proxy's editable fields back into the matching node in the quest asset. */
	void SaveToNode() const;

public:
	/* The name of the quest node. */
	UPROPERTY(EditAnywhere, Category="Quest")
	FName NodeName;

	/* The outgoing transitions from this quest node. */
	UPROPERTY(EditAnywhere, Category="Quest")
	TArray<struct FQuestTransition> Transitions;

	/* The effects applied when this quest node is reached. */
	UPROPERTY(EditAnywhere, Category="Quest")
	TArray<struct FQuestEffect> Effects;

	/* The asset that owns the real quest node being edited. */
	UPROPERTY()
	TObjectPtr<UQuestAsset> QuestAsset;

	/* The ID of the real quest node being edited. */
	UPROPERTY()
	FGuid NodeId;

	/** The quest asset editor that owns this proxy. */
	TWeakPtr<class FQuestAssetEditor> Editor;

};
