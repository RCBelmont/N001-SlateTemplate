// Copyright 1998-2019 Epic Games, Inc. All Rights Reserved.

#include "UITemplateCommands.h"

#define LOCTEXT_NAMESPACE "FUITemplateModule"

void FUITemplateCommands::RegisterCommands()
{
	UI_COMMAND(OpenPluginWindow, "UITemplate", "Bring up UITemplate window", EUserInterfaceActionType::Button, FInputGesture());
}

#undef LOCTEXT_NAMESPACE
