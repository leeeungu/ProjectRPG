// Fill out your copyright notice in the Description page of Project Settings.


#include "Monster/C_GimmickComponent.h"




bool UC_GimmickComponent::canGimmickStart()
{
	if (!m_bGimmickPlaying)
		return true;


	return false;
}

// Sets default values for this component's properties
UC_GimmickComponent::UC_GimmickComponent()
{
	// Set this component to be initialized when the game starts, and to be ticked every frame.  You can turn these features
	// off to improve performance if you don't need them.
	PrimaryComponentTick.bCanEverTick = true;

	m_pMonster = Cast<AC_MonsterBaseCharacter>(GetOwner());

	// ...
}


// Called when the game starts
void UC_GimmickComponent::BeginPlay()
{
	Super::BeginPlay();

	// ...
	
	m_pBBcom = Cast<UBlackboardComponent>(m_pMonster->GetController());
}

void UC_GimmickComponent::excuteGimmick()
{
	m_bGimmickPlaying = true;
	updateGimmickBool();

}

bool UC_GimmickComponent::endGimmick()
{
	m_bGimmickPlaying = false;

	updateGimmickBool();


	SetComponentTickEnabled(false);

	return false;
}

void UC_GimmickComponent::updateGimmickBool()
{
	m_pBBcom->SetValueAsBool(AC_MonsterAiController::IsGimmickPlayingKey, m_bGimmickPlaying);
}


// Called every frame
void UC_GimmickComponent::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);

	if (m_bGimmickPlaying)
		m_fGimmickTime -= DeltaTime;

	if (m_fGimmickTime <= 0)
		endGimmick();

	// ...
}

