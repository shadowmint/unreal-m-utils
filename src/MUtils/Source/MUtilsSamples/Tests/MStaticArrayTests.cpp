#include "Misc/AutomationTest.h"
#include "Utils/MStaticArray.h"

IMPLEMENT_SIMPLE_AUTOMATION_TEST(MStaticArrayTests, "Tests.MStaticArrayTests",
                                 EAutomationTestFlags::EditorContext | EAutomationTestFlags::EngineFilter)

bool MStaticArrayTests::RunTest(const FString& Parameters)
{
	FName Keys[] = {TEXT("TargetObject"), TEXT("Radius"), TEXT("RotationState"), TEXT("RotationSpeed")};
	TMStaticArray<FName> KeyArray = TMStaticArray<FName>(Keys, 4, 4);
	check(KeyArray.Num() == 4);
	
	for (auto V : KeyArray)
	{
		UE_LOG(LogTemp, Display, TEXT("- %s"), *V.ToString());
	}
	
	auto Copy = KeyArray;
	for (auto V : Copy)
	{
		UE_LOG(LogTemp, Display, TEXT("- %s"), *V.ToString());
	}

	// You can't do this, because it's invalid!
	// TArray<FName> X = KeyArray

	// But you can do this...
	TArray<FName> ArrayLike = KeyArray.Collect();
	for (auto V : ArrayLike)
	{
		UE_LOG(LogTemp, Display, TEXT("- %s"), *V.ToString());
	}
	
	// Make the test pass by returning true, or fail by returning false.
	return true;
}
