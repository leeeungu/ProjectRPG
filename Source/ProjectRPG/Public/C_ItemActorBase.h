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
	AActor* m_pInstigator{};
	UPROPERTY(BlueprintReadOnly, EditAnywhere, Category = "AC_ItemActorBase")
	int m_nItemID{};

public:	
	AC_ItemActorBase();

	UFUNCTION(BlueprintCallable)
	bool useItemActor();

protected:
	UFUNCTION(BlueprintCallable, BlueprintNativeEvent)
	bool findActor(AActor*& pTargetActor);
	virtual bool findActor_Implementation(AActor*& pTargetActor);

	UFUNCTION(BlueprintCallable, BlueprintNativeEvent)
	bool itemEffect();
	virtual bool itemEffect_Implementation();

	UFUNCTION(BlueprintCallable, BlueprintNativeEvent)
	bool useFail();
	virtual bool useFail_Implementation();

protected:
	virtual void BeginPlay() override;

	//bool FindActor_Internal(AActor*& m_pTargetActor);
	//bool UseItem_Internal();
public:	

};
