#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "C_DamageWidget.generated.h"

class UTextBlock;
class UCanvasPanel;

UCLASS()
class PROJECTRPG_API UC_DamageWidget : public UUserWidget
{
	GENERATED_BODY()
protected:
	UPROPERTY(meta = (BindWidget))
	UCanvasPanel* m_pCanvasPanel;
	UPROPERTY()
	TArray<UTextBlock*> m_arDamageText;
	TArray<float> m_arDamageTime;

public:
	UFUNCTION(BlueprintCallable)
	void drawDamage(float Damage, FColor Color = FColor::Red);

protected:
	virtual void NativeOnInitialized() override;
	virtual void NativePreConstruct() override;
	virtual void NativeConstruct() override;
	virtual void NativeDestruct() override;

private:
	UTextBlock* createDataTextBlock();
	void drawDamage(UTextBlock* TextBlock, float Damage, FColor Color = FColor::Red);
};
