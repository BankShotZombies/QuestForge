#include "FQuestForgeSerializer.h"

#include "JsonObjectConverter.h"
#include "QuestAsset.h"
#include "WorldStateAsset.h"


FString FQuestForgeSerializer::SerializeQuestAsset(const UQuestAsset* QuestAsset)
{
	TSharedPtr<FJsonObject> QuestJson = MakeShared<FJsonObject>();

	QuestJson->SetStringField("name", QuestAsset->QuestName.ToString());

	TArray<TSharedPtr<FJsonValue>> NodesJsonArray;

	for(const FQuestNode& Node : QuestAsset->Nodes)
	{
		TSharedPtr<FJsonObject> NodeJson = MakeShared<FJsonObject>();

		NodeJson->SetStringField("name", Node.NodeName.ToString());
		NodeJson->SetStringField("id", Node.NodeId.ToString());

		{
			TArray<TSharedPtr<FJsonValue>> EffectsJsonArray;

			for(const FQuestEffect& Effect : Node.Effects)
			{
				TSharedPtr<FJsonObject> EffectJson = MakeShared<FJsonObject>();

				EffectJson->SetStringField("fact", Effect.FactName.ToString());
				EffectJson->SetNumberField("value", Effect.Value);

				EffectsJsonArray.Add(MakeShared<FJsonValueObject>(EffectJson));
			}

			NodeJson->SetArrayField("effects", EffectsJsonArray);
		}

		{
			TArray<TSharedPtr<FJsonValue>> TransitionsJsonArray;

			for(const FQuestTransition& Transition : Node.Transitions)
			{
				TSharedPtr<FJsonObject> TransitionJson = MakeShared<FJsonObject>();

				TransitionJson->SetStringField("targetNodeId", Transition.TargetNodeId.ToString());

				TArray<TSharedPtr<FJsonValue>> PreconditionsJsonArray;

				for(const FQuestCondition& Precondition : Transition.Preconditions)
				{
					TSharedPtr<FJsonObject> PreconditionJson = MakeShared<FJsonObject>();
					PreconditionJson->SetStringField("fact", Precondition.FactName.ToString());
					PreconditionJson->SetStringField("op", UEnum::GetDisplayValueAsText(Precondition.Op).ToString());
					PreconditionJson->SetNumberField("value", Precondition.Value);

					PreconditionsJsonArray.Add(MakeShared<FJsonValueObject>(PreconditionJson));
				}

				TransitionJson->SetArrayField("preconditions", PreconditionsJsonArray);

				TransitionsJsonArray.Add(MakeShared<FJsonValueObject>(TransitionJson));
			}

			NodeJson->SetArrayField("transitions", TransitionsJsonArray);
		}
		
		NodesJsonArray.Add(MakeShared<FJsonValueObject>(NodeJson));
	}

	QuestJson->SetArrayField("nodes", NodesJsonArray);

	FString OutputString;
	TSharedRef<TJsonWriter<>> JsonWriter = TJsonWriterFactory<>::Create(&OutputString);
	FJsonSerializer::Serialize(QuestJson.ToSharedRef(), JsonWriter);

	return OutputString;
}

FString FQuestForgeSerializer::SerializeWorldStateAsset(const UWorldStateAsset* WorldStateAsset)
{
	TSharedPtr<FJsonObject> WorldStateJson = MakeShared<FJsonObject>();

	WorldStateJson->SetStringField("name", WorldStateAsset->GetName());

	TSharedPtr<FJsonObject> FactsJson = MakeShared<FJsonObject>();

	for (const TPair<FName, int32>& Pair : WorldStateAsset->Facts)
	{
		FactsJson->SetNumberField(Pair.Key.ToString(), Pair.Value);
	}

	WorldStateJson->SetObjectField("facts", FactsJson);

	FString OutputString;
	TSharedRef<TJsonWriter<>> JsonWriter = TJsonWriterFactory<>::Create(&OutputString);
	FJsonSerializer::Serialize(WorldStateJson.ToSharedRef(), JsonWriter);

	return OutputString;
}
