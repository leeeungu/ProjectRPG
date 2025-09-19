#pragma once

#include "CoreMinimal.h"
#include "Components/WidgetComponent.h"
#include "C_QuestAlertWidgetComponent.generated.h"

class UQuestAsset;

UCLASS(ClassGroup = (QuestAlert), meta = (BlueprintSpawnableComponent))
class PROJECTRPG_API UC_QuestAlertWidgetComponent : public UWidgetComponent
{
	GENERATED_BODY()
private:
	UQuestAsset* m_pQuest{};
	//UC_DamageWidget* m_pDamageWidget{};
public:
	UC_QuestAlertWidgetComponent(const FObjectInitializer& ObjectInitializer = FObjectInitializer::Get());
	virtual void BeginPlay() override;
	virtual void OnRegister() override;
	virtual void EndPlay(const EEndPlayReason::Type EndPlayReason) override;
private:
	UFUNCTION()
	void changeQuest(UQuestAsset* Quest);

};
