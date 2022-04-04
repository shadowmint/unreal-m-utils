#include "CoreMinimal.h"
#include "Tests/AutomationEditorCommon.h"
#include "Utils/MMath.h"

IMPLEMENT_SIMPLE_AUTOMATION_TEST(MMathTests, "Tests.MMathTests", EAutomationTestFlags::EditorContext | EAutomationTestFlags::EngineFilter)

bool MMathTests::RunTest(const FString& Parameters)
{
	check(FMMath::Chance(1));
	check(!FMMath::Chance(0));
	return true;
}
