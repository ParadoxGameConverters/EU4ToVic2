#include "Buildings/Building.h"
#include "gtest/gtest.h"

TEST(Mappers_BuildingTests, costDefaultsToZero)
{
	std::stringstream input;
	const mappers::Building theBuilding(input);

	ASSERT_EQ(0, theBuilding.getCost());
}

TEST(Mappers_BuildingTests, costCanBeSet)
{
	std::stringstream input;
	input << "cost = 100\n";
	const mappers::Building theBuilding(input);

	ASSERT_NEAR(100, theBuilding.getCost(), 0.001);
}

TEST(Mappers_BuildingTests, modifiersDefaultToEmpty)
{
	std::stringstream input;
	const mappers::Building theBuilding(input);

	ASSERT_TRUE(theBuilding.getModifier().getAllEffects().empty());
}

TEST(Mappers_BuildingTests, modifiersCanBeAdded)
{
	std::stringstream input;
	input << "modifier = {\n";
	input << "\taModifier = 0.5\n";
	input << "\tanotherModifier = 0.75\n";
	input << "}\n";
	const mappers::Building theBuilding(input);

	ASSERT_EQ(2, theBuilding.getModifier().getAllEffects().size());
	ASSERT_NEAR(0.5, theBuilding.getModifier().getAllEffects().at("aModifier"), 0.001);
	ASSERT_NEAR(0.75, theBuilding.getModifier().getAllEffects().at("anotherModifier"), 0.001);
}

TEST(Mappers_BuildingTests, manufacturyDefaultsToFalse)
{
	std::stringstream input;
	const mappers::Building theBuilding(input);

	ASSERT_FALSE(theBuilding.isManufactory());
}

TEST(Mappers_BuildingTests, manufacturyCanBeSet)
{
	std::stringstream input;
	input << "manufactory = {\n";
	input << "\tnaval_supplies\n";
	input << "\tfish\n";
	input << "\tsalt\n";
	input << "}\n";
	const mappers::Building theBuilding(input);

	ASSERT_TRUE(theBuilding.isManufactory());
}
