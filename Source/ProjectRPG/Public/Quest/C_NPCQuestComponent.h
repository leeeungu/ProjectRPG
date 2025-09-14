#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "C_NPCQuestComponent.generated.h"

class UQuestAsset;
class UQuestRuntimeNode;

DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FOnQuestChanged, bool, bHasNode);
DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FOnQuestEnd, UQuestRuntimeNode*, pQuestNode);

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

	UQuestRuntimeNode* m_pRunTimeNode{};
public:
	UPROPERTY(BlueprintAssignable, BlueprintCallable, Category = "C_NPCQuestComponent")
	FOnQuestChanged m_onQuestChanged{};
	UPROPERTY(BlueprintAssignable, BlueprintCallable, Category = "C_NPCQuestComponent")
	FOnQuestEnd m_onQuestEnd{};
public:	
	UC_NPCQuestComponent();

	UFUNCTION(BlueprintPure)
	UQuestAsset* getCurrentQuestAsset() const { return m_CurrentQuestAsset; }

	UFUNCTION(BlueprintPure)
	FText getQuestText() const;

	UFUNCTION(BlueprintCallable)
	bool startTalk();


	UFUNCTION(BlueprintCallable)
	void setNextText(UQuestRuntimeNode* pNext);

	UFUNCTION(BlueprintCallable)
	void endTalk();

	UFUNCTION(BlueprintPure)
	bool getResponeNode(int nIndex, FText& NodeText, UQuestRuntimeNode*& pNode) const ;

	UFUNCTION(BlueprintPure)
	bool getResponeSize(int& nIndex) const;

protected:
	virtual void BeginPlay() override;


};
