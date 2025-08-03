#include "C_SearchingCollision.h"

UC_SearchingCollision::UC_SearchingCollision() : UCapsuleComponent{}
{

}

void UC_SearchingCollision::BeginPlay()
{
	UCapsuleComponent::BeginPlay();
	
	if (GetOwner()->Implements<UC_SearchingInterface>())
	{
		m_ownerInterface = TScriptInterface<IC_SearchingInterface>(GetOwner());
	}
	else
	{
		UE_LOG(LogTemp, Log, TEXT("Owner가 IC_SearchingInterface를 상속받지 않았습니다."));
	}
	OnComponentBeginOverlap.AddDynamic(this, &UC_SearchingCollision::beginSearching);
	OnComponentEndOverlap.AddDynamic(this, &UC_SearchingCollision::endSearching);
}

void UC_SearchingCollision::PostEditChangeProperty(FPropertyChangedEvent& PropertyChangedEvent)
{
	UCapsuleComponent::PostEditChangeProperty(PropertyChangedEvent);
}

void UC_SearchingCollision::beginSearching(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
	if (OtherActor == GetOwner())
		return;
	if (m_ownerInterface.GetObject())
		IC_SearchingInterface::Execute_beginSearchingEvent(GetOwner(), OtherActor);

	//IC_DetectedInterface* Interface = Cast<IC_DetectedInterface>(OtherActor);
	if (OtherActor->Implements<UC_DetectedInterface>())
	{
		m_setDetected.Add(TScriptInterface< IC_DetectedInterface>(OtherActor));
		IC_DetectedInterface::Execute_beginDetectedEvent(OtherActor, GetOwner());
	}
}

void UC_SearchingCollision::endSearching(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex)
{
	if (OtherActor == GetOwner())
		return;
	if (m_ownerInterface.GetObject())
		IC_SearchingInterface::Execute_endSearchingEvent(GetOwner(), OtherActor);

	TScriptInterface <IC_DetectedInterface > Interface = TScriptInterface< IC_DetectedInterface>(OtherActor);
	if (Interface.GetInterface())
	{
		m_setDetected.Remove(Interface);
	}
}
