// Fill out your copyright notice in the Description page of Project Settings.


#include "QuestGraphSchema.h"

#include "QuestAssetEditor.h"
#include "QuestGraph.h"

UEdGraphNode* FQuestGraphSchemaAction_NewNode::PerformAction(UEdGraph* ParentGraph, UEdGraphPin* FromPin,
                                                             const FVector2D Location, bool bSelectNewNode)
{
	if (!ParentGraph)
	{
		return nullptr;
	}

	if (UQuestGraph* QuestGraph = Cast<UQuestGraph>(ParentGraph))
	{
		if (TSharedPtr<FQuestAssetEditor> Editor = QuestGraph->Editor.Pin())
		{
			Editor->CreateNodeAtLocation(Location);
		}
	}

	return nullptr;
}

void UQuestGraphSchema::GetContextMenuActions(UToolMenu* Menu, UGraphNodeContextMenuContext* Context) const
{
	Super::GetContextMenuActions(Menu, Context);
}

void UQuestGraphSchema::GetGraphContextActions(FGraphContextMenuBuilder& ContextMenuBuilder) const
{
	if (!ContextMenuBuilder.CurrentGraph)
	{
		return;
	}

	if (ContextMenuBuilder.FromPin == nullptr)
	{
		TSharedPtr<FQuestGraphSchemaAction_NewNode> AddNodeAction =
			MakeShared<FQuestGraphSchemaAction_NewNode>();

		ContextMenuBuilder.AddAction(AddNodeAction);
	}
}

const FPinConnectionResponse UQuestGraphSchema::CanCreateConnection(const UEdGraphPin* A, const UEdGraphPin* B) const
{
	if(!A || !B)
	{
		return FPinConnectionResponse(CONNECT_RESPONSE_DISALLOW, TEXT("Invalid pins"));
	}

	if (A == B)
	{
		return FPinConnectionResponse(CONNECT_RESPONSE_DISALLOW, TEXT("Cannot connect pin to itself"));
	}

	if (A->Direction == B->Direction)
	{
		return FPinConnectionResponse(CONNECT_RESPONSE_DISALLOW, TEXT("Pins must have opposite directions"));
	}

	if (A->GetOwningNode() == B->GetOwningNode())
	{
		return FPinConnectionResponse(CONNECT_RESPONSE_DISALLOW, TEXT("Cannot connect a node to itself"));
	}

	return FPinConnectionResponse(CONNECT_RESPONSE_MAKE, TEXT(""));
}

void UQuestGraphSchema::CreateDefaultNodesForGraph(UEdGraph& Graph) const
{
	
}

FLinearColor UQuestGraphSchema::GetPinTypeColor(const FEdGraphPinType& PinType) const
{
	return FLinearColor::White;
}
