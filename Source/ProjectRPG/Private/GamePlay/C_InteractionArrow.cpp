#include "GamePlay/C_InteractionArrow.h"

UC_InteractionArrow::UC_InteractionArrow(const FObjectInitializer& ObjectInitializer) :
	UStaticMeshComponent(ObjectInitializer)
{
	PrimaryComponentTick.bCanEverTick = true;
	PrimaryComponentTick.bStartWithTickEnabled= true;
	SetComponentTickEnabled(true);
	//Script/Engine.StaticMesh'/Game/Fab/InteractionArrow/SM_InteractionArrow.SM_InteractionArrow'
	ConstructorHelpers::FObjectFinder<UStaticMesh> Mesh(TEXT("Game/Fab/InteractionArrow/SM_InteractionArrow.SM_InteractionArrow"));
	if (Mesh.Succeeded())
	{
		SetStaticMesh(Mesh.Object);
	}
	else
	{
		UE_LOG(LogTemp, Warning, TEXT("Failed to load static mesh at path: %s"), TEXT("Game/Fab/InteractionArrow/SM_InteractionArrow.SM_InteractionArrow"));
	}
	SetCollisionProfileName(TEXT("NoCollision"));
	SetGenerateOverlapEvents(false);
}

void UC_InteractionArrow::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);
	float fMove = DeltaTime * m_fMoveSpeed;
	if (m_fPosition + fMove >= m_fHieght)
	{
		fMove = m_fHieght - m_fPosition;
	}
	m_fPosition += fMove;
	fMove *= m_fDir;
	if (FMath::IsNearlyEqual(m_fPosition, m_fHieght))
	{
		m_fPosition = 0;
		m_fDir *= -1.f;
	}
	AddRelativeLocation(FVector(0,0, fMove));

	float fRotate = DeltaTime * m_fRotateSpeed;
	AddRelativeRotation(FRotator(0, fRotate, 0));
}

void UC_InteractionArrow::OnRegister()
{
	Super::OnRegister();
	SetRelativeScale3D(FVector(3,3,3));
}
