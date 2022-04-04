#include "Utils/MMath.h"

bool FMMath::Chance(const float P)
{
	constexpr float Min = 0;
	constexpr float Max = 1;
	return FMath::RandRange(Min, Max) <= P;
}