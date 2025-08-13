// Fill out your copyright notice in the Description page of Project Settings.


#include "C_CounterNotify.h"
#include "C_CounterComponent.h"

void UC_CounterNotify::Notify(USkeletalMeshComponent* skMeshCom, UAnimSequenceBase* Animation)
{
	Super::Notify(skMeshCom, Animation);

	APawn* pMonster = Cast<APawn>(skMeshCom->GetOwner());

	if (!pMonster)
		return;

	if (UC_CounterComponent* pCounterCom = pMonster->FindComponentByClass<UC_CounterComponent>())
	{
		pCounterCom->startCounterWindow(m_fCanCounterDuration);
	}
}
