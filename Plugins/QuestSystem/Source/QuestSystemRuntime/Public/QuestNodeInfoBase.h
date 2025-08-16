// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "QuestNodeInfoBase.generated.h"

class UQuestAsset;

UCLASS(BlueprintType)
class QUESTSYSTEMRUNTIME_API UQuestNodeInfoBase : public UObject
{
	GENERATED_BODY()
public:
	UPROPERTY(BlueprintReadOnly, VisibleAnywhere)
	UQuestAsset* pQuestAsset;
};
