// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"

enum class FMEnabled : uint8
{
	Enabled,
	Disabled,
	DontChange
};

class MUTILS_API FMActor
{
public:
	static void SetVisibility(AActor* Actor, bool IsVisible);

	static void SetPhysicsEnabled(AActor* Actor, bool IsPhysicsEnabled);

	static void SetTickEnabled(AActor* Actor, bool IsTickEnabled);

	static void SetEnabled(AActor* Actor, FMEnabled Tick, FMEnabled Physics, FMEnabled Visibility);
};
