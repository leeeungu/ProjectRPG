#pragma once

#include "CoreMinimal.h"
#include "Components/WidgetComponent.h"
#include "C_DamageWidgetComponent.generated.h"

class UC_DamageWidget;

UCLASS(ClassGroup = (Damage), meta = (BlueprintSpawnableComponent))
class PROJECTRPG_API UC_DamageWidgetComponent : public UWidgetComponent
{
	GENERATED_BODY()
private:
	UC_DamageWidget* m_pDamageWidget{};
public:
	UC_DamageWidgetComponent(const FObjectInitializer& ObjectInitializer = FObjectInitializer::Get());
	virtual void BeginPlay() override;
	virtual void OnRegister() override;
private:
	UFUNCTION()
	void changeHp(float fPre, float fCurrent);

};
