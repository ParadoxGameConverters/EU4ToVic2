#include "gtest/gtest.h"
#include "../../EU4toV2/Source/Mappers/CountryMappings/CountryMapping.h"

TEST(Mappers_CountryMappingTests, SourceCountryDefaultsToBlank)
{
	std::stringstream input;
	const mappers::CountryMapping countryMapping(input);

	ASSERT_TRUE(countryMapping.getEU4Tag().empty());
}

TEST(Mappers_BlockedSchoolsTests, SourceCountryCanBeSet)
{
	std::stringstream input;
	input << "eu4 = TAG";
	mappers::CountryMapping countryMapping(input);

	ASSERT_EQ(countryMapping.getEU4Tag(), "TAG");
}

TEST(Mappers_CountryMappingTests, TargetCountryDefaultstoBlank)
{
	std::stringstream input;
	const mappers::CountryMapping countryMapping(input);

	ASSERT_TRUE(countryMapping.getVic2Tag().empty());
}

TEST(Mappers_BlockedSchoolsTests, TargetountryCanBeSet)
{
	std::stringstream input;
	input << "v2 = TAG";
	mappers::CountryMapping countryMapping(input);

	ASSERT_EQ(countryMapping.getVic2Tag(), "TAG");
}

TEST(Mappers_CountryMappingTests, FlagsDefaultToEmpty)
{
	std::stringstream input;
	const mappers::CountryMapping countryMapping(input);

	ASSERT_TRUE(countryMapping.getFlags().empty());
}

TEST(Mappers_BlockedSchoolsTests, FlagsCanBeLoaded)
{
	std::stringstream input;
	input << "flag = flag1";
	input << "\tflag = flag1";
	input << "\tflag = flag2";
	input << "\tflag = flag1";
	mappers::CountryMapping countryMapping(input);

	ASSERT_EQ(countryMapping.getFlags().size(), 2);
	ASSERT_TRUE(countryMapping.getFlags().count("flag1"));
	ASSERT_TRUE(countryMapping.getFlags().count("flag2"));
}

TEST(Mappers_CountryMappingTests, ReformsDefaultToEmpty)
{
	std::stringstream input;
	const mappers::CountryMapping countryMapping(input);

	ASSERT_TRUE(countryMapping.getReforms().empty());
}

TEST(Mappers_BlockedSchoolsTests, ReformsCanBeLoaded)
{
	std::stringstream input;
	input << "reform = reform1";
	input << "\treform = reform1";
	input << "\treform = reform2";
	input << "\treform = reform1";
	mappers::CountryMapping countryMapping(input);

	ASSERT_EQ(countryMapping.getReforms().size(), 2);
	ASSERT_TRUE(countryMapping.getReforms().count("reform1"));
	ASSERT_TRUE(countryMapping.getReforms().count("reform2"));
}
