#include "Utils/MActor.h"

#include "Utils/MComponent.h"

void FMActor::SetVisibility(AActor* Actor, bool IsVisible)
{
	if (auto const Root = Actor->GetRootComponent())
	{
		Root->SetVisibility(IsVisible, true);
	}
	Actor->SetActorEnableCollision(IsVisible);
}

void FMActor::SetPhysicsEnabled(AActor* Actor, bool IsPhysicsEnabled)
{
	if (auto const Body = FMComponent::GetComponent<UPrimitiveComponent>(Actor))
	{
		Body->SetSimulatePhysics(IsPhysicsEnabled);
	}
}

void FMActor::SetTickEnabled(AActor* Actor, bool IsTickEnabled)
{
	Actor->SetActorTickEnabled(IsTickEnabled);
}

void FMActor::SetEnabled(AActor* Actor, FMEnabled Tick, FMEnabled Physics, FMEnabled Visibility)
{
	if (Tick != FMEnabled::DontChange)
	{
		SetTickEnabled(Actor, Tick == FMEnabled::Enabled);
	}
	if (Physics != FMEnabled::DontChange)
	{
		SetPhysicsEnabled(Actor, Physics == FMEnabled::Enabled);
	}
	if (Visibility != FMEnabled::DontChange)
	{
		SetVisibility(Actor, Visibility == FMEnabled::Enabled);
	}
}
