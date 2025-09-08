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
	TArray< UQuestAsset*> m_arrQuestAsset{};
	UPROPERTY()
	UQuestAsset* m_CurrentQuestAsset{};
	int m_nQuestAssetIndex{};
	bool m_bQuestAccept{};
public:	
	UC_NPCQuestComponent();

	UFUNCTION(BlueprintPure)
	UQuestAsset* getCurrentQuestAsset() const { return m_CurrentQuestAsset; }
	UFUNCTION(BlueprintCallable)
	void setCurrentQuestAsset(UQuestAsset* pAsset);
	UFUNCTION(BlueprintCallable)
	void setQuestStart();
	UFUNCTION(BlueprintPure)
	bool getQuestAccept() const { return m_bQuestAccept; }
protected:
	virtual void BeginPlay() override;

	UFUNCTION()
	void onQuestSucceed(UQuestAsset* pAsset);
	UFUNCTION()
	void onQuestFail(UQuestAsset* pAsset);
};
