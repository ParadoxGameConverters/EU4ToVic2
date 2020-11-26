#include "../EU4toV2/Source/Configuration.h"
#include "../EU4toV2/Source/Mappers/ProvinceMappings/ProvinceMapper.h"
#include "gtest/gtest.h"
#include <sstream>

TEST(Mappers_ProvinceMapperTests, emptyMappingsDefaultToEmpty)
{
	std::stringstream colonialInput;
	std::stringstream input;
	input << "0.0.0.0 = {\n";
	input << "}";

	const Configuration testConfiguration;
	const mappers::ProvinceMapper theMapper(input, colonialInput, testConfiguration);

	ASSERT_EQ(theMapper.getVic2ProvinceNumbers(1).size(), 0);
}

TEST(Mappers_ProvinceMapperTests, canLookupVic2Provinces)
{
	std::stringstream colonialInput;
	std::stringstream input;
	input << "0.0.0.0 = {\n";
	input << "	link = { eu4 = 2 eu4 = 1 v2 = 2 v2 = 1 }\n";
	input << "}";

	const Configuration testConfiguration;
	const mappers::ProvinceMapper theMapper(input, colonialInput, testConfiguration);

	ASSERT_EQ(theMapper.getVic2ProvinceNumbers(1).size(), 2);
	ASSERT_EQ(theMapper.getVic2ProvinceNumbers(1)[0], 2);
	ASSERT_EQ(theMapper.getVic2ProvinceNumbers(1)[1], 1);
	ASSERT_EQ(theMapper.getVic2ProvinceNumbers(2).size(), 2);
	ASSERT_EQ(theMapper.getVic2ProvinceNumbers(2)[0], 2);
	ASSERT_EQ(theMapper.getVic2ProvinceNumbers(2)[1], 1);
}

TEST(Mappers_ProvinceMapperTests, canLookupEU4Provinces)
{
	std::stringstream colonialInput;
	std::stringstream input;
	input << "0.0.0.0 = {\n";
	input << "	link = { eu4 = 2 eu4 = 1 v2 = 2 v2 = 1 }\n";
	input << "}";

	const Configuration testConfiguration;
	const mappers::ProvinceMapper theMapper(input, colonialInput, testConfiguration);

	ASSERT_EQ(theMapper.getEU4ProvinceNumbers(1).size(), 2);
	ASSERT_EQ(theMapper.getEU4ProvinceNumbers(1)[0], 2);
	ASSERT_EQ(theMapper.getEU4ProvinceNumbers(1)[1], 1);
	ASSERT_EQ(theMapper.getEU4ProvinceNumbers(2).size(), 2);
	ASSERT_EQ(theMapper.getEU4ProvinceNumbers(2)[0], 2);
	ASSERT_EQ(theMapper.getEU4ProvinceNumbers(2)[1], 1);
}

TEST(Mappers_ProvinceMapperTests, properVesionIsUsed)
{
	std::stringstream colonialInput;
	std::stringstream input;
	input << "0.0.0.0 = {\n";
	input << "	link = { eu4 = 2 eu4 = 1 v2 = 2 v2 = 1 }\n";
	input << "}";
	input << "1.0.0.0 = {\n";
	input << "	link = { eu4 = 2 eu4 = 1 v2 = 3 v2 = 1 }\n";
	input << "}";
	input << "1.2.0.0 = {\n";
	input << "	link = { eu4 = 2 eu4 = 1 v2 = 4 v2 = 1 }\n";
	input << "}";

	Configuration testConfiguration;
	const GameVersion testVersion("1.1.0.0");
	testConfiguration.setEU4Version(testVersion);
	const mappers::ProvinceMapper theMapper(input, colonialInput, testConfiguration);

	ASSERT_EQ(theMapper.getVic2ProvinceNumbers(1).size(), 2);
	ASSERT_EQ(theMapper.getVic2ProvinceNumbers(1)[0], 3);
	ASSERT_EQ(theMapper.getVic2ProvinceNumbers(1)[1], 1);
}
