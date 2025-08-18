#include "Widget/WBP_SlotFrameBorder.h"
#include "Components/CanvasPanelSlot.h"

DEFINE_LOG_CATEGORY_STATIC(WBP_SlotFrameBorder, Log, All);

UWBP_SlotFrameBorder::UWBP_SlotFrameBorder()
{
	static ConstructorHelpers::FObjectFinder<UTexture2D> Texture(TEXT("/Game/UI/Texture/T_SlotFrame.T_SlotFrame"));
	if (Texture.Succeeded())
	{
		SetBrushFromTexture(Texture.Object);
	}
	else
		UE_LOG(WBP_SlotFrameBorder, Error, TEXT("NoFrameTextureObject"));

	Background.DrawAs = ESlateBrushDrawType::Box;
	Background.Margin = FMargin(0.15);
	SetBrushColor({0.2,0.2,0.2,1});
}
