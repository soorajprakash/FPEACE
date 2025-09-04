// Copyright © Sooraj Prakash. All rights reserved.
// Unauthorized distribution of this file, or any part of it, is prohibited.

#include "FPCGameInstance.h"

UFPCGameInstance* UFPCGameInstance::GameInstance = nullptr;

UFPCGameInstance::~UFPCGameInstance()
{
	GameInstance = nullptr;
}

UFPCGameInstance* UFPCGameInstance::GetInstance(const UObject* WorldContextObject)
{
	if (GameInstance == nullptr) GameInstance = Cast<UFPCGameInstance>(WorldContextObject->GetWorld()->GetGameInstance());

	return GameInstance;
}
