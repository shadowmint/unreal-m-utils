#pragma once

class MUTILS_API FMMath
{
public:
	/**
	 * Return true if the next value is <= P; P should be 0 <= P <= 1
	 * eg.
	 *		auto ChanceOfEvent = 0.5;
	 *		auto Happened = FMMathUtil::Chance(ChanceOfEvent);
	 */
	static bool Chance(float P);

	/**
	 * If you have a an array of objects with a weighted probability of P,
	 * pass in an array of <P, I> and this function will return I for the
	 * randomly picked item.
	 *
	 * Some I value will always be returned.
	 * eg.
	 *
	 *		struct FRule {
	 *			TSubclassOf<AActor> Mob;
	 *			float SpawnChance;
	 *		}
	 *
	 *		TArray<FRule> Rules;
	 *		auto Picked = FMMathUtil::PickWeighted<FRule>(Rules, [](FRule M)
	 *		{ return M.SpawnChance; });
	 */
	template <typename T>
	static TOptional<T> PickWeighted(TArray<T> Values, TFunctionRef<float(const T&)> ProbabilityOf)
	{
		if (Values.Num() == 0)
		{
			return TOptional<T>(); // Nothing had a non-zero chance of being picked.
		}
		
		float Total = 0;
		for (auto const& VRef : Values)
		{
			Total += ProbabilityOf(VRef);
		}

		if (Total == 0)
		{
			return TOptional<T>(); // Nothing had a non-zero chance of being picked.
		}

		auto Picked = FMath::RandRange(static_cast<float>(0), Total);
		float RunningTotal = 0;
		for (auto const& VRef : Values)
		{
			RunningTotal += ProbabilityOf(VRef);
			if (Picked <= RunningTotal)
			{
				return VRef;
			}
		}

		// It shouldn't really be possible to get here
		UE_LOG(LogTemp, Warning, TEXT("FMMathUtil::PickWeighted: Invalid value picked"));
		return TOptional<T>();
	}
};
