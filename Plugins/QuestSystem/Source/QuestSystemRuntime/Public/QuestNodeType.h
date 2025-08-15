#pragma once

#include "CoreMinimal.h"
#include "QuestNodeType.generated.h"

UENUM(BlueprintType)
enum class EQuestNodeType : uint8
{
	UnKnown,
	StartNode,
	QuestNode,
	EndNode
};

