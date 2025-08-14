#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "C_NPCServiceMenuWidget.generated.h"

class UButton;
class UTextBlock;
class UImage;
class UTexture2D;

DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FOnServiceClicked, UC_NPCServiceMenuWidget*, pClickedWidget);

UCLASS(Blueprintable)
class PROJECTRPG_API UC_NPCServiceMenuWidget : public UUserWidget
{
	GENERATED_BODY()
public:
	UPROPERTY(BlueprintAssignable, BlueprintCallable)
	FOnServiceClicked m_onServiceClicked{};
protected:
	UPROPERTY(BlueprintReadOnly, VisibleAnywhere, meta = (BindWidget), Category = "UC_NPCServiceMenuWidget")
	UButton* m_pServiceButton{};
	UPROPERTY(BlueprintReadOnly, VisibleAnywhere, meta = (BindWidget), Category = "UC_NPCServiceMenuWidget")
	
	UTextBlock* m_pServiceText{};
	UPROPERTY(BlueprintReadOnly, VisibleAnywhere, meta = (BindWidget), Category = "UC_NPCServiceMenuWidget")
	UImage* m_pServiceImage{};
	UPROPERTY(BlueprintReadOnly, EditAnywhere, Category = "UC_NPCServiceMenuWidget")
	UTexture2D* m_pServiceTexture{};
	UPROPERTY(BlueprintReadOnly, EditAnywhere, Category = "UC_NPCServiceMenuWidget")
	FText m_streServiceText{};
	UPROPERTY(BlueprintReadOnly, VisibleAnywhere, Category = "UC_NPCServiceMenuWidget")
	bool m_bClicked{};
public:
	UC_NPCServiceMenuWidget(const FObjectInitializer& ObjectInitializer);

	UFUNCTION(BlueprintCallable)
	UC_NPCServiceMenuWidget* unSelectServiceWidget(UC_NPCServiceMenuWidget* pPreWidget);

	virtual void NativeOnInitialized() override;
protected:
	virtual void NativePreConstruct() override;

	UFUNCTION(BlueprintCallable, BlueprintNativeEvent, Category = "UC_NPCServiceMenuWidget")
	void onServiceButton();

	virtual void onServiceButton_Implementation() {}

	UFUNCTION(BlueprintCallable, BlueprintNativeEvent, Category = "UC_NPCServiceMenuWidget")
	void unSelectService();

	virtual void unSelectService_Implementation() {}
private:
	UFUNCTION()
	void buttonClick();
};
