// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "EdGraph/EdGraphSchema.h"
#include "QuestGraphSchema.generated.h"

struct FQuestGraphSchemaAction_NewNode : public FEdGraphSchemaAction
{
	FVector2D NodePosition;

	FQuestGraphSchemaAction_NewNode() : FEdGraphSchemaAction(
		FText::FromString(TEXT("Quest")),
		FText::FromString(TEXT("Add Quest Node")),
		FText::FromString(TEXT("Creates a new quest node.")),
		0)
	{
		
	}

	virtual UEdGraphNode* PerformAction(
		UEdGraph* ParentGraph,
		UEdGraphPin* FromPin,
		const FVector2D Location,
		bool bSelectNewNode = true) override;
};

UCLASS()
class QUESTFORGEEDITOR_API UQuestGraphSchema : public UEdGraphSchema
{
	GENERATED_BODY()

public:
	// Populates the right-click context menu when you click on a node or pin in the graph
	virtual void GetContextMenuActions(class UToolMenu* Menu, class UGraphNodeContextMenuContext* Context) const override;

	/* Populates the action menu shown when right-clicking in empty graph space. */
	virtual void GetGraphContextActions(FGraphContextMenuBuilder& ContextMenuBuilder) const override;

	// Whether a connection between two pins is valid
	virtual const FPinConnectionResponse CanCreateConnection(const UEdGraphPin* A, const UEdGraphPin* B) const override;

	// Populate new graph with any default nodes
	virtual void CreateDefaultNodesForGraph(UEdGraph& Graph) const override;

	// Returns the display color for a pin based on its type
	virtual FLinearColor GetPinTypeColor(const FEdGraphPinType& PinType) const override;
};
