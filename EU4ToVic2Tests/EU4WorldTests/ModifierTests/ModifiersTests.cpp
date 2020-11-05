#include "Modifiers/Modifiers.h"
#include "gtest/gtest.h"

TEST(EU4World_ModifiersTests, nonExistentModifierReturnsNullopt)
{
	std::stringstream input;
	const EU4::Modifiers theModifiers(input);

	ASSERT_FALSE(theModifiers.getModifier("nonModifier"));
}

TEST(EU4World_ModifiersTests, modifierIsReturned)
{
	std::stringstream input;
	input << "testModifier = {\n";
	input << "\teffect = 100\n";
	input << "\teffect2 = 200\n";
	input << "}";
	input << "testModifier2 = {\n";
	input << "\teffect = 200\n";
	input << "}";
	const EU4::Modifiers theModifiers(input);

	ASSERT_EQ(theModifiers.getModifier("testModifier")->getEffectAmount("effect"), 100);
	ASSERT_EQ(theModifiers.getModifier("testModifier")->getEffectAmount("effect2"), 200);
	ASSERT_EQ(theModifiers.getModifier("testModifier2")->getEffectAmount("effect"), 200);
}