// Fill out your copyright notice in the Description page of Project Settings.


#include "QuestAsset.h"

FQuestNode* UQuestAsset::FindNodeById(const FGuid& NodeId)
{
	return Nodes.FindByPredicate([&](const FQuestNode& Node)
	{
		return Node.NodeId == NodeId;
	});
}

const FQuestNode* UQuestAsset::FindNodeById(const FGuid& NodeId) const
{
	return Nodes.FindByPredicate([&](const FQuestNode& Node)
	{
		return Node.NodeId == NodeId;
	});
}
