// Copyright Epic Games, Inc. All Rights Reserved.

#include "QuestForgeEditorModule.h"

#include "AssetTypeActions_QuestAsset.h"
#include "FQuestForgeSerializer.h"
#include "WorldStateAsset.h"
#include "QuestAsset.h"

#define LOCTEXT_NAMESPACE "FQuestForgeModule"

void FQuestForgeEditorModule::StartupModule()
{
	// This code will execute after your module is loaded into memory; the exact timing is specified in the .uplugin file per-module

	IAssetTools& AssetTools = FModuleManager::LoadModuleChecked<FAssetToolsModule>("AssetTools").Get();

	QuestAssetTypeActions = MakeShared<FAssetTypeActions_QuestAsset>();
	AssetTools.RegisterAssetTypeActions(QuestAssetTypeActions.ToSharedRef());

	/*UQuestAsset* QuestAsset = LoadObject<UQuestAsset>(nullptr, TEXT("/Game/QuestForgeDemo/Quests/MissingDaughter"));
	if(QuestAsset)
	{
		FString Json = FQuestForgeSerializer::SerializeQuestAsset(QuestAsset);
		UE_LOG(LogTemp, Warning, TEXT("\n%s"), *Json);
	}*/
}

void FQuestForgeEditorModule::ShutdownModule()
{
	// This function may be called during shutdown to clean up your module.  For modules that support dynamic reloading,
	// we call this function before unloading the module.

	if(FModuleManager::Get().IsModuleLoaded("AssetTools"))
	{
		IAssetTools& AssetTools = FModuleManager::GetModuleChecked<FAssetToolsModule>("AssetTools").Get();

		if(QuestAssetTypeActions.IsValid())
		{
			AssetTools.UnregisterAssetTypeActions(QuestAssetTypeActions.ToSharedRef());
		}
	}
}

#undef LOCTEXT_NAMESPACE
	
IMPLEMENT_MODULE(FQuestForgeEditorModule, QuestForgeEditor)