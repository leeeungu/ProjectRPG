#pragma once

#include "CoreMinimal.h"
#include "QuestNodeType.h"

class UQuestGraphNodeBase;
class UObject;

namespace QuestUtility
{
	bool CreateQuestGraphNodeBase(UObject* Outer, EQuestNodeType eTyp, UQuestGraphNodeBase*& rResult);
};
