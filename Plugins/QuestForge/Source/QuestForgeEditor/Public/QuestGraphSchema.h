// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "EdGraph/EdGraphSchema.h"
#include "QuestGraphSchema.generated.h"

/**
 * 
 */
UCLASS()
class QUESTFORGEEDITOR_API UQuestGraphSchema : public UEdGraphSchema
{
	GENERATED_BODY()

public:
	// Populates the right-click context menu when you click on a node or pin in the graph
	virtual void GetContextMenuActions(class UToolMenu* Menu, class UGraphNodeContextMenuContext* Context) const override;

	// Whether a connection between two pins is valid
	virtual const FPinConnectionResponse CanCreateConnection(const UEdGraphPin* A, const UEdGraphPin* B) const override;

	// Populate new graph with any default nodes
	virtual void CreateDefaultNodesForGraph(UEdGraph& Graph) const override;

	// Returns the display color for a pin based on its type
	virtual FLinearColor GetPinTypeColor(const FEdGraphPinType& PinType) const override;
};
