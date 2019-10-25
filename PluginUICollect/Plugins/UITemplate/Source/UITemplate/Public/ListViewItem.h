#pragma once

#include "CoreMinimal.h"
class ListViewItem
{
public:
	ListViewItem(int idx, FString name)
	{
		Idx = idx;
		Name = name;
	}
	int Idx = 0;
	FString Name = "";
};