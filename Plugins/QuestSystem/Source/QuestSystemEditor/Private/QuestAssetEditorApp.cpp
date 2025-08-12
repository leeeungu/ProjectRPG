#include "QuestAssetEditorApp.h"
#include "QuestAsset.h"
#include "QuestAssetAppMode.h"
#include "kismet2/BlueprintEditorUtils.h"
#include "QuestGraphSchema.h"
#include "QuestRuntimeGraph.h"
#include "QuestGraphNode.h"

//void QuestAssetEditorApp::RegisterTabSpawners(const TSharedRef< class FTabManager>& TabManager)
//{
//	FWorkflowCentricApplication::RegisterTabSpawners(TabManager);
//}


void QuestAssetEditorApp::InitEditor(const EToolkitMode::Type mode, const TSharedPtr<IToolkitHost>& initToolkitHost, UObject* inObject)
{
	TArray<UObject*> ObjectsToEdit{};
	ObjectsToEdit.Add(inObject);

	_workingAsset = Cast<UQuestAsset>(inObject);
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
    _graohChangeLisenerHandle = _workingGraph->AddOnGraphChangedHandler(
        FOnGraphChanged::FDelegate::CreateSP(this, &QuestAssetEditorApp::OnGraphChanged)
    );
}

void QuestAssetEditorApp::OnClose()
{
    UpdateWorkingAssetFromGraph();
    _workingGraph->RemoveOnGraphChangedHandler(_graohChangeLisenerHandle);
    FAssetEditorToolkit::OnClose();
}


void QuestAssetEditorApp::OnGraphChanged(const FEdGraphEditAction& InAction)
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

        for (UEdGraphPin* uiPin : uiNode->Pins) {
            UQuestRuntimePin* runtimePin = NewObject<UQuestRuntimePin>(runtimeNode);
            runtimePin->PinName = uiPin->PinName;
            runtimePin->PinId = uiPin->PinId;

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
        return;
    }

    // Create all the nodes/pins first
    TArray<std::pair<FGuid, FGuid>> connections;
    TMap<FGuid, UEdGraphPin*> idToPinMap;
    for (UQuestRuntimeNode* runtimeNode : _workingAsset->Graph->Nodes) {
        UQuestGraphNode* newNode = NewObject<UQuestGraphNode>(_workingGraph);
        newNode->CreateNewGuid();
        newNode->NodePosX = runtimeNode->Position.X;
        newNode->NodePosY = runtimeNode->Position.Y;

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
