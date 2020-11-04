#include "Modifiers/Modifier.h"
#include "gtest/gtest.h"

TEST(EU4World_ModifierTests, effectsDefaultToEmpty)
{
	std::stringstream input;
	const EU4::Modifier theModifier(input);

	ASSERT_TRUE(theModifier.getAllEffects().empty());
}

TEST(EU4World_ModifierTests, nonExistentEffectHasAmountOfZero)
{
	std::stringstream input;
	const EU4::Modifier theModifier(input);

	ASSERT_EQ(0, theModifier.getEffectAmount("non_existent_effect"));
}

TEST(EU4World_ModifierTests, doubleEffectsCanBeMatched)
{
	std::stringstream input;
	input << "effect1 = 0.5\n";
	input << "effect2 = 0.75\n";
	const EU4::Modifier theModifier(input);

	ASSERT_NEAR(0.5, theModifier.getEffectAmount("effect1"), 0.001);
	ASSERT_NEAR(0.75, theModifier.getEffectAmount("effect2"), 0.001);
}

TEST(EU4World_ModifierTests, integerEffectsCanBeMapped)
{
	std::stringstream input;
	input << "effect1 = 5\n";
	input << "effect2 = 15\n";
	const EU4::Modifier theModifier(input);

	ASSERT_NEAR(5, theModifier.getEffectAmount("effect1"), 0.001);
	ASSERT_NEAR(15, theModifier.getEffectAmount("effect2"), 0.001);
}

TEST(EU4World_ModifierTests, nonDecimalEffectsAreNotMapped)
{
	std::stringstream input;
	input << "effect = textual_effect\n";
	const EU4::Modifier theModifier(input);

	ASSERT_EQ(0, theModifier.getEffectAmount("effect"));
}
