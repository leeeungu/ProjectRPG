#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Character.h"
#include "C_NPCBase.generated.h"

class UC_InteractionComponent;

UCLASS(Blueprintable)
class PROJECTRPG_API AC_NPCBase : public ACharacter
{
	GENERATED_BODY()
protected:
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "AC_NPCBase")
	UC_InteractionComponent* m_pInteractionComponent{};
public:
	AC_NPCBase();

protected:
	virtual void BeginPlay() override;
};
