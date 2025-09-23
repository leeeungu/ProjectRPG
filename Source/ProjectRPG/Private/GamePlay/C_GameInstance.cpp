#include "GamePlay/C_GameInstance.h"
#include "Kismet/GameplayStatics.h"
#include "Kismet/KismetSystemLibrary.h"

void UC_GameInstance::Init()
{
	UGameInstance::Init();
}

void UC_GameInstance::Shutdown()
{
	UGameInstance::Shutdown();
}

void UC_GameInstance::loadLevelStatic(UObject* pWorldContext)
{
	if (!pWorldContext || !pWorldContext->GetWorld())
		return;
	UWorld* World = pWorldContext->GetWorld();
	UC_GameInstance* Instance = Cast<UC_GameInstance>(World->GetGameInstance());
	if (!Instance || Instance->m_pTargetLevel.IsNull())
	{
		UE_LOG(LogTemp, Warning, TEXT("TargetLevel is Nullptr"));
		UKismetSystemLibrary::QuitGame(World, nullptr, EQuitPreference::Type::Quit, false);
		return;
	}

	Instance->m_nCount = 0;
	//FLoadPackageAsyncOptionalParams LoadParams{};
	//LoadParams.CompletionDelegate = MakeUnique<FLoadPackageAsyncDelegate>();
	//LoadParams.CompletionDelegate->BindUObject(Instance, &UC_GameInstance::openLoadedLevel);
	//LoadParams.ProgressDelegate = MakeUnique<FLoadPackageAsyncProgressDelegate>();
	//LoadParams.ProgressDelegate->BindUFunction(Instance, FName(TEXT("progressFunction")));	

	FLoadPackageAsyncDelegate LoadedDelegate{};
	LoadedDelegate.BindUFunction(Instance, FName(TEXT("openLoadedLevel")));
	LoadPackageAsync(FPackageName::ObjectPathToPackageName(Instance->m_pTargetLevel.ToString()), MoveTemp(LoadedDelegate));
}

void UC_GameInstance::openLevelStatic(UObject* pWorldContext, TSoftObjectPtr<UWorld> newLevel)
{
	if (newLevel.IsNull() || !pWorldContext || !pWorldContext->GetWorld())
		return;
	UWorld* World = pWorldContext->GetWorld();
	UC_GameInstance* Instance = Cast<UC_GameInstance>(World->GetGameInstance());
	if (!Instance)
		return;
	Instance->m_pTargetLevel = newLevel;
	UGameplayStatics::UnloadStreamLevelBySoftObjectPtr(World, World->GetCurrentLevel(), FLatentActionInfo{}, false);
	UGameplayStatics::OpenLevel(World, TEXT("Loading"));
}

void UC_GameInstance::openLoadedLevel()
{
	UGameplayStatics::UnloadStreamLevelBySoftObjectPtr(GetWorld(), GetWorld()->GetCurrentLevel(), FLatentActionInfo{}, false);
	UGameplayStatics::OpenLevelBySoftObjectPtr(GetWorld(), m_pTargetLevel);
	m_pTargetLevel = nullptr;
	UE_LOG(LogTemp, Warning, TEXT("openLoadedLevel %d"), m_nCount);
	m_nCount++;
}
//void UC_GameInstance::openLoadedLevel(const FName& PackageName, UPackage* LoadedPackage, EAsyncLoadingResult::Type Result)
//{
//	if (Result == EAsyncLoadingResult::Succeeded)
//	{
//		UGameplayStatics::UnloadStreamLevelBySoftObjectPtr(GetWorld(), GetWorld()->GetCurrentLevel(), FLatentActionInfo{}, false);
//		UGameplayStatics::OpenLevelBySoftObjectPtr(GetWorld(), m_pTargetLevel);
//		m_pTargetLevel = nullptr;
//		UE_LOG(LogTemp, Warning, TEXT("openLoadedLevel %d"), m_nCount);
//		m_nCount++;
//	}
//}

void UC_GameInstance::progressFunction()
{
	UE_LOG(LogTemp, Warning, TEXT("Load %d"), m_nCount);
	m_nCount++;
}
