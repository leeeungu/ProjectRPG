#pragma once

#include "CoreMinimal.h"
#include "Animation/AnimNotifies/AnimNotify.h"
#include "C_MonsterAttackNotify.generated.h"

UCLASS()
class PROJECTRPG_API UC_MonsterAttackNotify : public UAnimNotify
{
	GENERATED_BODY()
protected:
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	FVector m_sRelativePosition{};
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "MonsterAttackNotify", meta = (AnimNotifyBoneName = "true"))
	FName m_SocketName{};
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	float m_Radius = 10.0f;
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	float m_Damage = 1.0f;
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	bool m_bDrawDebug{};
private:
	virtual void Notify(USkeletalMeshComponent* MeshComp, UAnimSequenceBase* Animation, const FAnimNotifyEventReference& EventReference) override;
};
