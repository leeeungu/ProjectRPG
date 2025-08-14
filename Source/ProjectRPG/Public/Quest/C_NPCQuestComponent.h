#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "C_NPCQuestComponent.generated.h"

class UQuestAsset;

UCLASS( ClassGroup=(Custom), meta=(BlueprintSpawnableComponent) )
class PROJECTRPG_API UC_NPCQuestComponent : public UActorComponent
{
	GENERATED_BODY()
protected:
	UPROPERTY(EditAnywhere, Category = "C_NPCQuestComponent")
	UQuestAsset* m_pCurrentQuestAsset{};
	bool m_bQuestAccept{};
public:	
	UC_NPCQuestComponent();

	UFUNCTION(BlueprintPure)
	UQuestAsset* getCurrentQuestAsset() const { return m_pCurrentQuestAsset; }
	UFUNCTION(BlueprintCallable)
	void setCurrentQuestAsset(UQuestAsset* pAsset);
	UFUNCTION(BlueprintCallable)
	void setQuestStart();
	UFUNCTION(BlueprintPure)
	bool getQuestAccept() const { return m_bQuestAccept; }
protected:
	virtual void BeginPlay() override;

};
