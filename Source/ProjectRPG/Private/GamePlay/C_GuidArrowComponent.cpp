#include "GamePlay/C_GuidArrowComponent.h"


UC_GuidArrowComponent::UC_GuidArrowComponent(const FObjectInitializer& ObjectInitializer) :
	UStaticMeshComponent(ObjectInitializer)
{
	PrimaryComponentTick.bCanEverTick = true;
	PrimaryComponentTick.bStartWithTickEnabled = true;
	SetComponentTickEnabled(true);
	//Script/Engine.StaticMesh'/Game/Fab/GuidArrow/SM_GuidArrow.SM_GuidArrow'
	//Script/Engine.StaticMesh'/Game/Fab/GuidArrow/SM_GuidArrow.SM_GuidArrow'
	ConstructorHelpers::FObjectFinder<UStaticMesh> Mesh(TEXT("/Game/Fab/GuidArrow/SM_GuidArrow.SM_GuidArrow"));
	if (Mesh.Succeeded())
	{
		SetStaticMesh(Mesh.Object);
	}
	else
	{
		UE_LOG(LogTemp, Warning, TEXT("Failed to load static mesh at path: %s"), TEXT("/Game/Fab/GuidArrow/SM_GuidArrow.SM_GuidArrow"));
	}
	SetCollisionProfileName(TEXT("NoCollision"));
	SetGenerateOverlapEvents(false);
	endGuiding();
}

void UC_GuidArrowComponent::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);
	float fRotate = m_fRotateSpeed * DeltaTime;
	FVector Direction = (-m_vTargetPosition + GetComponentLocation()).GetSafeNormal();
	float Angle = FMath::RadiansToDegrees(FMath::Acos(FVector::DotProduct(GetForwardVector(), Direction)));
	if (fRotate > Angle)
		fRotate = Angle;
	if (FMath::IsNearlyZero(fRotate))
		return;

	float Dir = FVector::DotProduct(GetRightVector(), Direction);
	if (Dir > 0)
		Dir = 1;
	else
		Dir = -1;
	
	AddRelativeRotation(FRotator(0, fRotate * Dir,0));
}

void UC_GuidArrowComponent::startGuiding(FVector target)
{
	m_vTargetPosition = target; 
	SetVisibility(true);
	SetComponentTickEnabled(true);
}

void UC_GuidArrowComponent::endGuiding()
{
	SetVisibility(false);
	SetComponentTickEnabled(false);
}

void UC_GuidArrowComponent::OnRegister()
{
	Super::OnRegister();
}
