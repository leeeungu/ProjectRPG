#pragma once

#include "CoreMinimal.h"
#include "QuestNodeType.generated.h"

UENUM()
enum class EQuestNodeType
{
	UnKnown,
	StartNode,
	QuestNode,
	EndNode
};
