#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include <Components/CanvasPanel.h>
#include "C_GameWindowWidget.h"
#include "C_MainWidget.generated.h"

UCLASS()
class PROJECTRPG_API UC_MainWidget : public UUserWidget
{
	GENERATED_BODY()

public:
	UPROPERTY(BlueprintReadWrite, meta = (BindWidget), Category = "UC_MainWidget")
	UCanvasPanel* m_pMainCanvas;

private:
	TArray<UC_GameWindowWidget*> m_arrGameWidget;

public:
	UC_MainWidget(const FObjectInitializer& ObjectInitializer);

	UFUNCTION(BlueprintCallable)
	bool addWidgetToMain(E_WindwoType eType);
	UFUNCTION(BlueprintCallable)
	bool removeWidgetFromMain(E_WindwoType eType);

	UFUNCTION(BlueprintPure)
	bool isWidgetOpened(E_WindwoType eType) const;

	void registerWidget(UC_GameWindowWidget* pWidget);
protected:
	virtual void NativeOnInitialized() override;
};
