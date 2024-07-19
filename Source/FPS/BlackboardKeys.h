// Fill out your copyright notice in the Description page of Project Settings.
//AI기능 쓸려면 Build.cs에 추가"AIModule", "NavigationSystem", "GamePlayTasks"
#pragma once

#include "Runtime/Core/Public/UObject/NameTypes.h"
#include "Runtime/Core/Public/Containers/UnrealString.h"

namespace BBKeys
{
	const FName TargetKey = TEXT("Target");
	const FName PatrolPointKey = TEXT("PatrolPoint");
}