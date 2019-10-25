// Copyright 1998-2019 Epic Games, Inc. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "Framework/Commands/Commands.h"
#include "UITemplateStyle.h"

class FUITemplateCommands : public TCommands<FUITemplateCommands>
{
public:

	FUITemplateCommands()
		: TCommands<FUITemplateCommands>(TEXT("UITemplate"), NSLOCTEXT("Contexts", "UITemplate", "UITemplate Plugin"), NAME_None, FUITemplateStyle::GetStyleSetName())
	{
	}

	// TCommands<> interface
	virtual void RegisterCommands() override;

public:
	TSharedPtr< FUICommandInfo > OpenPluginWindow;
};