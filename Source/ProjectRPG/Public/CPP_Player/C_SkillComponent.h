// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "S_SkillData.h"
#include "C_SkillComponent.generated.h"

DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FOnSkillMontageRequested, class UAnimMontage*, MontageToPlay);

struct FSkillData;


UCLASS( ClassGroup=(Custom), meta=(BlueprintSpawnableComponent) )
class PROJECTRPG_API UC_SkillComponent : public UActorComponent
{
	GENERATED_BODY()
private:
	UPROPERTY()
	TMap<FName, FSkillData> SkillMap;
	TMap<FName, float> SkillCooldownEndTime;
public:	
	// Sets default values for this component's properties
	UC_SkillComponent();
	UPROPERTY(BlueprintAssignable, Category = "Skill")
	FOnSkillMontageRequested OnSkillMontageRequested;
	//스킬컴포넌트에서 애님인스턴스 참조용 포인터
	UPROPERTY()
    class UC_PlayerAnimInstance* CachedAnimInstance;
	UFUNCTION()
	void RequestJumpToSection(FName SectionName);

protected:
	// Called when the game starts
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction) override;
	virtual void InitializeComponent() override;
	void UsingSkill(FName skill_Key);

	bool IsCooldownReady(FName SkillName) const;
	void StartCooldown(FName SkillName);
	float GetRemainingCooldown(FName SkillName) const;

		
};
