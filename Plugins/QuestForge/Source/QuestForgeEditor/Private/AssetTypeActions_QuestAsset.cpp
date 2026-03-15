#include "AssetTypeActions_QuestAsset.h"

#include "QuestAsset.h"
#include "QuestAssetEditor.h"

FText FAssetTypeActions_QuestAsset::GetName() const
{
	return FText::FromString(TEXT("Quest Asset"));
}

FColor FAssetTypeActions_QuestAsset::GetTypeColor() const
{
	return FColor(100, 200, 255);
}

UClass* FAssetTypeActions_QuestAsset::GetSupportedClass() const
{
	return UQuestAsset::StaticClass();
}

uint32 FAssetTypeActions_QuestAsset::GetCategories()
{
	return EAssetTypeCategories::Gameplay;
}

void FAssetTypeActions_QuestAsset::OpenAssetEditor(const TArray<UObject*>& InObjects,
	TSharedPtr<IToolkitHost> EditWithinLevelEditor)
{
	for(UObject* Object : InObjects)
	{
		if(UQuestAsset* QuestAsset = Cast<UQuestAsset>(Object))
		{
			TSharedRef<FQuestAssetEditor> Editor = MakeShared<FQuestAssetEditor>();
			Editor->InitQuestAssetEditor(EToolkitMode::Standalone, EditWithinLevelEditor, QuestAsset);
		}
	}
}

