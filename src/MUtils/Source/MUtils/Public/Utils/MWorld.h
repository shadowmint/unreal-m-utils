// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Kismet/GameplayStatics.h"
#include "UObject/Object.h"

class MUTILS_API FMWorld
{
public:
	/**
	 * Collects all actors of the given type matching all given tags into OutActors.
	 * Returns the count of matching actors.
	 */
	template <typename ActorType>
	static int FindActorsMatchingTags(UObject* WorldContextObject, TArray<FName> Tags, TArray<ActorType*>& OutActors)
	{
		OutActors.Reset();

		if (Tags.Num() == 0)
		{
			return 0;
		}

		TArray<AActor*> RawActors;
		UGameplayStatics::GetAllActorsOfClassWithTag(WorldContextObject, ActorType::StaticClass(), Tags[0], RawActors);
		for (auto const Actor : RawActors)
		{
			auto IsValid = true;
			for (auto const Tag : Tags)
			{
				if (!Actor->Tags.Contains(Tag))
				{
					IsValid = false;
					break;
				}
			}
			if (IsValid)
			{
				auto const TypedActor = Cast<ActorType>(Actor);
				if (TypedActor)
				{
					OutActors.Add(TypedActor);
				}
			}
		}

		return OutActors.Num();
	}
};
