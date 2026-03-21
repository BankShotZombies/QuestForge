#include "QuestAssetEditor.h"

#include "QuestAsset.h"
#include "QuestGraph.h"
#include "QuestGraphNode.h"
#include "QuestGraphSchema.h"
#include "QuestNodeDetailsProxy.h"
#include "Framework/Commands/GenericCommands.h"

const FName FQuestAssetEditor::GraphTabID(TEXT("QuestGraph"));
const FName FQuestAssetEditor::DetailsTabID(TEXT("QuestDetails"));

void FQuestAssetEditor::InitQuestAssetEditor(const EToolkitMode::Type Mode,
                                             const TSharedPtr<IToolkitHost>& InitToolkitHost, UQuestAsset* InQuestAsset)
{
	UE_LOG(LogTemp, Warning, TEXT("InitQuestAssetEditor start"));
	
	QuestAsset = InQuestAsset;

	CreateInternalGraph();
	RebuildGraphFromAsset();

	FPropertyEditorModule& PropertyEditorModule = FModuleManager::LoadModuleChecked<FPropertyEditorModule>("PropertyEditor");

	FDetailsViewArgs DetailsArgs;
	DetailsView = PropertyEditorModule.CreateDetailView(DetailsArgs);
	DetailsView->SetObject(QuestAsset.Get());

	FGraphAppearanceInfo AppearanceInfo;
	AppearanceInfo.CornerText = FText::FromString(TEXT("QuestForge"));

	BindEditorCommands();

	SGraphEditor::FGraphEditorEvents GraphEvents;
	GraphEvents.OnSelectionChanged = SGraphEditor::FOnSelectionChanged::CreateRaw(this, &FQuestAssetEditor::OnSelectedNodesChanged);

	GraphEditorWidget = SNew(SGraphEditor).AdditionalCommands(GraphEditorCommands).Appearance(AppearanceInfo).GraphToEdit(Graph).GraphEvents(GraphEvents);

	const TSharedRef<FTabManager::FLayout> Layout = FTabManager::NewLayout("QuestAssetEditorLayout")
	->AddArea
	(
		FTabManager::NewPrimaryArea()->SetOrientation(Orient_Horizontal)
		->Split
		(
			FTabManager::NewStack()
			->AddTab(GraphTabID, ETabState::OpenedTab)
		)
		->Split
		(
			FTabManager::NewStack()
			->AddTab(DetailsTabID, ETabState::OpenedTab)
			->SetSizeCoefficient(0.3f)
		)
	);

	InitAssetEditor(Mode, InitToolkitHost, FName("QuestAssetEditorApp"), Layout, true, true, InQuestAsset);

	RegenerateMenusAndToolbars();

	UE_LOG(LogTemp, Warning, TEXT("InitQuestAssetEditor end"));
}

FName FQuestAssetEditor::GetToolkitFName() const
{
	return FName("QuestAssetEditor");
}

FText FQuestAssetEditor::GetBaseToolkitName() const
{
	return FText::FromString(TEXT("Quest Asset Editor"));
}

FString FQuestAssetEditor::GetWorldCentricTabPrefix() const
{
	return TEXT("QuestAsset");
}

FLinearColor FQuestAssetEditor::GetWorldCentricTabColorScale() const
{
	return FLinearColor(0.2f, 0.2f, 0.8f, 0.5f);
}

void FQuestAssetEditor::AddReferencedObjects(FReferenceCollector& Collector)
{
	Collector.AddReferencedObject(QuestAsset);
	Collector.AddReferencedObject(Graph);
	Collector.AddReferencedObject(NodeDetailsProxy);
}

void FQuestAssetEditor::RegisterTabSpawners(const TSharedRef<FTabManager>& InTabManager)
{
	FAssetEditorToolkit::RegisterTabSpawners(InTabManager);

	InTabManager->RegisterTabSpawner(GraphTabID, FOnSpawnTab::CreateSP(this, &FQuestAssetEditor::SpawnGraphTab));
	InTabManager->RegisterTabSpawner(DetailsTabID, FOnSpawnTab::CreateSP(this, &FQuestAssetEditor::SpawnDetailsTab));
}

void FQuestAssetEditor::UnregisterTabSpawners(const TSharedRef<FTabManager>& InTabManager)
{
	FAssetEditorToolkit::UnregisterTabSpawners(InTabManager);

	InTabManager->UnregisterTabSpawner(GraphTabID);
	InTabManager->UnregisterTabSpawner(DetailsTabID);
}

void FQuestAssetEditor::SaveAsset_Execute()
{
	SaveCurrentNodeDetailsProxy();
	SyncAllGraphNodePositionsToAsset();
	FAssetEditorToolkit::SaveAsset_Execute();
}

void FQuestAssetEditor::CreateInternalGraph()
{
	Graph = NewObject<UQuestGraph>(QuestAsset.Get(), NAME_None, RF_Transactional);
	Graph->Schema = UQuestGraphSchema::StaticClass();
	Graph->Editor = SharedThis(this);
}

void FQuestAssetEditor::RebuildGraphFromAsset()
{
	if(!Graph || !QuestAsset)
	{
		return;
	}

	bIsRebuildingGraph = true;

	Graph->Modify();
	Graph->Nodes.Reset();

	CreateGraphNodes();
	CreateGraphEdges();

	bIsRebuildingGraph = false;
}

void FQuestAssetEditor::CreateGraphNodes()
{
	for(const FQuestNode& Node : QuestAsset->Nodes)
	{
		UQuestGraphNode* GraphNode = NewObject<UQuestGraphNode>(Graph, NAME_None, RF_Transactional);
		GraphNode->Initialize(QuestAsset, Node.NodeId);
		GraphNode->CreateNewGuid();
		GraphNode->NodePosX = Node.EditorPosition.X;
		GraphNode->NodePosY = Node.EditorPosition.Y;
		GraphNode->AllocateDefaultPins();

		Graph->AddNode(GraphNode, true, false);
	}
}

void FQuestAssetEditor::CreateGraphEdges()
{
	TMap<FGuid, UQuestGraphNode*> NodeMap;

	for(UEdGraphNode* EdNode : Graph->Nodes)
	{
		if(UQuestGraphNode* QuestGraphNode = Cast<UQuestGraphNode>(EdNode))
		{
			NodeMap.Add(QuestGraphNode->NodeId, QuestGraphNode);
		}
	}

	for(const FQuestNode& Node : QuestAsset->Nodes)
	{
		UQuestGraphNode** SourceNodePtr = NodeMap.Find(Node.NodeId);
		if(!SourceNodePtr)
		{
			continue;
		}

		UQuestGraphNode* SourceNode = *SourceNodePtr;
		UEdGraphPin* OutputPin = SourceNode->GetOutputPin();
		if(!OutputPin)
		{
			continue;
		}

		for(const FQuestTransition& Transition : Node.Transitions)
		{
			UQuestGraphNode** TargetNodePtr = NodeMap.Find(Transition.TargetNodeId);
			if(!TargetNodePtr)
			{
				continue;
			}

			UQuestGraphNode* TargetNode = *TargetNodePtr;
			UEdGraphPin* InputPin = TargetNode->GetInputPin();
			if(!InputPin)
			{
				continue;
			}

			OutputPin->MakeLinkTo(InputPin);
		}
	}
}

void FQuestAssetEditor::BindEditorCommands()
{
	GraphEditorCommands = MakeShared<FUICommandList>();

	GraphEditorCommands->MapAction(FGenericCommands::Get().Delete, FExecuteAction::CreateRaw(this, &FQuestAssetEditor::DeleteSelectedNodes),
		FCanExecuteAction::CreateRaw(this, &FQuestAssetEditor::CanDeleteSelectedNodes));
}

void FQuestAssetEditor::CreateNodeAtLocation(const FVector2D& GraphPosition)
{
	if(!QuestAsset)
	{
		return;
	}

	QuestAsset->Modify();

	FQuestNode& NewNode = QuestAsset->Nodes.AddDefaulted_GetRef();
	NewNode.NodeId = FGuid::NewGuid();
	NewNode.NodeName = TEXT("NewNode");
	NewNode.EditorPosition = GraphPosition;

	QuestAsset->MarkPackageDirty();

	RebuildGraphFromAsset();
}

void FQuestAssetEditor::RefreshGraphAfterNodeEdit()
{
	// TODO: If node name is the only thing to refresh in the graph, can I just do that instead of rebuilding the whole graph? 
	RebuildGraphFromAsset();
}

void FQuestAssetEditor::SaveCurrentNodeDetailsProxy()
{
	if(NodeDetailsProxy)
	{
		NodeDetailsProxy->SaveToNode();
	}
}

bool FQuestAssetEditor::CanDeleteSelectedNodes() const
{
	return GraphEditorWidget.IsValid() && GraphEditorWidget->GetSelectedNodes().Num() > 0;
}

void FQuestAssetEditor::DeleteSelectedNodes()
{
	const FGraphPanelSelectionSet SelectedNodes = GraphEditorWidget->GetSelectedNodes();

	TSet<FGuid> DeletedNodeIds;

	for(UObject* SelectedObject : SelectedNodes)
	{
		if (UQuestGraphNode* QuestGraphNode = Cast<UQuestGraphNode>(SelectedObject))
		{
			DeletedNodeIds.Add(QuestGraphNode->NodeId);
		}
	}

	if(DeletedNodeIds.Num() == 0)
	{
		return;
	}

	QuestAsset->Modify();

	QuestAsset->Nodes.RemoveAll([&](const FQuestNode& Node)
	{
		return DeletedNodeIds.Contains(Node.NodeId);
	});

	for(FQuestNode& Node : QuestAsset->Nodes)
	{
		Node.Transitions.RemoveAll([&](const FQuestTransition& Transition)
		{
			return DeletedNodeIds.Contains(Transition.TargetNodeId);
		});
	}

	QuestAsset->MarkPackageDirty();

	RebuildGraphFromAsset();
	DetailsView->SetObject(QuestAsset.Get());

}

void FQuestAssetEditor::SyncAllGraphNodePositionsToAsset()
{
	for(UEdGraphNode* EdNode : Graph->Nodes)
	{
		if(UQuestGraphNode* QuestGraphNode = Cast<UQuestGraphNode>(EdNode))
		{
			QuestGraphNode->SyncNodePositionToAsset();
		}
	}
}

void FQuestAssetEditor::OnSelectedNodesChanged(const TSet<UObject*>& NewSelection)
{
	SaveCurrentNodeDetailsProxy();
	
	if(NewSelection.Num() == 1)
	{
		for(UObject* SelectedObject : NewSelection)
		{
			if(UQuestGraphNode* QuestGraphNode = Cast<UQuestGraphNode>(SelectedObject))
			{
				NodeDetailsProxy = NewObject<UQuestNodeDetailsProxy>();
				NodeDetailsProxy->Editor = SharedThis(this);
				NodeDetailsProxy->LoadFromNode(QuestAsset, QuestGraphNode->NodeId);
				DetailsView->SetObject(NodeDetailsProxy);
				return;
			}
		}
	}

	NodeDetailsProxy = nullptr;
	DetailsView->SetObject(QuestAsset);
}

TSharedRef<SDockTab> FQuestAssetEditor::SpawnGraphTab(const FSpawnTabArgs& Args)
{
	return SNew(SDockTab)
	[
		GraphEditorWidget.ToSharedRef()
	];
}

TSharedRef<SDockTab> FQuestAssetEditor::SpawnDetailsTab(const FSpawnTabArgs& Args)
{
	return SNew(SDockTab)
	[
		DetailsView.ToSharedRef()
	];
}
