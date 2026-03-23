// Fill out your copyright notice in the Description page of Project Settings.


#include "QuestGraphNode.h"

#include "QuestAsset.h"
#include "QuestTypes.h"

void UQuestGraphNode::Initialize(UQuestAsset* InQuestAsset, const FGuid& InNodeId)
{
	QuestAsset = InQuestAsset;
	NodeId = InNodeId;
}

void UQuestGraphNode::AllocateDefaultPins()
{
	CreatePin(EGPD_Input, TEXT("QuestNode"), TEXT("In"));
	CreatePin(EGPD_Output, TEXT("QuestNode"), TEXT("Out"));
}

FText UQuestGraphNode::GetNodeTitle(ENodeTitleType::Type TitleType) const
{
	if(!QuestAsset)
	{
		return FText::FromString(TEXT("Invalid Quest Node"));
	}

	if(const FQuestNode* Node = QuestAsset->FindNodeById(NodeId))
	{
		return FText::FromName(Node->NodeName);
	}

	return FText::FromString(TEXT("Missing Quest Node"));
}

void UQuestGraphNode::SyncNodePositionToAsset() const
{
	if(!QuestAsset)
	{
		return;
	}

	// Persist the editor-only graph coordinates back onto the quest asset so layout survives closing and reopening the custom editor.
	if(FQuestNode* Node = QuestAsset->FindNodeById(NodeId))
	{
		Node->EditorPosition = FVector2D(NodePosX, NodePosY);
		QuestAsset->MarkPackageDirty();
	}
}

void UQuestGraphNode::SyncTransitionsToAsset()
{
	FQuestNode* Node = QuestAsset->FindNodeById(NodeId);
	if(!Node)
	{
		return;
	}

	Node->Transitions.Reset();

	UEdGraphPin* OutputPin = GetOutputPin();
	if(!OutputPin)
	{
		return;
	}

	for(UEdGraphPin* LinkedPin : OutputPin->LinkedTo)
	{
		if(!LinkedPin)
		{
			continue;
		}

		UQuestGraphNode* TargetNode = Cast<UQuestGraphNode>(LinkedPin->GetOwningNode());
		if(!TargetNode)
		{
			continue;
		}

		// Ignore self-links so we only serialize meaningful progression edges.
		if(TargetNode->NodeId == NodeId)
		{
			continue;
		}

		FQuestTransition NewTransition;
		NewTransition.TargetNodeId = TargetNode->NodeId;
		Node->Transitions.Add(NewTransition);
	}

	QuestAsset->MarkPackageDirty();
}

void UQuestGraphNode::NodeConnectionListChanged()
{
	Super::NodeConnectionListChanged();
	SyncTransitionsToAsset();
}

void UQuestGraphNode::PinConnectionListChanged(UEdGraphPin* Pin)
{
	Super::PinConnectionListChanged(Pin);
	SyncTransitionsToAsset();
}

UEdGraphPin* UQuestGraphNode::GetInputPin() const
{
	return Pins.Num() > 0 ? Pins[0] : nullptr;
}

UEdGraphPin* UQuestGraphNode::GetOutputPin() const
{
	return Pins.Num() > 1 ? Pins[1] : nullptr;
}
