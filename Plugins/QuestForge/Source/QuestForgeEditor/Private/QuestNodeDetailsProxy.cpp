// Fill out your copyright notice in the Description page of Project Settings.


#include "QuestNodeDetailsProxy.h"

#include "QuestAsset.h"
#include "QuestAssetEditor.h"
#include "QuestTypes.h"

void UQuestNodeDetailsProxy::PostEditChangeProperty(FPropertyChangedEvent& PropertyChangedEvent)
{
	Super::PostEditChangeProperty(PropertyChangedEvent);

	SaveToNode();

	const FName PropertyName = PropertyChangedEvent.GetPropertyName();

	if(PropertyName == GET_MEMBER_NAME_CHECKED(UQuestNodeDetailsProxy, NodeName))
	{
		if(TSharedPtr<FQuestAssetEditor> PinnedEditor = Editor.Pin())
		{
			PinnedEditor->RefreshGraphAfterNodeEdit();
		}
	}
}

void UQuestNodeDetailsProxy::LoadFromNode(UQuestAsset* InQuestAsset, const FGuid& InNodeId)
{
	QuestAsset = InQuestAsset;
	NodeId = InNodeId;

	if (FQuestNode* Node = QuestAsset->FindNodeById(NodeId))
	{
		NodeName = Node->NodeName;
		Transitions = Node->Transitions;
		Effects = Node->Effects;
	}
}

void UQuestNodeDetailsProxy::SaveToNode() const
{
	if (FQuestNode* Node = QuestAsset->FindNodeById(NodeId))
	{
		Node->NodeName = NodeName;
		Node->Transitions = Transitions;
		Node->Effects = Effects;

		QuestAsset->MarkPackageDirty();
	}
}
