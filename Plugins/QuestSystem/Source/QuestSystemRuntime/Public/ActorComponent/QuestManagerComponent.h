#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "QuestManagerComponent.generated.h"

class UQuestAsset;
class AQuestObject;

DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FOnQuestClear, UQuestAsset*, TargetQuest);

UCLASS( ClassGroup=(Custom), meta=(BlueprintSpawnableComponent) )
class QUESTSYSTEMRUNTIME_API UQuestManagerComponent : public UActorComponent
{
	GENERATED_BODY()
protected:
	UPROPERTY()
	TMap<UQuestAsset*, AQuestObject*> mapQuestObject;
public:
	UPROPERTY(BlueprintAssignable, BlueprintCallable)
	FOnQuestClear OnQuestClear{};
public:	
	UQuestManagerComponent();

protected:
	virtual void BeginPlay() override;

	UFUNCTION()
	void QuestEnd(UQuestAsset* pQuest);
	UFUNCTION()
	void QuestSucceed(UQuestAsset* pQuest);
	UFUNCTION()
	void QuestFail(UQuestAsset* pQuest);
public:
	UFUNCTION(BlueprintCallable)
	bool pushQuest(UQuestAsset* pQuest);
};
