/**
 * InstaLODTypes.h (InstaLOD)
 *
 * Copyright 2016-2024 InstaLOD GmbH - All Rights Reserved.
 *
 * Unauthorized copying of this file, via any medium is strictly prohibited.
 * This file and all it's contents are proprietary and confidential.
 *
 * @file InstaLODTypes.h
 * @copyright 2016-2024 InstaLOD GmbH. All rights reserved.
 * @section License
 */

#pragma once
#include "Camera/CameraComponent.h"
#include "Components/SkeletalMeshComponent.h"
#include "Components/StaticMeshComponent.h"
 
#include "Runtime/Engine/Classes/Camera/CameraComponent.h" 

/**
 * A union type structure for selected components and if they should be
 * included into the tools actions. 
 */
struct FInstaLODMeshComponent
{
	FInstaLODMeshComponent(class UStaticMeshComponent* InStaticMeshComp) :
	StaticMeshComponent(InStaticMeshComp),
	SkeletalMeshComponent(nullptr),
	CameraComponent(nullptr),
	bShouldBeIncluded(true)
	{
	}

	FInstaLODMeshComponent(class USkeletalMeshComponent* InSkeletalMeshComp) :
	StaticMeshComponent(nullptr),
	SkeletalMeshComponent(InSkeletalMeshComp),
	CameraComponent(nullptr),
	bShouldBeIncluded(true)
	{
	}
	
	FInstaLODMeshComponent(class UCameraComponent* InCameraComponent) :
	StaticMeshComponent(nullptr),
	SkeletalMeshComponent(nullptr),
	CameraComponent(InCameraComponent),
	bShouldBeIncluded(true)
	{
	}

	bool IsValid() const 
	{
		return GetComponent() != nullptr;
	}
	
	USceneComponent* GetComponent() const
	{
		if (StaticMeshComponent.IsValid())
			return StaticMeshComponent.Get();

		if (SkeletalMeshComponent.IsValid())
			return SkeletalMeshComponent.Get();

		if (CameraComponent.IsValid())
			return CameraComponent.Get();

		return nullptr;
	}
	
	USceneComponent* GetComponent()
	{
		if (StaticMeshComponent.IsValid())
			return StaticMeshComponent.Get();

		if (SkeletalMeshComponent.IsValid())
			return SkeletalMeshComponent.Get();

		if (CameraComponent.IsValid())
			return CameraComponent.Get();

		return nullptr;
	}

	/**
	 * Gets the display name visible in the hierarchy browser.
	 * 
	 * @return The scene name.
	 */
	FString GetDisplayName() const
	{
		FString Name;

		if (USceneComponent* const Component = GetComponent())
		{
			Name = Component->GetOwner()->GetActorNameOrLabel();
		}

		return Name;
	}
	
	TWeakObjectPtr<class UStaticMeshComponent> StaticMeshComponent;		/**< StaticMesh from selected Actor. */
	TWeakObjectPtr<class USkeletalMeshComponent> SkeletalMeshComponent;	/**< SkeletalMesh from selected Actor. */
	TWeakObjectPtr<class UCameraComponent> CameraComponent;				/**< CameraComponent from selected Actor. */
	
	bool bShouldBeIncluded; /**< flag determining whether or not this Component should be included into the tools actions. */
};

