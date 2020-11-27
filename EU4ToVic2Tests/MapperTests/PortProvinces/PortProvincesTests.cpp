#include "PortProvinces/PortProvinces.h"
#include "gtest/gtest.h"

TEST(Mappers_PortProvincesTests, whitelistedProvinceCanBePinged)
{
	std::stringstream input;
	input << "whitelist = { 1 2 3 }\n";
	const mappers::PortProvinces mapper(input);

	ASSERT_TRUE(mapper.isProvinceIDWhitelisted(1));
	ASSERT_TRUE(mapper.isProvinceIDWhitelisted(2));
	ASSERT_TRUE(mapper.isProvinceIDWhitelisted(3));
}

TEST(Mappers_PortProvincesTests, blacklistedProvinceCanBePinged)
{
	std::stringstream input;
	input << "blacklist = { 1 2 3 }\n";
	const mappers::PortProvinces mapper(input);

	ASSERT_TRUE(mapper.isProvinceIDBlacklisted(1));
	ASSERT_TRUE(mapper.isProvinceIDBlacklisted(2));
	ASSERT_TRUE(mapper.isProvinceIDBlacklisted(3));
}

TEST(Mappers_PortProvincesTests, nonListedProvinceReturnsFalse)
{
	std::stringstream input;
	input << "whitelist = { 1 2 3 }\n";
	input << "blacklist = { 4 5 6 }\n";
	const mappers::PortProvinces mapper(input);

	ASSERT_FALSE(mapper.isProvinceIDBlacklisted(1));
	ASSERT_FALSE(mapper.isProvinceIDBlacklisted(2));
	ASSERT_FALSE(mapper.isProvinceIDBlacklisted(3));
	ASSERT_FALSE(mapper.isProvinceIDWhitelisted(4));
	ASSERT_FALSE(mapper.isProvinceIDWhitelisted(5));
	ASSERT_FALSE(mapper.isProvinceIDWhitelisted(6));
	
	ASSERT_FALSE(mapper.isProvinceIDWhitelisted(7));
	ASSERT_FALSE(mapper.isProvinceIDBlacklisted(7));
}
