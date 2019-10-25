#include "MyViewportClient.h"

#include "Runtime/Engine/Classes/Engine/TextureRenderTarget2D.h"
#include "Runtime/Engine/Public/Slate/SceneViewport.h"
#include "Engine/Canvas.h"


void MyViewportClient::Draw(FViewport* Viewport, FCanvas* Canvas)
{
	// Clear entire canvas
	Canvas->Clear(FLinearColor::White);

	// Draw SceenCaptureComponent texture to entire canvas
	// UTextureRenderTarget2D* TextRenderTarget2D = FAutoMapCaptureModule::Get()->GetPreviewImage();
	if (TargetRt != nullptr && TargetRt->Resource != nullptr)
	{
		FCanvasTileItem TileItem(FVector2D(0, 0), TargetRt->Resource,
			FVector2D(TargetRt->SizeX, TargetRt->SizeY),
			FLinearColor::White);
		TileItem.BlendMode = ESimpleElementBlendMode::SE_BLEND_Opaque;
		Canvas->DrawItem(TileItem);
	}
}
