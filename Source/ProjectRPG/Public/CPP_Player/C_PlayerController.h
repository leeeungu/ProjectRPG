// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/PlayerController.h"
#include "InputAction.h"
#include "InputActionValue.h"
#include "C_PlayerController.generated.h"

class UInputMappingContext;
class UInputAction;
class UC_InputQueueComponent;
/**
 * 
 */

UENUM(BlueprintType)
enum class EMouseHitType : uint8
{
	None,
	Object,
	Ground
};

UCLASS()
class PROJECTRPG_API AC_PlayerController : public APlayerController
{
	GENERATED_BODY()

private:
	FHitResult CachedMouseHit;
	EMouseHitType CachedHitType = EMouseHitType::None;
	void UpdateMouseHit();

protected:
	virtual void BeginPlay() override;
	virtual void Tick(float DeltaTime) override;
	virtual void SetupInputComponent() override;

	UPROPERTY(EditAnywhere, Category = "EnhancedInput")
	UInputMappingContext* InputMapping;
	UPROPERTY(VisibleAnywhere)
	UC_InputQueueComponent* InputQueueSystem;
	UPROPERTY(EditAnywhere, Category = "Input")
	UInputAction* RightClick;
	UPROPERTY(EditAnywhere, Category = "Input")
	UInputAction* SpaceBar;
	UPROPERTY(EditAnywhere, Category = "Input")
	UInputAction* Q_Skill;


public:
	void OnRightClickAction(const FInputActionValue& Value);
	void OnSpaceBarAction(const FInputActionValue& Value);
	void OnQ_Action(const FInputActionValue& Value);
public:
	AC_PlayerController();
	virtual void OnPossess(APawn* pawn) override;
	// 캐시된 마우스 Hit 결과를 반환
	UFUNCTION(BlueprintCallable, Category = "Mouse")
	bool GetCachedMouseHit(FHitResult& OutHit, EMouseHitType& OutType) const;
	
};
