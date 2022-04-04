// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"

class MUTILS_API FMComponent
{
public:
	/** Return a single matching component */
	template <typename ComponentType>
	static ComponentType* GetComponent(AActor* FromActor, bool Debug = false)
	{
		if (!FromActor) return nullptr;

		TArray<UActorComponent*> Components;
		FromActor->GetComponents(ComponentType::StaticClass(), Components, true);
		if (Components.Num() > 0)
		{
			return Cast<ComponentType>(Components[0]);
		}
		if (Debug)
		{
			Components.Reset();
			FromActor->GetComponents(Components, true);
			UE_LOG(LogTemp, Warning, TEXT("MissingComponent: %s in:"), *ComponentType::StaticClass()->GetName());
			for (const auto& Cmp : Components)
			{
				UE_LOG(LogTemp, Warning, TEXT("- %s"), *Cmp->GetClass()->GetName());
			}
		}
		return nullptr;
	}

	template <typename ComponentType>
	static void GetComponents(AActor* Actor, TArray<ComponentType*> &Components, bool Debug = false)
	{
		TArray<UActorComponent*> Cmp;
		Actor->GetComponents(ComponentType::StaticClass(), Cmp, true);
		for (auto* C : Cmp)
		{
			const auto TypedC = Cast<ComponentType>(C);
			Components.Add(TypedC);
		}
		if (Debug && Components.Num() == 0)
		{
			Actor->GetComponents(Components, true);
			UE_LOG(LogTemp, Warning, TEXT("MissingComponent: %s in:"), *ComponentType::StaticClass()->GetName());
			for (const auto& Item : Components)
			{
				UE_LOG(LogTemp, Warning, TEXT("- %s"), *Item->GetClass()->GetName());
			}
		}
	}
};
