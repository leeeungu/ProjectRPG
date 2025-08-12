#pragma once

#include "CoreMinimal.h"
#include "Factories/Factory.h"
#include "QuestFactory.generated.h"

UCLASS()
class UQuestFactory : public UFactory
{
	GENERATED_BODY()
public:
	UQuestFactory(const FObjectInitializer& ObjectInitializer);

	virtual UObject* FactoryCreateNew(UClass* InClass, UObject* InParent, FName InName, EObjectFlags Flags, UObject* Context, FFeedbackContext* Warn, FName CallingContext) override;
	virtual bool CanCreateNew() const override;
};
