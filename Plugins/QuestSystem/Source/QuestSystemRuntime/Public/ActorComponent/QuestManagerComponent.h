#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "QuestManagerComponent.generated.h"

class UQuestAsset;
class AQuestObject;

DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FOnQuestClear, UQuestAsset*, TargetQuest);
DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FOnQuestManagerEvent, UQuestAsset*, TargetQuest);

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

	UPROPERTY(BlueprintAssignable, BlueprintCallable)
	FOnQuestManagerEvent OnQuestPush{};
	UPROPERTY(BlueprintAssignable, BlueprintCallable)
	FOnQuestManagerEvent OnQuestPop{};
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

	UFUNCTION(BlueprintPure)
	TArray< UQuestAsset*> getQuestAsset() const;
};
