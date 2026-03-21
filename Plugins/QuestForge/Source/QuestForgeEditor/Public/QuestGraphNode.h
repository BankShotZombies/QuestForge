// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "EdGraph/EdGraphNode.h"
#include "QuestGraphNode.generated.h"

class UQuestAsset;

UCLASS()
class QUESTFORGEEDITOR_API UQuestGraphNode : public UEdGraphNode
{
	GENERATED_BODY()

public:

	UPROPERTY(VisibleAnywhere, Category="Quest")
	TObjectPtr<UQuestAsset> QuestAsset;

	UPROPERTY(VisibleAnywhere, Category="Quest")
	FGuid NodeId;

	void Initialize(UQuestAsset* InQuestAsset, const FGuid& InNodeId);

	/* Creates the default input and output pins for this node. */
	virtual void AllocateDefaultPins() override;

	/* Returns the display title shown on the graph node. */
	virtual FText GetNodeTitle(ENodeTitleType::Type TitleType) const override;

	/* Syncs this graph node's visual position back into the corresponding FQuestNode. */
	void SyncNodePositionToAsset() const;

	/* Rebuilds this node's outgoing transition GUID list from its linked output pins. */
	void SyncTransitionsToAsset();
	
	/* Called when the node's connection list changes. Used to sync transitions back to the asset. */
	virtual void NodeConnectionListChanged() override;

	/* Called when one of this node's pins changes connections. Used to sync transitions back to the asset. */
	virtual void PinConnectionListChanged(UEdGraphPin* Pin) override;

	/* Returns this node's input pin. */
	UEdGraphPin* GetInputPin() const;

	/* Returns this node's input pin. */
	UEdGraphPin* GetOutputPin() const;
	
};
