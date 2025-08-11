#include "C_AnimationInteraction.h"
#include <GameFramework/Character.h>
#include <Components/ArrowComponent.h>
#include <Components/WidgetComponent.h>
#include <C_InteractionComponent.h>
#include <Kismet/KismetMathLibrary.h>
#include <NavigationSystem.h>
#include <NavigationPath.h>

AC_AnimationInteraction::AC_AnimationInteraction() :
	AActor{}
{

	PrimaryActorTick.bCanEverTick = true;
	m_pRoot = CreateDefaultSubobject<USceneComponent>("Root");
	SetRootComponent(m_pRoot);

	m_pStartCollision = CreateDefaultSubobject< UC_InteractionComponent>("StartCollision");
	m_pStartCollision->AttachToComponent(m_pRoot, FAttachmentTransformRules::KeepRelativeTransform);

	m_pStartDirection = CreateDefaultSubobject< UArrowComponent>("Direction");
	m_pStartDirection->AttachToComponent(m_pRoot, FAttachmentTransformRules::KeepRelativeTransform);

	m_pEndCollision2 = CreateDefaultSubobject< UCapsuleComponent>("EndCollision");
	m_pEndCollision2->SetCollisionProfileName(*UC_InteractionComponent::getInteractionPresetName());
	m_pEndCollision2->SetCapsuleSize(200.0f, 200.0f);
	m_pEndCollision2->AttachToComponent(m_pRoot, FAttachmentTransformRules::KeepRelativeTransform);

	m_pInteractionWidget = CreateDefaultSubobject< UWidgetComponent>("InteractionWidget");
	m_pInteractionWidget->AttachToComponent(m_pRoot, FAttachmentTransformRules::KeepRelativeTransform);
	m_pInteractionWidget->SetWidgetSpace(EWidgetSpace::Screen);
	m_pInteractionWidget->SetDrawSize(FVector2D{64,64});
	m_pInteractionWidget->SetRelativeRotation({ 0,180,0 });
	m_pInteractionWidget->SetCollisionProfileName("NoCollision");

	//Script/Engine.Texture2D'/Game/UI/Interaction/Texture/T_InteractionKey.T_InteractionKey'
	///Script/UMGEditor.WidgetBlueprint'/Game/UI/Interaction/WBP_InteractionUI.WBP_InteractionUI'
	static ConstructorHelpers::FClassFinder<UUserWidget> Texture(TEXT("/Game/UI/Interaction/WBP_InteractionUI.WBP_InteractionUI_C"));
	if (Texture.Succeeded())
	{
		m_pInteractionWidget->SetWidgetClass(Texture.Class);
	}
}

void AC_AnimationInteraction::PostEditChangeProperty(FPropertyChangedEvent& PropertyChangedEvent)
{
	AActor::PostEditChangeProperty(PropertyChangedEvent);
	if (m_pStartDirection && m_pEndCollision2)
	{
		FRotator Rot = UKismetMathLibrary::FindLookAtRotation(m_pStartDirection->GetComponentLocation(), m_pEndCollision2->GetComponentLocation());
		if (!m_bRotateYaw)
			Rot.Yaw = 0.0;
		if (!m_bRotatePitch)
			Rot.Pitch = 0.0;
		if (!m_bRotateRoll)
			Rot.Roll = 0.0;
		m_pStartDirection->SetWorldRotation(Rot);
	}
}

void AC_AnimationInteraction::PostEditMove(bool bFinished)
{
	AActor::PostEditMove(bFinished);
	if (m_pStartDirection && m_pEndCollision2)
	{
		FRotator Rot = UKismetMathLibrary::FindLookAtRotation(m_pStartDirection->GetComponentLocation(), m_pEndCollision2->GetComponentLocation());
		if (!m_bRotateYaw)
			Rot.Yaw = 0.0;
		if (!m_bRotatePitch)
			Rot.Pitch = 0.0;
		if (!m_bRotateRoll)
			Rot.Roll = 0.0;
		m_pStartDirection->SetWorldRotation(Rot);
	}
}

void AC_AnimationInteraction::Tick(float DeltaTime)
{
	AActor::Tick(DeltaTime);

	if (!m_arrLocations.IsValidIndex(nIndex) || !m_pDetector)
		return;

	FVector Dir = m_arrLocations[nIndex] - m_pDetector->GetActorLocation();
	Dir.Z = 0.0f;
	Dir.Normalize();
	m_pDetector->AddMovementInput(Dir);
	Dir = m_arrLocations[nIndex] - m_pDetector->GetActorLocation();
	Dir.Z = 0.0f;
	if (Dir.Length() < 15.0f)
	{
		nIndex++;
		if (!m_arrLocations.IsValidIndex(nIndex))
		{
			StartAnimation();
		}
	}
}

void AC_AnimationInteraction::BeginPlay()
{
	SetActorTickEnabled(false);
	m_pInteractionWidget->SetVisibility(false);

	AActor::BeginPlay();
	m_pStartCollision->m_onInteraction.AddDynamic(this, &AC_AnimationInteraction::interactionStart);
	m_pStartCollision->OnComponentBeginOverlap.AddDynamic(this, &AC_AnimationInteraction::beginOverlap);
	m_pStartCollision->OnComponentEndOverlap.AddDynamic(this, &AC_AnimationInteraction::endOverlap);
	if (m_pEndCollision2)
		m_pEndCollision2->OnComponentBeginOverlap.AddDynamic(this, &AC_AnimationInteraction::beginEndCollision);
}

void AC_AnimationInteraction::interactionStart(AActor* pDetectedActor)
{
	if (!pDetectedActor || m_eStartType == E_TrabelType::E_NONE || m_bPlay)
		return;
	m_pDetector = Cast<ACharacter>(pDetectedActor);
	m_pTravelManagerComponent = pDetectedActor->GetComponentByClass<UC_TravelManagerComponent>();
	if (!m_pDetector || !m_pTravelManagerComponent || m_pTravelManagerComponent->getTravelType() != E_TrabelType::E_NONE)
		return;

	UNavigationPath* pPath = UNavigationSystemV1::FindPathToLocationSynchronously(GetWorld(), m_pDetector->GetActorLocation(), m_pStartDirection->GetComponentLocation());
	if (pPath && pPath->IsValid() && pPath->PathPoints.Num() > 1)
	{
		SetActorTickEnabled(true);
		m_arrLocations = pPath->PathPoints;
		nIndex = 0;
	}
}

void AC_AnimationInteraction::StartAnimation()
{
	if (!m_pDetector)
		return;
	SetActorTickEnabled(false); 
	m_pDetector->SetActorTickEnabled(false);
	m_pDetector->SetActorRotation(m_pStartDirection->GetComponentRotation());
	m_pDetector->SetActorLocation(m_pStartDirection->GetComponentLocation());
	m_pTravelManagerComponent->setTravelType(m_eStartType);
	m_bPlay = true;
}

void AC_AnimationInteraction::beginEndCollision(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
	if (!m_pDetector || m_pDetector != Cast<ACharacter>(OtherActor) || m_eEndType == E_TrabelType::E_NONE || !m_pTravelManagerComponent || !m_bPlay)
		return;
	m_pTravelManagerComponent->setTravelType(m_eEndType);
	m_pDetector = nullptr;
	m_pTravelManagerComponent = nullptr;
	m_bPlay = false;
}

void AC_AnimationInteraction::beginOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
	m_pInteractionWidget->SetVisibility(true);
}

void AC_AnimationInteraction::endOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex)
{
	m_pInteractionWidget->SetVisibility(false);
}
