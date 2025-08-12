#pragma once

#include "CoreMinimal.h"
#include "Factories/Factory.h"
#include "QuestAssetFactory.generated.h"

UCLASS()
class UQuestAssetFactory : public UFactory
{
	GENERATED_BODY()
public:
	UQuestAssetFactory(const FObjectInitializer& ObjectInitializer);

	virtual UObject* FactoryCreateNew(UClass* InClass, UObject* InParent, FName InName, EObjectFlags Flags, UObject* Context, FFeedbackContext* Warn, FName CallingContext) override;
	virtual bool CanCreateNew() const override;
};
