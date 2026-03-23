// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "QuestTypes.h"
#include "Engine/DataAsset.h"
#include "WorldStateAsset.generated.h"

/* Broadcast when a fact changes in this world state. */
DECLARE_MULTICAST_DELEGATE_OneParam(FOnWorldStateFactChanged, FName);

/**
 * Authorable and runtime-duplicable QuestForge world state.
 * Saved instances live as assets on disk; runtime can duplicate one into a transient working copy.
 */
UCLASS(BlueprintType)
class QUESTFORGERUNTIME_API UWorldStateAsset : public UDataAsset
{
	GENERATED_BODY()

public:
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "World State")
	TMap<FName, int32> Facts;
	
	FOnWorldStateFactChanged OnFactChanged;

public:

	/* Returns true if this fact exists in either fact map. */
	UFUNCTION(BlueprintCallable, Category = "World State")
	bool HasFact(FName FactName) const;

	/* Returns the current int value for a fact. */
	UFUNCTION(BlueprintCallable, Category = "World State")
	int32 GetFact(FName FactName) const;
	/* Sets an int fact and broadcasts that it changed. */
	UFUNCTION(BlueprintCallable, Category = "World State")
	void SetFact(FName FactName, int32 Value);

	/* Evaluates a single quest condition against the world state. */
	UFUNCTION(BlueprintCallable, Category = "World State")
	bool EvaluateCondition(const FQuestCondition& Condition) const;
	/* Returns true only if all provided conditions pass. */
	UFUNCTION(BlueprintCallable, Category = "World State")
	bool EvaluateConditions(const TArray<FQuestCondition>& Conditions) const;

	/* Applies one quest effect to the current world state. */
	UFUNCTION(BlueprintCallable, Category = "World State")
	void ApplyEffect(const FQuestEffect& Effect);
	/* Applies each effect in order. */
	UFUNCTION(BlueprintCallable, Category = "World State")
	void ApplyEffects(const TArray<FQuestEffect>& Effects);
	
};
