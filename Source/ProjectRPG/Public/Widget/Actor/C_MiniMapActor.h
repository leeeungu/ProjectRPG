#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "C_MiniMapActor.generated.h"

class USceneCaptureComponent2D;
class UMaterial;

UCLASS()
class PROJECTRPG_API AC_MiniMapActor : public AActor
{
	GENERATED_BODY()
protected:
	UPROPERTY(EditAnywhere, BlueprintReadOnly)
	USceneCaptureComponent2D* m_CapCom{};

	UPROPERTY(EditAnywhere, BlueprintReadOnly)
	UMaterial* m_pMapMaterial{};
public:
	AC_MiniMapActor();
	float getOrthoWidth();
	UMaterial* getMapMaterial() { return m_pMapMaterial; }
protected:
	virtual void BeginPlay() override;
};