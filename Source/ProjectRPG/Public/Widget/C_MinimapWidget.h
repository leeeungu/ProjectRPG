#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "C_MinimapWidget.generated.h"

class UOverlay;
class AC_MiniMapActor;
class ACharacter;
class UImage;

UCLASS()
class PROJECTRPG_API UC_MinimapWidget : public UUserWidget
{
	GENERATED_BODY()
protected:
	UPROPERTY(BlueprintReadOnly, EditAnywhere, meta = (BindWidget), Category = "MinimapWidget")
	UOverlay* m_pOverLay{};

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "UI")
	TSubclassOf<UUserWidget> m_cPlayerIcon{};

	UPROPERTY()
	UUserWidget* m_pPlayerIcon{};
	UPROPERTY()
	ACharacter* m_pPlayer{};

	UPROPERTY()
	AC_MiniMapActor* m_CapCom{};

	UPROPERTY(BlueprintReadOnly, EditAnywhere, meta = (BindWidget), Category = "MinimapWidget")
	UImage* m_pMapImage{};
protected:
	virtual void NativeOnInitialized() override;
	virtual void NativePreConstruct() override;
	virtual void NativeConstruct() override;
	virtual void NativeDestruct() override;
	virtual void NativeTick(const FGeometry& MyGeometry, float InDeltaTime) override;

private:
	FVector2D WorldToMiniMap(FVector WorldPos, FVector2D MiniMapSize);
};
