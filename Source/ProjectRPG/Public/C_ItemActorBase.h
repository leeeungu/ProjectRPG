#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "C_ItemActorBase.generated.h"

UCLASS()
class PROJECTRPG_API AC_ItemActorBase : public AActor
{
	GENERATED_BODY()
protected:
	UPROPERTY(BlueprintReadOnly, Category = "AC_ItemActorBase")
	AActor* m_pTargetActor{};
	UPROPERTY(BlueprintReadOnly, Category = "AC_ItemActorBase")
	int m_nItemID{};
public:	
	AC_ItemActorBase();

	UFUNCTION(BlueprintCallable)
	bool useItemActor();

	void setItemID(int nItemID) { m_nItemID = nItemID; }
protected:
	UFUNCTION(BlueprintPure, BlueprintNativeEvent)
	bool findActor(AActor*& pTargetActor);
	virtual bool findActor_Implementation(AActor*& pTargetActor);

	UFUNCTION(BlueprintCallable, BlueprintNativeEvent)
	bool itemEffect();
	virtual bool itemEffect_Implementation();

	UFUNCTION(BlueprintCallable, BlueprintNativeEvent)
	bool useFail();
	virtual bool useFail_Implementation();

	UFUNCTION(BlueprintPure, BlueprintNativeEvent)
	bool checkItemUseable();
	virtual bool checkItemUseable_Implementation();

	UFUNCTION(BlueprintCallable, BlueprintNativeEvent)
	bool useSuccess();
	virtual bool useSuccess_Implementation();

	UFUNCTION(BlueprintCallable)
	void setTargetActor(AActor* pTargetActor) { m_pTargetActor = pTargetActor; }
protected:

};
