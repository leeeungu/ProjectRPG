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
	UPROPERTY(meta = (BindWidget), BlueprintReadOnly)
	UCanvasPanel* m_pCanvasPanel{};
	UPROPERTY()
	TArray<UTextBlock*> m_arDamageText{};
	TArray<float> m_arDamageTime{};
	UPROPERTY()
	float m_fUpSpeed = 90;
	UPROPERTY()
	float m_fUpDistance = 70;
public:
	UFUNCTION(BlueprintCallable)
	void drawDamage(float Damage, FColor Color = FColor::Red);

protected:
	virtual void NativeOnInitialized() override;
	virtual void NativeTick(const FGeometry& MyGeometry, float InDeltaTime) override;
private:
	UTextBlock* createDataTextBlock();
	void drawDamage(UTextBlock* TextBlock, float Damage, FColor Color = FColor::Red);
	void InitSetting(UTextBlock* TextBlock);
	void endText(UTextBlock* TextBlock);
};
