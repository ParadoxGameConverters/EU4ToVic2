#include "FactoryInventions/FactoryInventionMapper.h"
#include "gtest/gtest.h"

TEST(Mappers_FactoryInventionMapperTests, inventionMapStartsEmpty)
{
	std::stringstream input;
	const mappers::FactoryInventionMapper mapper(input);

	ASSERT_TRUE(mapper.getFactoryInventionMap().empty());
}

TEST(Mappers_FactoryInventionMapperTests, inventionsCanBeLoaded)
{
	std::stringstream input;
	input << "empty_invention ={\n";
	input << "\teffect = {\n";
	input << "\t}";
	input << "}";
	input << "full_invention ={\n";
	input << "\teffect = {\n";
	input << "\t\tactivate_building = factory1\n";
	input << "\t}";
	input << "}";
	const mappers::FactoryInventionMapper mapper(input);

	ASSERT_EQ(1, mapper.getFactoryInventionMap().size());
	ASSERT_EQ("full_invention", mapper.getFactoryInventionMap().at("factory1"));
}
