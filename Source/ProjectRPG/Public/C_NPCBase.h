#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Character.h"
#include "C_NPCBase.generated.h"

class UC_InteractionComponent;
class UC_NPCServiceMenuWidget;

UCLASS(Blueprintable)
class PROJECTRPG_API AC_NPCBase : public ACharacter
{
	GENERATED_BODY()
protected:
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "AC_NPCBase")
	UC_InteractionComponent* m_pInteractionComponent{};
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "AC_NPCBase")
	TArray<TSubclassOf<UC_NPCServiceMenuWidget>> m_arrServiceWidgetClasses{};
public:
	AC_NPCBase();

protected:
	virtual void BeginPlay() override;
};
