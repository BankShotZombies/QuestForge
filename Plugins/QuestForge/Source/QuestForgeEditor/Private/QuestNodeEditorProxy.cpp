// Fill out your copyright notice in the Description page of Project Settings.


#include "QuestNodeEditorProxy.h"

#include "QuestAsset.h"
#include "QuestAssetEditor.h"
#include "QuestTypes.h"

void UQuestNodeEditorProxy::PostEditChangeProperty(FPropertyChangedEvent& PropertyChangedEvent)
{
	Super::PostEditChangeProperty(PropertyChangedEvent);

	SaveToNode();

	const FName PropertyName = PropertyChangedEvent.GetPropertyName();

	if(PropertyName == GET_MEMBER_NAME_CHECKED(UQuestNodeEditorProxy, NodeName))
	{
		if(TSharedPtr<FQuestAssetEditor> PinnedEditor = Editor.Pin())
		{
			PinnedEditor->RefreshGraphAfterNodeEdit();
		}
	}
}

void UQuestNodeEditorProxy::LoadFromNode(UQuestAsset* InQuestAsset, const FGuid& InNodeId)
{
	QuestAsset = InQuestAsset;
	NodeId = InNodeId;

	if (FQuestNode* Node = QuestAsset->FindNodeById(NodeId))
	{
		NodeName = Node->NodeName;
		Effects = Node->Effects;

		Transitions.Reset();

		for(const FQuestTransition& Transition : Node->Transitions)
		{
			FQuestTransitionEditorData& TransitionDetails = Transitions.AddDefaulted_GetRef();
			TransitionDetails.TargetNodeId = Transition.TargetNodeId;
			TransitionDetails.Preconditions = Transition.Preconditions;

			if(const FQuestNode* TargetNode = QuestAsset->FindNodeById(Transition.TargetNodeId))
			{
				TransitionDetails.TargetNodeName = TargetNode->NodeName;
			}
			else
			{
				TransitionDetails.TargetNodeName = NAME_None;
			}
		}
	}
}

void UQuestNodeEditorProxy::SaveToNode() const
{
	if (FQuestNode* Node = QuestAsset->FindNodeById(NodeId))
	{
		Node->NodeName = NodeName;
		Node->Effects = Effects;

		Node->Transitions.Reset();

		for (const FQuestTransitionEditorData& TransitionDetails : Transitions)
		{
			FQuestTransition& Transition = Node->Transitions.AddDefaulted_GetRef();
			Transition.TargetNodeId = TransitionDetails.TargetNodeId;
			Transition.Preconditions = TransitionDetails.Preconditions;
		}

		QuestAsset->MarkPackageDirty();
	}
}
