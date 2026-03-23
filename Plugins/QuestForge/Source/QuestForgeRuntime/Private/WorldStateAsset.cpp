// Fill out your copyright notice in the Description page of Project Settings.


#include "WorldStateAsset.h"

bool UWorldStateAsset::HasFact(FName FactName) const
{
	return Facts.Contains(FactName);
}

int32 UWorldStateAsset::GetFact(FName FactName) const
{
	return Facts.FindRef(FactName);
}

void UWorldStateAsset::SetFact(FName FactName, int32 Value)
{
	Facts[FactName] = Value;

	OnFactChanged.Broadcast(FactName);
}

bool UWorldStateAsset::EvaluateCondition(const FQuestCondition& Condition) const
{
	switch(Condition.Op)
	{
	case EQuestCompareOp::Equal:
		return GetFact(Condition.FactName) == Condition.Value;
	case EQuestCompareOp::NotEqual:
		return GetFact(Condition.FactName) != Condition.Value;
	case EQuestCompareOp::GreaterThan:
		return GetFact(Condition.FactName) > Condition.Value;
	case EQuestCompareOp::LessThan:
		return GetFact(Condition.FactName) < Condition.Value;
	case EQuestCompareOp::GreaterThanOrEqual:
		return GetFact(Condition.FactName) >= Condition.Value;
	case EQuestCompareOp::LessThanOrEqual:
		return GetFact(Condition.FactName) <= Condition.Value;
	default:
		return false;
	}
}

bool UWorldStateAsset::EvaluateConditions(const TArray<FQuestCondition>& Conditions) const
{
	for(const FQuestCondition& Condition : Conditions)
	{
		if(!EvaluateCondition(Condition))
		{
			return false;
		}		
	}

	return true;
}

void UWorldStateAsset::ApplyEffect(const FQuestEffect& Effect)
{
	SetFact(Effect.FactName, Effect.Value);
}

void UWorldStateAsset::ApplyEffects(const TArray<FQuestEffect>& Effects)
{
	for(const FQuestEffect& Effect : Effects)
	{
		ApplyEffect(Effect);
	}
}
