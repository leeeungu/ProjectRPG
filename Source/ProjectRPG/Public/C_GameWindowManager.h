#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "C_GameWindowWidget.h"
#include "C_GameWindowManager.generated.h"

class APlayerController;
class UC_MainWidget;

UCLASS( ClassGroup=(UC_GameWindowManager), meta=(BlueprintSpawnableComponent) )
class PROJECTRPG_API UC_GameWindowManager : public UActorComponent
{
	GENERATED_BODY()
protected:
	UPROPERTY(EditDefaultsOnly, Category = "UC_GameWindowManager")
	TSubclassOf<UC_MainWidget> m_cMainWidget;
private:
	UC_MainWidget* m_pMainWidget;
	TArray<UC_GameWindowWidget*> m_arWidget;
	APlayerController* m_pPlayer;
public:	
	UC_GameWindowManager();

	UFUNCTION(BlueprintCallable)
	bool addWidgetToMain(E_WindwoType eType, UC_GameWindowWidget* pWidget);
	UFUNCTION(BlueprintCallable)
	bool removeWidgetFromMain(UC_GameWindowWidget* pWidget);
protected:
	virtual void BeginPlay() override;
		
};
