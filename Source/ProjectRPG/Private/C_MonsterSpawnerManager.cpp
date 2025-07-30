// Fill out your copyright notice in the Description page of Project Settings.


#include "C_MonsterSpawnerManager.h"

// Sets default values
AC_MonsterSpawnerManager::AC_MonsterSpawnerManager()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

}

// Called when the game starts or when spawned
void AC_MonsterSpawnerManager::BeginPlay()
{
	Super::BeginPlay();

	TArray<AActor*> arrFoundSpawner;

	UGameplayStatics::GetAllActorsOfClass(GetWorld(), AC_MonsterSpawner::StaticClass(), arrFoundSpawner);

	for (AActor* pActor : arrFoundSpawner)
	{
		AC_MonsterSpawner* pSpawner = Cast<AC_MonsterSpawner>(pActor);
		if (pSpawner)
		{
			m_arrSpawner.Add(pSpawner);
		}
	}

	m_nCurrentIndex = 0;
	activateCurrentSpawner();
	
}

void AC_MonsterSpawnerManager::onCurrentSpawnerComplete()
{
	m_nCurrentIndex++;

	if (m_arrSpawner.IsValidIndex(m_nCurrentIndex))
	{
		activateCurrentSpawner();
	}
	else
	{
		UE_LOG(LogTemp, Log, TEXT("All Spawner Complete!"));
	}
}

void AC_MonsterSpawnerManager::activateCurrentSpawner()
{
	if (m_arrSpawner.IsValidIndex(m_nCurrentIndex))
	{
		AC_MonsterSpawner* pSpawner = m_arrSpawner[m_nCurrentIndex];
		if (pSpawner)
		{
			pSpawner->m_onSpawnComplete.AddDynamic(this, &AC_MonsterSpawnerManager::onCurrentSpawnerComplete);


			pSpawner->setSpawnerActive(true);
		}
	}
}

// Called every frame
void AC_MonsterSpawnerManager::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

