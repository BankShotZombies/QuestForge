#pragma once

#include "CoreMinimal.h"
#include "Toolkits/AssetEditorToolkit.h"
#include "UObject/GCObject.h"

class UQuestAsset;
class UQuestGraph;
class IDetailsView;
class SGraphEditor;

class FQuestAssetEditor : public FAssetEditorToolkit, public FGCObject
{
public:

	static const FName GraphTabID;
	static const FName DetailsTabID;
	
	/* Opens the custom editor for the given quest asset. */
	void InitQuestAssetEditor(const EToolkitMode::Type Mode, const TSharedPtr<IToolkitHost>& InitToolkitHost, UQuestAsset* InQuestAsset);

	/** Opens the custom editor for the given quest asset. */
	virtual FName GetToolkitFName() const override;
	virtual FText GetBaseToolkitName() const override;
	virtual FString GetWorldCentricTabPrefix() const override;
	virtual FLinearColor GetWorldCentricTabColorScale() const override;

	virtual void AddReferencedObjects(FReferenceCollector& Collector) override;
	virtual FString GetReferencerName() const override
	{
		return TEXT("FQuestAssetEditor");
	}

	virtual void RegisterTabSpawners(const TSharedRef<FTabManager>& InTabManager) override;
	virtual void UnregisterTabSpawners(const TSharedRef<FTabManager>& InTabManager) override;

private:
	/* The quest asset currently being edited. */
	TObjectPtr<UQuestAsset> QuestAsset;

	/* The editor graph generated from the runtime quest nodes. */
	TObjectPtr<UQuestGraph> Graph;

	/* Graph editor widget used to display and edit quest nodes visually. */
	TSharedPtr<SGraphEditor> GraphEditorWidget;

	/* Graph editor widget used to display and edit quest nodes visually. */
	TSharedPtr<IDetailsView> DetailsView;

	bool bIsRebuildingGraph = false;

private:

	/* Creates the internal graph object used by the graph editor widget. */
	void CreateInternalGraph();

	/* Rebuilds all graph nodes and edges from the quest asset's runtime data. */
	void RebuildGraphFromAsset();

	/* Creates graph nodes for each FQuestNode in the asset. */
	void CreateGraphNodes();

	/* Creates graph links based on each node's outgoing transition GUIDs. */
	void CreateGraphEdges();

	/* Creates a new quest node at the given graph position. */
	void CreateNodeAtLocation(const FVector2D& GraphPosition);

	/* Saves all graph node positions back into the corresponding runtime quest nodes. */
	void SyncAllGraphNodePositionsToAsset();

	/* Handles selection changes in the graph editor and updates the details panel. */
	void OnSelectedNodesChanged(const TSet<UObject*>& NewSelection);

	/* Spawns the graph tab for this asset editor. */
	TSharedRef<SDockTab> SpawnGraphTab(const FSpawnTabArgs& Args);

	/* Spawns the details tab for this asset editor. */
	TSharedRef<SDockTab> SpawnDetailsTab(const FSpawnTabArgs& Args);
};