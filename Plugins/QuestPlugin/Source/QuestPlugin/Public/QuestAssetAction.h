#pragma once

#include "CoreMinimal.h"
#include "AssetTypeActions_Base.h"

class QuestAssetAction : public FAssetTypeActions_Base
{
public:
	QuestAssetAction(EAssetTypeCategories::Type categpry);

    // 이 에셋 타입의 이름을 반환합니다.
    virtual FText GetName() const override;

    // 콘텐츠 브라우저에서 표시될 색상을 지정합니다.
    virtual FColor GetTypeColor() const override;

    // 이 에셋이 지원하는 클래스를 지정합니다.
    virtual UClass* GetSupportedClass() const override;

    // 에셋을 더블클릭했을 때 호출될 함수입니다. 여기서 에디터를 엽니다.
    virtual void OpenAssetEditor(const TArray<UObject*>& InObjects, TSharedPtr<class IToolkitHost> EditWithinLevelEditor = TSharedPtr<IToolkitHost>()) override;

    virtual  uint32 GetCategories() override;

private:
    EAssetTypeCategories::Type _assetCategpry;
};
