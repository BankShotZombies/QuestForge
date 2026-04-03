#pragma once

#include "QuestTypes.generated.h"

UENUM(BlueprintType)
enum class EQuestCompareOp : uint8
{
	Equal,
	NotEqual,
	GreaterThan,
	GreaterThanOrEqual,
	LessThan,
	LessThanOrEqual
};

USTRUCT(BlueprintType)
struct FQuestCondition
{
	GENERATED_BODY()

public:
	UPROPERTY(EditAnywhere, BlueprintReadOnly)
	FName FactName;

	UPROPERTY(EditAnywhere, BlueprintReadOnly)
	EQuestCompareOp	Op = EQuestCompareOp::Equal;

	UPROPERTY(EditAnywhere, BlueprintReadOnly)
	int32 Value = 0;
};

USTRUCT(BlueprintType)
struct FQuestTransition
{
	GENERATED_BODY()

	UPROPERTY(EditAnywhere, BlueprintReadOnly)
	FGuid TargetNodeId;

	UPROPERTY(EditAnywhere, BlueprintReadOnly)
	TArray<FQuestCondition> Preconditions;
};

USTRUCT(BlueprintType)
struct FQuestEffect
{
	GENERATED_BODY()

public:
	UPROPERTY(EditAnywhere, BlueprintReadOnly)
	FName FactName;

	UPROPERTY(EditAnywhere, BlueprintReadOnly)
	int32 Value = 0;
};

USTRUCT(BlueprintType)
struct FQuestNode
{
	GENERATED_BODY()

public:
	FQuestNode() : NodeId(FGuid::NewGuid()) {}

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category="Quest")
	FName NodeName;
	
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category="Quest")
	FGuid NodeId = FGuid();

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category="Quest")
	TArray<FQuestTransition> Transitions;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category="Quest")
	TArray<FQuestEffect> Effects;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category="Editor")
	FVector2D EditorPosition = FVector2D::ZeroVector;
};