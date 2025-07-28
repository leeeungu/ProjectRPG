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
	UPROPERTY(BlueprintReadWrite, meta = (BindWidget), Category = "UC_MainWidget")
	UCanvasPanel* m_pInventoryCanvas;
	UPROPERTY(BlueprintReadWrite, meta = (BindWidget), Category = "UC_MainWidget")
	UCanvasPanel* m_pAlertCanvas;
	UPROPERTY(BlueprintReadWrite, meta = (BindWidget), Category = "UC_MainWidget")
	UCanvasPanel* m_pQuickSlot;

private:
	TArray<UCanvasPanel*> m_arCanvasPanel;

public:
	UC_MainWidget(const FObjectInitializer& ObjectInitializer);

	UFUNCTION(BlueprintCallable)
	bool addWidgetToMain(E_WindwoType eType, UC_GameWindowWidget* pWidget);
	UFUNCTION(BlueprintCallable)
	bool removeWidgetFromMain(UC_GameWindowWidget* pWidget);
protected:
	virtual void NativeOnInitialized() override;
};
