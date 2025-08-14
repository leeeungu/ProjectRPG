#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "C_QuestManager.generated.h"

class UQuestEndNodeInfo;
class UQuestAsset;

UCLASS( ClassGroup=(Custom), meta=(BlueprintSpawnableComponent) )
class PROJECTRPG_API UC_QuestManager : public UActorComponent
{
	GENERATED_BODY()
protected:
	UPROPERTY()
	TSet< UQuestAsset*> m_psetQuest;

public:	
	UC_QuestManager();

protected:
	virtual void BeginPlay() override;

public:	
	UFUNCTION(BlueprintCallable)
	bool pushQuest(UQuestEndNodeInfo* pEndNode);
};
