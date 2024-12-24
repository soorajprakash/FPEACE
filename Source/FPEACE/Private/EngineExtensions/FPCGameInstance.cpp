// Copyright © 2024 Sooraj Prakash. All rights reserved.
// Unauthorized distribution of this file, or any part of it, is prohibited.


#include "EngineExtensions/FPCGameInstance.h"

UFPCGameInstance* UFPCGameInstance::GameInstance = nullptr;

UFPCGameInstance::~UFPCGameInstance()
{
	GameInstance = nullptr;
}

UFPCGameInstance* UFPCGameInstance::GetInstance(const UObject* WorldContextObject)
{
	if (GameInstance == nullptr) GameInstance = CastChecked<UFPCGameInstance>(WorldContextObject->GetWorld()->GetGameInstance());

	return GameInstance;
}
