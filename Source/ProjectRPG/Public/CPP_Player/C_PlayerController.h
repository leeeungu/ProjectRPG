// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/PlayerController.h"
#include "InputAction.h"
#include "InputActionValue.h"
#include "C_PlayerController.generated.h"

class UInputMappingContext;
class UInputAction;
class UCameraComponent;
/**
 * 
 */
UCLASS()
class PROJECTRPG_API AC_PlayerController : public APlayerController
{
	GENERATED_BODY()

private:
	

protected:
	virtual void BeginPlay() override;
	virtual void Tick(float DeltaTime) override;
	virtual void SetupInputComponent() override;

	UPROPERTY(EditAnywhere, Category = "EnhancedInput")
	UInputMappingContext* InputMapping;
	UPROPERTY(EditAnywhere, Category = "Input")
	UInputAction* RightClick;
	UPROPERTY(EditAnywhere, Category = "Input")
	UInputAction* SpaceBar;


	void GetMousePos(bool IsOpenMousePoint);
public:
	bool IsOpenMousePointTrigger = false;
	void OnRightClickAction(const FInputActionValue& Value);
	void OnSpaceBarAction(const FInputActionValue& Value);
public:
	AC_PlayerController();
	virtual void OnPossess(APawn* pawn) override;
	
};
