#include "GamePlay/C_InteractionArrow.h"

UC_InteractionArrow::UC_InteractionArrow(const FObjectInitializer& ObjectInitializer) :
	UStaticMeshComponent(ObjectInitializer)
{
	PrimaryComponentTick.bCanEverTick = true;
	SetComponentTickEnabled(true);
	//Script/Engine.StaticMesh'/Game/Fab/Arrow_2/arrow_2.arrow_2'
	ConstructorHelpers::FObjectFinder<UStaticMesh> Mesh(TEXT("/Game/Fab/Arrow_2/arrow_2.arrow_2"));
	if (Mesh.Succeeded())
	{
		SetStaticMesh(Mesh.Object);
	}
}

void UC_InteractionArrow::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	UStaticMeshComponent::TickComponent(DeltaTime, TickType, ThisTickFunction);
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
