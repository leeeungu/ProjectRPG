#include "Widget/C_MinimapWidget.h"
#include "Components/Overlay.h"
#include "Components/OverlaySlot.h"
#include "Components/Image.h"
#include "GameFramework/Character.h"
#include "Blueprint/UserWidget.h"
#include "Kismet/GameplayStatics.h"
#include "Widget/Actor/C_MiniMapActor.h"

void UC_MinimapWidget::NativeOnInitialized()
{
	UUserWidget::NativeOnInitialized();

	if (m_cPlayerIcon)
	{
		m_pPlayerIcon = CreateWidget<UUserWidget>(GetOwningPlayer(), m_cPlayerIcon);
	}
	if (m_pOverLay)
	{
		UOverlaySlot* OverSlot = m_pOverLay->AddChildToOverlay(m_pPlayerIcon);
		if (OverSlot)
		{
			OverSlot->SetHorizontalAlignment(HAlign_Left);
			OverSlot->SetVerticalAlignment(VAlign_Top);
		}
	}
}
void UC_MinimapWidget::NativePreConstruct()
{
	UUserWidget::NativePreConstruct();
}
void UC_MinimapWidget::NativeConstruct()
{
	UUserWidget::NativeConstruct();
	m_pPlayer = Cast<ACharacter>(GetOwningPlayer()->GetPawn());
	m_CapCom = Cast< AC_MiniMapActor>(UGameplayStatics::GetActorOfClass(GetWorld(), AC_MiniMapActor::StaticClass()));
	if (m_pMapImage && m_CapCom)
	{
		m_pMapImage->SetBrushFromMaterial(m_CapCom->getMapMaterial());
	}

}
void UC_MinimapWidget::NativeDestruct()
{
	UUserWidget::NativeDestruct();
}

void UC_MinimapWidget::NativeTick(const FGeometry& MyGeometry, float InDeltaTime)
{
	UUserWidget::NativeTick(MyGeometry, InDeltaTime);
	FVector2D minimapSize = { 300.0f,300.0f };

	if (m_pPlayer && m_pPlayerIcon && m_CapCom)
	{
		FVector2D Pos = WorldToMiniMap(m_pPlayer->GetActorLocation(), minimapSize);
		m_pPlayerIcon->SetRenderTranslation(Pos);
	}
}

FVector2D UC_MinimapWidget::WorldToMiniMap(FVector WorldPos, FVector2D MiniMapSize)
{
	float OrthoWidth = m_CapCom->getOrthoWidth();
	FVector CapPos = m_CapCom->GetActorLocation();
	float DeltaX = WorldPos.X - CapPos.X;
	float DeltaY = WorldPos.Y - CapPos.Y;
	// uv와 xy 좌표가 다르니깐 조심하자
	float NormalU = DeltaY / OrthoWidth;  // + 0.5f;
	float NormalV = DeltaX / OrthoWidth;  // + 0.5f;

	float U = (NormalU + 0.5f) * MiniMapSize.Y;
	float V = (1.0f - (NormalV + 0.5f)) * MiniMapSize.X;

	return FVector2D(U - 8, V - 8); // 그림 크기 8 pixel
}