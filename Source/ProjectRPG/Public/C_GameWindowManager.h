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
	APlayerController* m_pPlayer;
public:	
	UC_GameWindowManager();

	UFUNCTION(BlueprintCallable)
	bool toggleWidget(E_WindowType eType);

	UFUNCTION(BlueprintCallable)
	bool removeWidgetFromMain(E_WindowType eType);

	UFUNCTION(BlueprintCallable)
	bool addWidgetToMain(E_WindowType eType);

	UFUNCTION(BlueprintPure)
	bool isWidgetOpened(E_WindowType eType) const;

	UFUNCTION(BlueprintCallable)
	UC_GameWindowWidget* getGameWindowWidget(E_WindowType eType);
protected:
	virtual void BeginPlay() override;
		
};
