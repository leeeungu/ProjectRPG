// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "S_SkillData.h"
#include "C_SkillComponent.generated.h"

struct FSkillData;

UCLASS( ClassGroup=(Custom), meta=(BlueprintSpawnableComponent) )
class PROJECTRPG_API UC_SkillComponent : public UActorComponent
{
	GENERATED_BODY()
private:
	UPROPERTY()
	TMap<FName, FSkillData> SkillMap;
public:	
	// Sets default values for this component's properties
	UC_SkillComponent();

protected:
	// Called when the game starts
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction) override;
	void skill1(FName skillName);

		
};
