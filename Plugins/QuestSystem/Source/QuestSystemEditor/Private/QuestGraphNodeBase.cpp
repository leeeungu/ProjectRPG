#include "QuestGraphNodeBase.h"

void UQuestGraphNodeBase::UpdateQuestGraphNodeBase()
{
	GetGraph()->NotifyNodeChanged(this);
	GetGraph()->Modify();
}
