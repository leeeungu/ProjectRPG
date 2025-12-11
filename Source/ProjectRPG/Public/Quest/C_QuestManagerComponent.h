// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "GamePlay/C_DataManagerInterface.h"
#include "C_QuestManagerComponent.generated.h"

class UQuestAsset;
class AQuestObject;

DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FOnQuestClear, UQuestAsset*, TargetQuest);
DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FOnQuestManagerEvent, UQuestAsset*, TargetQuest);

UCLASS(ClassGroup = (Custom), meta = (BlueprintSpawnableComponent))
class PROJECTRPG_API UC_QuestManagerComponent : public UActorComponent, public IC_DataManagerInterface
{
	GENERATED_BODY()
private:
	struct SQuestSaveData
	{
		int nSize{};
		TArray<FString> mapQuestObject{};

		friend FArchive& operator<<(FArchive& Ar, SQuestSaveData* Data)
		{
			Ar << Data->nSize;
			if (Data->mapQuestObject.IsEmpty())
			{
				Data->mapQuestObject.SetNum(Data->nSize);
			}
			for (FString& pAsset : Data->mapQuestObject)
			{
				Ar << pAsset;
			}
			return Ar;
		}
	};
protected:
	UPROPERTY()
	TMap<UQuestAsset*, AQuestObject*> m_mapQuestObject;
public:
	UPROPERTY(BlueprintAssignable, BlueprintCallable)
	FOnQuestClear OnQuestClear{};

	UPROPERTY(BlueprintAssignable, BlueprintCallable)
	FOnQuestManagerEvent OnQuestPush{};
	UPROPERTY(BlueprintAssignable, BlueprintCallable)
	FOnQuestManagerEvent OnQuestPop{};
public:
	UC_QuestManagerComponent();

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

	UFUNCTION(BlueprintPure)
	bool getHasQuest(UQuestAsset* pQuest) const;

	UFUNCTION(BlueprintCallable)
	bool clearQuest(UQuestAsset* pQuest, bool bSucceed);

	virtual void EndPlay(const EEndPlayReason::Type EndPlayReason) override;

	// IC_DataManagerInterface을(를) 통해 상속됨
public:
	virtual E_DataType getDataType() override;
	virtual FString getFilePath(E_DataType eType) override;

	virtual void loadBinaryData(TArray<uint8>& arData) override;
	virtual TArray<uint8> getBinaryData() override;
};