#pragma once

#include "CoreMinimal.h"
#include "Toolkits/AssetEditorToolkit.h"
#include "UObject/GCObject.h"

class UQuestNodeEditorProxy;
class UQuestAsset;
class UQuestGraph;
class IDetailsView;
class SGraphEditor;

class QUESTFORGEEDITOR_API FQuestAssetEditor : public FAssetEditorToolkit, public FGCObject
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

	virtual void SaveAsset_Execute() override;

	/* Creates a new quest node at the given graph position. */
	void CreateNodeAtLocation(const FVector2D& GraphPosition);

	/** Refreshes the graph after a node's editable data has changed. */
	void RefreshGraphAfterNodeEdit();

private:
	/* The quest asset currently being edited. */
	TObjectPtr<UQuestAsset> QuestAsset;

	/* The editor graph generated from the runtime quest nodes. */
	TObjectPtr<UQuestGraph> Graph;

	/* Graph editor widget used to display and edit quest nodes visually. */
	TSharedPtr<SGraphEditor> GraphEditorWidget;

	/* Graph editor widget used to display and edit quest nodes visually. */
	TSharedPtr<IDetailsView> DetailsView;

	TSharedPtr<FUICommandList> GraphEditorCommands;

	TObjectPtr<UQuestNodeEditorProxy> NodeDetailsProxy;

private:

	/* Creates the internal graph object used by the graph editor widget. */
	void CreateInternalGraph();

	/* Creates graph nodes for each FQuestNode in the asset. */
	void CreateGraphNodes();

	/* Creates graph links based on each node's outgoing transition GUIDs. */
	void CreateGraphEdges();

	/* Rebuilds all graph nodes and edges from the quest asset's runtime data. */
	void RebuildGraphFromAsset();

	/* Binds editor commands such as deleting the currently selected graph nodes. */
	void BindEditorCommands();

	/* Returns whether there are any selected graph nodes that can currently be deleted. */
	bool CanDeleteSelectedNodes() const;

	/* Deletes the currently selected graph nodes and removes any transitions pointing to them. */
	void DeleteSelectedNodes();

	/* Saves all graph node positions back into the corresponding runtime quest nodes. */
	void SyncAllGraphNodePositionsToAsset();

	/* Saves the currently edited node proxy back into the quest asset, if one exists. */
	void SaveCurrentNodeDetailsProxy();

	/* Handles selection changes in the graph editor and updates the details panel. */
	void OnSelectedNodesChanged(const TSet<UObject*>& NewSelection);

	/* Spawns the graph tab for this asset editor. */
	TSharedRef<SDockTab> SpawnGraphTab(const FSpawnTabArgs& Args);

	/* Spawns the details tab for this asset editor. */
	TSharedRef<SDockTab> SpawnDetailsTab(const FSpawnTabArgs& Args);
};