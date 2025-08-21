#pragma once

#include "CoreMinimal.h"
#include "Animation/AnimNotifies/AnimNotify.h"
#include "C_SphereAttackNotify.generated.h"

UCLASS()
class PROJECTRPG_API UC_SphereAttackNotify : public UAnimNotify
{
	GENERATED_BODY()
protected:
	UPROPERTY(BlueprintReadOnly, EditAnywhere, Category = "UC_SphereAttackNotify")
	FVector m_sPosition{};
	UPROPERTY(BlueprintReadOnly, EditAnywhere, Category = "UC_SphereAttackNotify")
	FName m_strSocketName = NAME_None;
	UPROPERTY(BlueprintReadOnly, EditAnywhere, Category = "UC_SphereAttackNotify")
	float m_fRadius = 100.0f;
	UPROPERTY(BlueprintReadOnly, EditAnywhere, Category = "UC_SphereAttackNotify")
	float m_fDamage = 1.0f;
	UPROPERTY(BlueprintReadOnly, EditAnywhere, Category = "UC_SphereAttackNotify")
	bool m_bDrawDebug = false;

private:
	virtual void OnAnimNotifyCreatedInEditor(FAnimNotifyEvent& ContainingAnimNotifyEvent) override;
	virtual void Notify(USkeletalMeshComponent* MeshComp, UAnimSequenceBase* Animation, const FAnimNotifyEventReference& EventReference) override;
};
