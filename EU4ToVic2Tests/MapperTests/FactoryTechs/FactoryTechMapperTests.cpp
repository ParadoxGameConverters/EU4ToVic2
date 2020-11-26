#include "FactoryTechs/FactoryTechMapper.h"
#include "gtest/gtest.h"

TEST(Mappers_FactoryTechMapperTests, techMapStartsEmpty)
{
	std::stringstream input;
	const mappers::FactoryTechMapper mapper(input);

	ASSERT_TRUE(mapper.getFactoryTechMap().empty());
}

TEST(Mappers_FactoryTechMapperTests, techsCanBeLoaded)
{
	std::stringstream input;
	input << "empty_tech ={\n";
	input << "}";
	input << "full_tech ={\n";
	input << "\tactivate_building = factory1\n";
	input << "}";
	const mappers::FactoryTechMapper mapper(input);

	ASSERT_EQ(1, mapper.getFactoryTechMap().size());
	ASSERT_EQ("full_tech", mapper.getFactoryTechMap().at("factory1"));
}
