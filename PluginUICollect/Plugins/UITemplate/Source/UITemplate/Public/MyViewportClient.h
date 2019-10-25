#pragma once
#include "DistCurveEditor/Private/CurveEditorViewportClient.h"
#include "Engine/TextureRenderTarget2D.h"

class MyViewportClient : public FViewportClient
{
public:
	MyViewportClient(UTextureRenderTarget2D* RT)
	{
		TargetRt = RT;
	}
	virtual void Draw(FViewport* Viewport, FCanvas* Canvas) override;

private:
	UTextureRenderTarget2D* TargetRt;
};

