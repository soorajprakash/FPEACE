// Fill out your copyright notice in the Description page of Project Settings.


#include "FPCSkeletalMeshComponent.h"

UFPCSkeletalMeshComponent::UFPCSkeletalMeshComponent() : Super()
{
	VisibilityBasedAnimTickOption = EVisibilityBasedAnimTickOption::OnlyTickPoseWhenRendered;
}
