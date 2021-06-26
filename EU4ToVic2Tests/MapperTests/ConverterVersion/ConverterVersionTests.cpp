#include "../../EU4toV2/Source/Mappers/ConverterVersion/ConverterVersion.h"
#include "gtest/gtest.h"

TEST(Mappers_ConverterVersionTests, PrimitivesDefaultToEmpty)
{
	std::stringstream input;
	const mappers::ConverterVersion version(input);

	EXPECT_TRUE(version.getName().empty());
	EXPECT_TRUE(version.getVersion().empty());
}

TEST(Mappers_ConverterVersionTests, NameCanBeSet)
{
	std::stringstream input;
	input << "name = \"Test Name\"";
	const mappers::ConverterVersion version(input);

	EXPECT_EQ("Test Name", version.getName());
}

TEST(Mappers_ConverterVersionTests, VersionCanBeSet)
{
	std::stringstream input;
	input << "version = \"5.4-Test\"";
	const mappers::ConverterVersion version(input);

	EXPECT_EQ("5.4-Test", version.getVersion());
}

TEST(Mappers_ConverterVersionTests, DescriptionCanBeConstructed)
{
	std::stringstream input;
	input << "source = \"EU4\"\n";
	input << "target = \"Vic2\"\n";
	input << "minSource = \"1.31\"\n";
	input << "maxSource = \"1.32\"\n";
	input << "minTarget = \"3.0\"\n";
	input << "maxTarget = \"3.1\"\n";
	const mappers::ConverterVersion version(input);

	EXPECT_EQ("Compatible with EU4 [v1.31-v1.32] and Vic2 [v3.0-v3.1]", version.getDescription());
}

TEST(Mappers_ConverterVersionTests, DescriptionDoesNotDuplicateVersions)
{
	std::stringstream input;
	input << "source = \"EU4\"\n";
	input << "target = \"Vic2\"\n";
	input << "minSource = \"1.31\"\n";
	input << "maxSource = \"1.31\"\n";
	input << "minTarget = \"3.0\"\n";
	input << "maxTarget = \"3.0\"\n";
	const mappers::ConverterVersion version(input);

	EXPECT_EQ("Compatible with EU4 [v1.31] and Vic2 [v3.0]", version.getDescription());
}