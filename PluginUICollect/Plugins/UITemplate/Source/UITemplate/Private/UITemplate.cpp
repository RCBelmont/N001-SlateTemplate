// Copyright 1998-2019 Epic Games, Inc. All Rights Reserved.

#include "UITemplate.h"
#include "UITemplateStyle.h"
#include "UITemplateCommands.h"
#include "LevelEditor.h"
#include "Widgets/Docking/SDockTab.h"
#include "Widgets/Layout/SBox.h"
#include "Widgets/Text/STextBlock.h"
#include "Framework/MultiBox/MultiBoxBuilder.h"
#include "SButton.h"
#include "SImage.h"
#include "IPluginManager.h"
#include "SCheckBox.h"
#include "SlateApplication.h"
#include "PropertyCustomizationHelpers.h"
#include "Materials/Material.h"
#include "PropertyEditor/Private/UserInterface/PropertyDetails/PropertyDetailsUtilities.h"

static const FName UITemplateTabName("UITemplate");

#define LOCTEXT_NAMESPACE "FUITemplateModule"

void FUITemplateModule::StartupModule()
{
	// This code will execute after your module is loaded into memory; the exact timing is specified in the .uplugin file per-module

	FUITemplateStyle::Initialize();
	FUITemplateStyle::ReloadTextures();

	FUITemplateCommands::Register();

	PluginCommands = MakeShareable(new FUICommandList);

	PluginCommands->MapAction(
		FUITemplateCommands::Get().OpenPluginWindow,
		FExecuteAction::CreateRaw(this, &FUITemplateModule::PluginButtonClicked),
		FCanExecuteAction());

	FLevelEditorModule& LevelEditorModule = FModuleManager::LoadModuleChecked<FLevelEditorModule>("LevelEditor");

	{
		TSharedPtr<FExtender> MenuExtender = MakeShareable(new FExtender());
		MenuExtender->AddMenuExtension("WindowLayout", EExtensionHook::After, PluginCommands,
		                               FMenuExtensionDelegate::CreateRaw(this, &FUITemplateModule::AddMenuExtension));

		LevelEditorModule.GetMenuExtensibilityManager()->AddExtender(MenuExtender);
	}

	{
		TSharedPtr<FExtender> ToolbarExtender = MakeShareable(new FExtender);
		ToolbarExtender->AddToolBarExtension("Settings", EExtensionHook::After, PluginCommands,
		                                     FToolBarExtensionDelegate::CreateRaw(
			                                     this, &FUITemplateModule::AddToolbarExtension));

		LevelEditorModule.GetToolBarExtensibilityManager()->AddExtender(ToolbarExtender);
	}

	FGlobalTabmanager::Get()->RegisterNomadTabSpawner(UITemplateTabName,
	                                                  FOnSpawnTab::
	                                                  CreateRaw(this, &FUITemplateModule::OnSpawnPluginTab))
	                        .SetDisplayName(LOCTEXT("FUITemplateTabTitle", "UITemplate"))
	                        .SetMenuType(ETabSpawnerMenuType::Hidden);
}

void FUITemplateModule::ShutdownModule()
{
	// This function may be called during shutdown to clean up your module.  For modules that support dynamic reloading,
	// we call this function before unloading the module.
	FUITemplateStyle::Shutdown();

	FUITemplateCommands::Unregister();

	FGlobalTabmanager::Get()->UnregisterNomadTabSpawner(UITemplateTabName);
}

TSharedRef<SDockTab> FUITemplateModule::OnSpawnPluginTab(const FSpawnTabArgs& SpawnTabArgs)
{
	FText WidgetText = FText::Format(
		LOCTEXT("WindowWidgetText", "Add code to {0} in {1} to override this window's contents"),
		FText::FromString(TEXT("FUITemplateModule::OnSpawnPluginTab")),
		FText::FromString(TEXT("UITemplate.cpp"))
	);
	FString Path = IPluginManager::Get().FindPlugin("UITemplate")->GetBaseDir() / TEXT("Resources");
	FSlateBrush* bursh = new FSlateImageBrush(Path + "/ButtonIcon_40x.png", FVector2D(40, 40));

	//InitListViewData
	for (int i = 0; i < 100; i++)
	{
		TSharedPtr<ListViewItem> a = MakeShared<ListViewItem>(i, FString::Format(TEXT("Item:{0} "), {i}));
		ListViewItemL.Add(a);
	}

	PreviewTex = NewObject<UTextureRenderTarget2D>(GetTransientPackage(), TEXT("PreviewRT"));
	PreviewTex->InitAutoFormat(512, 512);
	
	FLevelEditorModule& LevelEditorModule = FModuleManager::LoadModuleChecked<FLevelEditorModule>("LevelEditor");
	TSharedPtr<FAssetThumbnailPool> ThumbnailPool = LevelEditorModule.GetFirstLevelEditor()->GetThumbnailPool();
	auto ret = SNew(SDockTab)
		.TabRole(ETabRole::NomadTab)
		[

			// Put your tab content here!
			SNew(SVerticalBox)
			+ SVerticalBox::Slot()
			.AutoHeight()
			[
				SNew(SBox)
				[
					SNew(STextBlock)
					.Text(FText(LOCTEXT("TextBlock", "TextBlock")))
				]
			]
			+ SVerticalBox::Slot()
			.AutoHeight()
			[
				SNew(SHorizontalBox)
				+ SHorizontalBox::Slot()
				.AutoWidth()
				[
					SNew(SBox)
					.HeightOverride(40)
					.WidthOverride(40)
					[
						SNew(SImage)
						.Image_Lambda([this, bursh]()
						{
							return bursh;
						})
					]
				]
				+ SHorizontalBox::Slot()
				.AutoWidth()
				[

					SNew(SButton)
						.Text(FText(LOCTEXT("Button", "Button")))
					.OnClicked_Lambda([]()
					             {
						             GEngine->AddOnScreenDebugMessage(-1, 2, FColor::Blue, TEXT("This is a buttn"));
						             return FReply::Handled();
					             })


				]
				+ SHorizontalBox::Slot()
				.AutoWidth()
				[
					SNew(SCheckBox)
						.ToolTipText(LOCTEXT("CheckBox", "CheckBox"))
						.OnCheckStateChanged_Lambda([](ECheckBoxState state)
					               {
						               if (state == ECheckBoxState::Checked)
						               {
							               GEngine->AddOnScreenDebugMessage(-1, 2, FColor::Blue, TEXT("Checked!!"));
						               }
						               else
						               {
							               GEngine->AddOnScreenDebugMessage(-1, 2, FColor::Blue, TEXT("UnChecked!!"));
						               }
					               })
						.Content()
					[
						SNew(STextBlock)
						.Text(LOCTEXT("CheckBoxContent", "CheckBoxContent"))
					]

				]
			]
			+ SVerticalBox::Slot()
			.AutoHeight()
			[
				SNew(SBox)
				.HeightOverride(100)
				.WidthOverride(50)
				[
					SAssignNew(ListView, SListView<TSharedPtr<ListViewItem>>)
				.OnGenerateRow_Lambda([](TSharedPtr<ListViewItem> itemData,
				                         const TSharedRef<class STableViewBase>& OwnerTable)-> TSharedRef<class
						                                                         ITableRow>
						                                                         {
							                                                         return SNew(
								                                                         STableRow<TSharedPtr<
								                                                         ListViewItem>>, OwnerTable)
							                                                         [
								                                                         SNew(SHorizontalBox)
								                                                         + SHorizontalBox::Slot()
								                                                         [
									                                                         SNew(STextBlock)
									                                                         .Text_Lambda(
										                                                         [itemData]()-> FText
										                                                         {
											                                                         return FText::
												                                                         FromString(
													                                                         itemData->
													                                                         Name);
										                                                         }
									                                                         )
								                                                         ]
							                                                         ];
						                                                         })
				.ListItemsSource(&ListViewItemL)
				]
			]
			+ SVerticalBox::Slot()
			.AutoHeight()
			[
				SNew(SBox)
				.HeightOverride(150)
				[
					SAssignNew(ViewportWidget, SViewport)
				.EnableGammaCorrection(false)
				.IsEnabled(FSlateApplication::Get().GetNormalExecutionAttribute())
				.ShowEffectWhenDisabled(false)
				.EnableBlending(true)
				]
			]
			+ SVerticalBox::Slot()
			[
				SNew(SObjectPropertyEntryBox)
				.ObjectPath_Lambda([this]()-> FString
				                             {
					                             return PropertyObjPath;
				                             })
				.AllowedClass(UMaterial::StaticClass())
												.OnObjectChanged_Lambda([this](const FAssetData& asset)
				                             {
					                             UE_LOG(LogTemp, Warning, TEXT("!!!!%s"), *asset.AssetName.ToString());
					                             PropertyObjPath = asset.ObjectPath.ToString();
				                             })
				.DisplayBrowse(true)
				.PropertyHandle(PHandle)
				.DisplayThumbnail(true)
				.ThumbnailPool(ThumbnailPool)

			]


		];

	ViewportClient = MakeShareable(new MyViewportClient(PreviewTex));

	SceneViewport = MakeShareable(new FSceneViewport(ViewportClient.Get(), ViewportWidget));

	ViewportWidget->SetViewportInterface(SceneViewport.ToSharedRef());
	return ret;
}

void FUITemplateModule::PluginButtonClicked()
{
	FGlobalTabmanager::Get()->InvokeTab(UITemplateTabName);
}

void FUITemplateModule::AddMenuExtension(FMenuBuilder& Builder)
{
	Builder.AddMenuEntry(FUITemplateCommands::Get().OpenPluginWindow);
}

void FUITemplateModule::AddToolbarExtension(FToolBarBuilder& Builder)
{
	Builder.AddToolBarButton(FUITemplateCommands::Get().OpenPluginWindow);
}

#undef LOCTEXT_NAMESPACE

IMPLEMENT_MODULE(FUITemplateModule, UITemplate)
