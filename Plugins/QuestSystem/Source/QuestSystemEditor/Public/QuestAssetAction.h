#pragma once

#include "CoreMinimal.h"
#include "AssetTypeActions_Base.h"

class QUESTSYSTEMEDITOR_API QuestAssetAction : public FAssetTypeActions_Base
{
public:
    QuestAssetAction(EAssetTypeCategories::Type categpry);

    virtual FText GetName() const override;
    virtual FColor GetTypeColor() const override;
    virtual UClass* GetSupportedClass() const override;
    virtual void OpenAssetEditor(const TArray<UObject*>& InObjects, TSharedPtr<class IToolkitHost> EditWithinLevelEditor = TSharedPtr<IToolkitHost>()) override;
    virtual  uint32 GetCategories() override;

private:
    EAssetTypeCategories::Type _assetCategpry;
};
