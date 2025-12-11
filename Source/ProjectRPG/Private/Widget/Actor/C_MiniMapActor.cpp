#include "Widget/Actor/C_MiniMapActor.h"
#include "Kismet/GameplayStatics.h"
#include "Components/Overlay.h"
#include "Components/OverlaySlot.h"
#include "Components/CanvasPanelSlot.h"
#include "Components/SceneCaptureComponent2D.h"
#include "GameFramework/Character.h"
#include "CPP_Player/C_PlayerController.h"
#include "Blueprint/UserWidget.h"
#include "Blueprint/WidgetTree.h"
#include "C_GameWindowManager.h"

AC_MiniMapActor::AC_MiniMapActor()
{
	PrimaryActorTick.bCanEverTick = false;
	m_CapCom = CreateDefaultSubobject<USceneCaptureComponent2D>(TEXT("SceneCaptureComponent2D"));
	m_CapCom->SetupAttachment(GetRootComponent());

	m_CapCom->ProjectionType = ECameraProjectionMode::Orthographic;
	m_CapCom->OrthoWidth = 9500.f;
	m_CapCom->PrimitiveRenderMode = ESceneCapturePrimitiveRenderMode::PRM_UseShowOnlyList;
}


float AC_MiniMapActor::getOrthoWidth()
{
	return m_CapCom->OrthoWidth;
}

void AC_MiniMapActor::BeginPlay()
{
	Super::BeginPlay();
	ACharacter* pPlayer = UGameplayStatics::GetPlayerCharacter(GetWorld(), 0);

	if (pPlayer)
	{
		m_CapCom->ShowOnlyActors.Add(pPlayer);
	}
}

