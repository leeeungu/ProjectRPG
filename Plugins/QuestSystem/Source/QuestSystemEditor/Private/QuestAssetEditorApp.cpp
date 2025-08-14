#include "QuestAssetEditorApp.h"
#include "QuestAsset.h"
#include "QuestAssetAppMode.h"
#include "kismet2/BlueprintEditorUtils.h"
#include "QuestGraphSchema.h"
#include "QuestRuntimeGraph.h"
#include "QuestGraphNode.h"
#include "IDetailsView.h"
#include "QuestStartGraphNode.h"
#include "QuestNodeInfo.h"
#include "QuestEndGraphNode.h"


//DEFINE_LOG_CATEGORY_STATIC(QuestAssetEditorApp, Log, All);
//#include "uedgraphpannel"

//void QuestAssetEditorApp::RegisterTabSpawners(const TSharedRef< class FTabManager>& TabManager)
//{
//	FWorkflowCentricApplication::RegisterTabSpawners(TabManager);
//}

void QuestAssetEditorApp::InitEditor(const EToolkitMode::Type mode, const TSharedPtr<IToolkitHost>& initToolkitHost, UObject* inObject)
{
	TArray<UObject*> ObjectsToEdit{};
	ObjectsToEdit.Add(inObject);

	_workingAsset = Cast<UQuestAsset>(inObject);
    _workingAsset->SetPreSaveListener([this]() {OnWorkingAssetPreSave(); });
	_workingGraph = FBlueprintEditorUtils::CreateNewGraph(
		_workingAsset,
		NAME_None,
		UEdGraph::StaticClass(),
		UQuestGraphSchema::StaticClass()
	);



	InitAssetEditor(
		mode, initToolkitHost, TEXT("QuestSystem"), FTabManager::FLayout::NullLayout, true, true, ObjectsToEdit
	);

	AddApplicationMode(TEXT("QuestAppMode"), MakeShareable(new QuestAssetAppMode(SharedThis(this))));

	SetCurrentMode(TEXT("QuestAppMode"));

    UpdateEditorGraphFromWorkingAsset();
}

void QuestAssetEditorApp::SetSelectedNodeDetailView(TSharedPtr<IDetailsView> detailsView)
{
    _SelectedNodeDetailsView = detailsView;
    _SelectedNodeDetailsView->OnFinishedChangingProperties().AddRaw(this, &QuestAssetEditorApp::OnNodeDetailViewPropertiesUpdated);
}

void QuestAssetEditorApp::OnGraphSelectedChanged(const FGraphPanelSelectionSet& selection)
{
    UQuestGraphNodeBase* selectedNode = GetSelectedNode(selection);
    if (selectedNode)
    {
        _SelectedNodeDetailsView->SetObject(selectedNode->GetNodeInfo());
    }
    else
    _SelectedNodeDetailsView->SetObject(nullptr);
}

void QuestAssetEditorApp::OnClose()
{
    UpdateWorkingAssetFromGraph();
    _workingAsset->SetPreSaveListener(nullptr);
  //  _workingGraph->RemoveOnGraphChangedHandler(_graohChangeLisenerHandle);
    FAssetEditorToolkit::OnClose();
}

void QuestAssetEditorApp::OnNodeDetailViewPropertiesUpdated(const FPropertyChangedEvent& event)
{
    if (_workingGraphUI != nullptr)
    {
        UQuestGraphNodeBase* selectedNode = Cast< UQuestGraphNodeBase>(GetSelectedNode(_workingGraphUI->GetSelectedNodes()));
        if (selectedNode)
        {
            selectedNode->OnPropertiesChanged();
        }
        _workingGraphUI->NotifyGraphChanged();
    }
}


void QuestAssetEditorApp::OnWorkingAssetPreSave()
{

    UpdateWorkingAssetFromGraph();
}

void QuestAssetEditorApp::UpdateWorkingAssetFromGraph()
{
	if (_workingAsset == nullptr || _workingGraph == nullptr)
		return;
    // Update the state we need into our saveable format
    UQuestRuntimeGraph* runtimeGraph = NewObject<UQuestRuntimeGraph>(_workingAsset);
    _workingAsset->Graph = runtimeGraph;

    TArray<std::pair<FGuid, FGuid>> connections;
    TMap<FGuid, UQuestRuntimePin*> idToPinMap;

    // First create all the nodes/pins and record the connections
    for (UEdGraphNode* uiNode : _workingGraph->Nodes) {
        UQuestRuntimeNode* runtimeNode = NewObject<UQuestRuntimeNode>(runtimeGraph);
        runtimeNode->Position = FVector2D(uiNode->NodePosX, uiNode->NodePosY);
       // runtimeNode->QuestInfo = uiGraphNode->GetNodeInfo();

        for (UEdGraphPin* uiPin : uiNode->Pins) {
            UQuestRuntimePin* runtimePin = NewObject<UQuestRuntimePin>(runtimeNode);
            runtimePin->PinName = uiPin->PinName;
            runtimePin->PinId = uiPin->PinId;
            runtimePin->Parent = runtimeNode;
            // Only record the the output side of the connection since this is a directed graph
            if (uiPin->HasAnyConnections() && uiPin->Direction == EEdGraphPinDirection::EGPD_Output) {
                // Only 1 connection is allowed so just take the first one
                std::pair<FGuid, FGuid> connection = std::make_pair(uiPin->PinId, uiPin->LinkedTo[0]->PinId);
                connections.Add(connection);
            }

            idToPinMap.Add(uiPin->PinId, runtimePin);
            if (uiPin->Direction == EEdGraphPinDirection::EGPD_Input) {
                runtimeNode->InputPin = runtimePin;
            }
            else {
                runtimeNode->OutputPins.Add(runtimePin);
            }
        }
     
        UQuestGraphNodeBase* uiGraphNode = Cast<UQuestGraphNodeBase>(uiNode);
        runtimeNode->NodeType = uiGraphNode->GetQuestNodeType();
        runtimeNode->QuestInfo = DuplicateObject( uiGraphNode->GetNodeInfo(), runtimeNode);
        runtimeGraph->Nodes.Add(runtimeNode);
    }

    // Now make all the connections
    for (std::pair<FGuid, FGuid> connection : connections) {
        UQuestRuntimePin* pin1 = idToPinMap[connection.first];
        UQuestRuntimePin* pin2 = idToPinMap[connection.second];
        pin1->Connection = pin2;
    }
}
void QuestAssetEditorApp::UpdateEditorGraphFromWorkingAsset()
{
    if (_workingAsset->Graph == nullptr) {
         _workingGraph->GetSchema()->CreateDefaultNodesForGraph(*_workingGraph);
        return;
    }

    // Create all the nodes/pins first
    TArray<std::pair<FGuid, FGuid>> connections;
    TMap<FGuid, UEdGraphPin*> idToPinMap;
    for (UQuestRuntimeNode* runtimeNode : _workingAsset->Graph->Nodes) {
        UQuestGraphNodeBase* newNode{};
        if (runtimeNode->NodeType == EQuestNodeType::StartNode)
        {
            newNode = NewObject<UQuestStartGraphNode>(_workingGraph);
        }
        else if (runtimeNode->NodeType == EQuestNodeType::QuestNode)
        {
            newNode = NewObject<UQuestGraphNode>(_workingGraph);
        }
        else if (runtimeNode->NodeType == EQuestNodeType::EndNode)
        {
            newNode = NewObject<UQuestEndGraphNode>(_workingGraph);
        }
        else
        {
            //UE_LOG(QuestAssetEditorApp, Error, TEXT("QuestAssetEditorApp::UpdateEditorGraphFromWorkingAsset: Unknown node Type"));
            continue;
        }

        newNode->CreateNewGuid();
        newNode->NodePosX = runtimeNode->Position.X;
        newNode->NodePosY = runtimeNode->Position.Y;

        if (runtimeNode->QuestInfo)
        {
            runtimeNode->QuestInfo->pQuestAsset = _workingAsset;
            newNode->SetNodeInfo(DuplicateObject(runtimeNode->QuestInfo, runtimeNode));
        }
        else 
        {
            newNode->InitNodeInfo(newNode);
        }

        if (runtimeNode->InputPin != nullptr) {
            UQuestRuntimePin* pin = runtimeNode->InputPin;
            UEdGraphPin* uiPin = newNode->CreateQuestPin(EEdGraphPinDirection::EGPD_Input, pin->PinName);
            uiPin->PinId = pin->PinId;

            if (pin->Connection != nullptr) {
                connections.Add(std::make_pair(pin->PinId, pin->Connection->PinId));
            }
            idToPinMap.Add(pin->PinId, uiPin);
        }

        for (UQuestRuntimePin* pin : runtimeNode->OutputPins) {
            UEdGraphPin* uiPin = newNode->CreateQuestPin(EEdGraphPinDirection::EGPD_Output, pin->PinName);
            uiPin->PinId = pin->PinId;

            if (pin->Connection != nullptr) {
                connections.Add(std::make_pair(pin->PinId, pin->Connection->PinId));
            }
            idToPinMap.Add(pin->PinId, uiPin);
        }

        _workingGraph->AddNode(newNode, true, true);
    }

    for (std::pair<FGuid, FGuid> connection : connections) {
        UEdGraphPin* fromPin = idToPinMap[connection.first];
        UEdGraphPin* toPin = idToPinMap[connection.second];
        fromPin->LinkedTo.Add(toPin);
        toPin->LinkedTo.Add(fromPin);
    }
}

UQuestGraphNodeBase* QuestAssetEditorApp::GetSelectedNode(const FGraphPanelSelectionSet& selection)
{
    for (UObject* obj : selection)
    {
        UQuestGraphNodeBase* node = Cast< UQuestGraphNodeBase>(obj);
        if (node)
        {
        return node;
        }
    }
    return nullptr;
}