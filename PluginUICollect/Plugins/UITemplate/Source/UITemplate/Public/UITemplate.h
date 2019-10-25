// Copyright 1998-2019 Epic Games, Inc. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "Modules/ModuleManager.h"
#include "SListView.h"
#include "ListViewItem.h"
#include "SViewport.h"
#include "SceneViewport.h"
#include "MyViewportClient.h"
#include "PropertyHandle.h"
class FToolBarBuilder;
class FMenuBuilder;

class FUITemplateModule : public IModuleInterface
{
public:

	/** IModuleInterface implementation */
	virtual void StartupModule() override;
	virtual void ShutdownModule() override;
	
	/** This function will be bound to Command (by default it will bring up plugin window) */
	void PluginButtonClicked();
	
private:

	void AddToolbarExtension(FToolBarBuilder& Builder);
	void AddMenuExtension(FMenuBuilder& Builder);

	TSharedRef<class SDockTab> OnSpawnPluginTab(const class FSpawnTabArgs& SpawnTabArgs);

private:
	TSharedPtr<class FUICommandList> PluginCommands;
	TSharedPtr<SListView<TSharedPtr<ListViewItem>>> ListView;
	TArray<TSharedPtr<ListViewItem>> ListViewItemL;
	TSharedPtr<SViewport> ViewportWidget;
	TSharedPtr<MyViewportClient> ViewportClient;
	TSharedPtr<FSceneViewport> SceneViewport;
	UTextureRenderTarget2D* PreviewTex;
	TSharedPtr<IPropertyHandle> PHandle;
	FString PropertyObjPath;
};
