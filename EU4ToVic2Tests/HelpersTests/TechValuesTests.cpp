#include "gtest/gtest.h"
#include "../EU4toV2/Source/Helpers/TechValues.h"
#include "../Mocks/EU4CountryMock.h"
#include "../Mocks/Vic2CountryMock.h"



TEST(Helpers_TechValuesTests, notValidCountryForTechConversionIfUncivilized)
{
	std::map<std::string, std::shared_ptr<V2::Country>> countries;

	helpers::TechValues techValues(countries);

	auto testCountry = std::make_shared<mockVic2Country>();
	EXPECT_CALL(*testCountry, isCivilized).WillOnce(testing::Return(false));

	ASSERT_FALSE(techValues.isValidCountryForTechConversion(*testCountry));
}


TEST(Helpers_TechValuesTests, notValidCountryForTechConversionIfNoProvinces)
{
	std::map<std::string, std::shared_ptr<V2::Country>> countries;

	helpers::TechValues techValues(countries);

	auto testCountry = std::make_shared<mockVic2Country>();
	EXPECT_CALL(*testCountry, isCivilized).WillOnce(testing::Return(true));
	std::map<int, std::shared_ptr<V2::Province>> emptyProvinces;
	EXPECT_CALL(*testCountry, getProvinces).WillOnce(testing::ReturnRef(emptyProvinces));

	ASSERT_FALSE(techValues.isValidCountryForTechConversion(*testCountry));
}


TEST(Helpers_TechValuesTests, notValidCountryForTechConversionIfNoSourceCountry)
{
	std::map<std::string, std::shared_ptr<V2::Country>> countries;

	helpers::TechValues techValues(countries);

	auto testCountry = std::make_shared<mockVic2Country>();
	EXPECT_CALL(*testCountry, isCivilized).WillOnce(testing::Return(true));
	std::map<int, std::shared_ptr<V2::Province>> provinces;
	std::shared_ptr<V2::Province> province;
	provinces.insert(std::make_pair(1, province));
	EXPECT_CALL(*testCountry, getProvinces).WillOnce(testing::ReturnRef(provinces));
	std::shared_ptr<EU4::Country> emptyCountry;
	EXPECT_CALL(*testCountry, getSourceCountry).WillOnce(testing::Return(emptyCountry));

	ASSERT_FALSE(techValues.isValidCountryForTechConversion(*testCountry));
}


TEST(Helpers_TechValuesTests, validCountryForTechConversionIfAllChecksPass)
{
	std::map<std::string, std::shared_ptr<V2::Country>> countries;

	helpers::TechValues techValues(countries);

	auto testCountry = std::make_shared<mockVic2Country>();
	EXPECT_CALL(*testCountry, isCivilized).WillOnce(testing::Return(true));
	std::map<int, std::shared_ptr<V2::Province>> provinces;
	std::shared_ptr<V2::Province> province;
	provinces.insert(std::make_pair(1, province));
	EXPECT_CALL(*testCountry, getProvinces).WillOnce(testing::ReturnRef(provinces));
	std::shared_ptr<EU4::Country> country = std::make_shared<EU4::Country>();
	EXPECT_CALL(*testCountry, getSourceCountry).WillOnce(testing::Return(country));

	ASSERT_TRUE(techValues.isValidCountryForTechConversion(*testCountry));
}



TEST(Helpers_TechValuesTests, getNormalizedArmyTechReturnsNegativeOneIfInitializedWithNoCountries)
{
	std::map<std::string, std::shared_ptr<V2::Country>> countries;

	mockEU4Country country;
	EXPECT_CALL(country, getAdmTech).WillOnce(testing::Return(32));
	EXPECT_CALL(country, getMilTech).WillOnce(testing::Return(32));

	helpers::TechValues techValues(countries);

	ASSERT_EQ(techValues.getNormalizedArmyTech(country), -1.0);
}


TEST(Helpers_TechValuesTests, getNormalizedArmyTechReturnsOneIfInitializedWithOneCountries)
{
	std::shared_ptr<mockEU4Country> country = std::make_shared<mockEU4Country>();
	EXPECT_CALL(*country, getAdmTech).WillRepeatedly(testing::Return(32));
	EXPECT_CALL(*country, getDipTech).WillRepeatedly(testing::Return(32));
	EXPECT_CALL(*country, getMilTech).WillRepeatedly(testing::Return(32));
	EXPECT_CALL(*country, getArmy).WillRepeatedly(testing::Return(0));
	EXPECT_CALL(*country, getNavy).WillRepeatedly(testing::Return(0));
	EXPECT_CALL(*country, getCommerce).WillRepeatedly(testing::Return(0));
	EXPECT_CALL(*country, getIndustry).WillRepeatedly(testing::Return(0));
	EXPECT_CALL(*country, getCulture).WillRepeatedly(testing::Return(0));
	std::set<std::string> nationalIdeas;
	EXPECT_CALL(*country, getNationalIdeas).WillRepeatedly(testing::ReturnRef(nationalIdeas));

	mockVic2Country testCountry;
	EXPECT_CALL(testCountry, isCivilized).WillOnce(testing::Return(true));
	std::map<int, std::shared_ptr<V2::Province>> provinces;
	std::shared_ptr<V2::Province> province;
	provinces.insert(std::make_pair(1, province));
	EXPECT_CALL(testCountry, getProvinces).WillOnce(testing::ReturnRef(provinces));
	EXPECT_CALL(testCountry, getSourceCountry).WillRepeatedly(testing::Return(country));
	std::map<std::string, std::shared_ptr<V2::Country>> countries;
	countries.insert(std::make_pair("TST", &testCountry));
	
	helpers::TechValues techValues(countries);

	ASSERT_EQ(techValues.getNormalizedArmyTech(*country), 1.0);
}


TEST(Helpers_TechValuesTests, getNormalizedArmyTechReturnsOneForHighestScore)
{
	std::shared_ptr<mockEU4Country> country = std::make_shared<mockEU4Country>();
	EXPECT_CALL(*country, getAdmTech).WillRepeatedly(testing::Return(32));
	EXPECT_CALL(*country, getDipTech).WillRepeatedly(testing::Return(32));
	EXPECT_CALL(*country, getMilTech).WillRepeatedly(testing::Return(32));
	std::set<std::string> nationalIdeas;
	EXPECT_CALL(*country, getNationalIdeas).WillRepeatedly(testing::ReturnRef(nationalIdeas));

	mockVic2Country testCountry;
	EXPECT_CALL(testCountry, isCivilized).WillRepeatedly(testing::Return(true));
	std::map<int, std::shared_ptr<V2::Province>> provinces;
	std::shared_ptr<V2::Province> province;
	provinces.insert(std::make_pair(1, province));
	EXPECT_CALL(testCountry, getProvinces).WillRepeatedly(testing::ReturnRef(provinces));
	EXPECT_CALL(testCountry, getSourceCountry).WillRepeatedly(testing::Return(country));

	std::shared_ptr<mockEU4Country> country2 = std::make_shared<mockEU4Country>();
	EXPECT_CALL(*country2, getAdmTech).WillRepeatedly(testing::Return(8));
	EXPECT_CALL(*country2, getDipTech).WillRepeatedly(testing::Return(8));
	EXPECT_CALL(*country2, getMilTech).WillRepeatedly(testing::Return(8));
	EXPECT_CALL(*country2, getNationalIdeas).WillRepeatedly(testing::ReturnRef(nationalIdeas));

	mockVic2Country testCountry2;
	EXPECT_CALL(testCountry2, isCivilized).WillRepeatedly(testing::Return(true));
	EXPECT_CALL(testCountry2, getProvinces).WillRepeatedly(testing::ReturnRef(provinces));
	EXPECT_CALL(testCountry2, getSourceCountry).WillRepeatedly(testing::Return(country2));

	std::shared_ptr<mockEU4Country> country3 = std::make_shared<mockEU4Country>();
	EXPECT_CALL(*country3, getAdmTech).WillRepeatedly(testing::Return(0));
	EXPECT_CALL(*country3, getDipTech).WillRepeatedly(testing::Return(0));
	EXPECT_CALL(*country3, getMilTech).WillRepeatedly(testing::Return(0));
	EXPECT_CALL(*country3, getNationalIdeas).WillRepeatedly(testing::ReturnRef(nationalIdeas));

	mockVic2Country testCountry3;
	EXPECT_CALL(testCountry3, isCivilized).WillRepeatedly(testing::Return(true));
	EXPECT_CALL(testCountry3, getProvinces).WillRepeatedly(testing::ReturnRef(provinces));
	EXPECT_CALL(testCountry3, getSourceCountry).WillRepeatedly(testing::Return(country3));

	std::map<std::string, std::shared_ptr<V2::Country>> countries;
	countries.insert(std::make_pair("TST", &testCountry));
	countries.insert(std::make_pair("MID", &testCountry3));
	countries.insert(std::make_pair("LOW", &testCountry3));

	helpers::TechValues techValues(countries);

	ASSERT_EQ(techValues.getNormalizedArmyTech(*country), 1.0);
}


TEST(Helpers_TechValuesTests, getNormalizedArmyTechReturnsScaledScore)
{
	std::shared_ptr<mockEU4Country> country = std::make_shared<mockEU4Country>();
	EXPECT_CALL(*country, getAdmTech).WillRepeatedly(testing::Return(32));
	EXPECT_CALL(*country, getDipTech).WillRepeatedly(testing::Return(32));
	EXPECT_CALL(*country, getMilTech).WillRepeatedly(testing::Return(32));
	std::set<std::string> nationalIdeas;
	EXPECT_CALL(*country, getNationalIdeas).WillRepeatedly(testing::ReturnRef(nationalIdeas));

	mockVic2Country testCountry;
	EXPECT_CALL(testCountry, isCivilized).WillRepeatedly(testing::Return(true));
	std::map<int, std::shared_ptr<V2::Province>> provinces;
	std::shared_ptr<V2::Province> province;
	provinces.insert(std::make_pair(1, province));
	EXPECT_CALL(testCountry, getProvinces).WillRepeatedly(testing::ReturnRef(provinces));
	EXPECT_CALL(testCountry, getSourceCountry).WillRepeatedly(testing::Return(country));

	std::shared_ptr<mockEU4Country> country2 = std::make_shared<mockEU4Country>();
	EXPECT_CALL(*country2, getAdmTech).WillRepeatedly(testing::Return(8));
	EXPECT_CALL(*country2, getDipTech).WillRepeatedly(testing::Return(8));
	EXPECT_CALL(*country2, getMilTech).WillRepeatedly(testing::Return(8));
	EXPECT_CALL(*country2, getNationalIdeas).WillRepeatedly(testing::ReturnRef(nationalIdeas));

	mockVic2Country testCountry2;
	EXPECT_CALL(testCountry2, isCivilized).WillRepeatedly(testing::Return(true));
	EXPECT_CALL(testCountry2, getProvinces).WillRepeatedly(testing::ReturnRef(provinces));
	EXPECT_CALL(testCountry2, getSourceCountry).WillRepeatedly(testing::Return(country2));

	std::shared_ptr<mockEU4Country> country3 = std::make_shared<mockEU4Country>();
	EXPECT_CALL(*country3, getAdmTech).WillRepeatedly(testing::Return(0));
	EXPECT_CALL(*country3, getDipTech).WillRepeatedly(testing::Return(0));
	EXPECT_CALL(*country3, getMilTech).WillRepeatedly(testing::Return(0));
	EXPECT_CALL(*country3, getNationalIdeas).WillRepeatedly(testing::ReturnRef(nationalIdeas));

	mockVic2Country testCountry3;
	EXPECT_CALL(testCountry3, isCivilized).WillRepeatedly(testing::Return(true));
	EXPECT_CALL(testCountry3, getProvinces).WillRepeatedly(testing::ReturnRef(provinces));
	EXPECT_CALL(testCountry3, getSourceCountry).WillRepeatedly(testing::Return(country3));

	std::map<std::string, std::shared_ptr<V2::Country>> countries;
	countries.insert(std::make_pair("TST", &testCountry));
	countries.insert(std::make_pair("MID", &testCountry2));
	countries.insert(std::make_pair("LOW", &testCountry3));

	helpers::TechValues techValues(countries);

	ASSERT_NEAR(techValues.getNormalizedArmyTech(*country2), (-2.0 / 7.0), 0.00001);
}


TEST(Helpers_TechValuesTests, getNormalizedNavyTechReturnsNegativeOneIfInitializedWithNoCountries)
{
	std::map<std::string, std::shared_ptr<V2::Country>> countries;

	helpers::TechValues techValues(countries);

	mockEU4Country country;
	EXPECT_CALL(country, getDipTech).WillOnce(testing::Return(32));
	EXPECT_CALL(country, getMilTech).WillOnce(testing::Return(32));

	ASSERT_EQ(techValues.getNormalizedNavyTech(country), -1.0);
}


TEST(Helpers_TechValuesTests, getNormalizedNavyTechReturnsOneIfInitializedWithOneCountries)
{
	std::shared_ptr<mockEU4Country> country = std::make_shared<mockEU4Country>();
	EXPECT_CALL(*country, getAdmTech).WillRepeatedly(testing::Return(32));
	EXPECT_CALL(*country, getDipTech).WillRepeatedly(testing::Return(32));
	EXPECT_CALL(*country, getMilTech).WillRepeatedly(testing::Return(32));
	std::set<std::string> nationalIdeas;
	EXPECT_CALL(*country, getNationalIdeas).WillRepeatedly(testing::ReturnRef(nationalIdeas));

	mockVic2Country testCountry;
	EXPECT_CALL(testCountry, isCivilized).WillOnce(testing::Return(true));
	std::map<int, std::shared_ptr<V2::Province>> provinces;
	std::shared_ptr<V2::Province> province;
	provinces.insert(std::make_pair(1, province));
	EXPECT_CALL(testCountry, getProvinces).WillOnce(testing::ReturnRef(provinces));
	EXPECT_CALL(testCountry, getSourceCountry).WillRepeatedly(testing::Return(country));
	std::map<std::string, std::shared_ptr<V2::Country>> countries;
	countries.insert(std::make_pair("TST", &testCountry));

	helpers::TechValues techValues(countries);

	ASSERT_EQ(techValues.getNormalizedNavyTech(*country), 1.0);
}


TEST(Helpers_TechValuesTests, getNormalizedNavyTechReturnsOneForHighestScore)
{
	std::shared_ptr<mockEU4Country> country = std::make_shared<mockEU4Country>();
	EXPECT_CALL(*country, getAdmTech).WillRepeatedly(testing::Return(32));
	EXPECT_CALL(*country, getDipTech).WillRepeatedly(testing::Return(32));
	EXPECT_CALL(*country, getMilTech).WillRepeatedly(testing::Return(32));
	std::set<std::string> nationalIdeas;
	EXPECT_CALL(*country, getNationalIdeas).WillRepeatedly(testing::ReturnRef(nationalIdeas));

	mockVic2Country testCountry;
	EXPECT_CALL(testCountry, isCivilized).WillRepeatedly(testing::Return(true));
	std::map<int, std::shared_ptr<V2::Province>> provinces;
	std::shared_ptr<V2::Province> province;
	provinces.insert(std::make_pair(1, province));
	EXPECT_CALL(testCountry, getProvinces).WillRepeatedly(testing::ReturnRef(provinces));
	EXPECT_CALL(testCountry, getSourceCountry).WillRepeatedly(testing::Return(country));

	std::shared_ptr<mockEU4Country> country2 = std::make_shared<mockEU4Country>();
	EXPECT_CALL(*country2, getAdmTech).WillRepeatedly(testing::Return(8));
	EXPECT_CALL(*country2, getDipTech).WillRepeatedly(testing::Return(8));
	EXPECT_CALL(*country2, getMilTech).WillRepeatedly(testing::Return(8));
	EXPECT_CALL(*country2, getNationalIdeas).WillRepeatedly(testing::ReturnRef(nationalIdeas));

	mockVic2Country testCountry2;
	EXPECT_CALL(testCountry2, isCivilized).WillRepeatedly(testing::Return(true));
	EXPECT_CALL(testCountry2, getProvinces).WillRepeatedly(testing::ReturnRef(provinces));
	EXPECT_CALL(testCountry2, getSourceCountry).WillRepeatedly(testing::Return(country2));

	std::shared_ptr<mockEU4Country> country3 = std::make_shared<mockEU4Country>();
	EXPECT_CALL(*country3, getAdmTech).WillRepeatedly(testing::Return(0));
	EXPECT_CALL(*country3, getDipTech).WillRepeatedly(testing::Return(0));
	EXPECT_CALL(*country3, getMilTech).WillRepeatedly(testing::Return(0));
	EXPECT_CALL(*country3, getNationalIdeas).WillRepeatedly(testing::ReturnRef(nationalIdeas));

	mockVic2Country testCountry3;
	EXPECT_CALL(testCountry3, isCivilized).WillRepeatedly(testing::Return(true));
	EXPECT_CALL(testCountry3, getProvinces).WillRepeatedly(testing::ReturnRef(provinces));
	EXPECT_CALL(testCountry3, getSourceCountry).WillRepeatedly(testing::Return(country3));

	std::map<std::string, std::shared_ptr<V2::Country>> countries;
	countries.insert(std::make_pair("TST", &testCountry));
	countries.insert(std::make_pair("MID", &testCountry3));
	countries.insert(std::make_pair("LOW", &testCountry3));

	helpers::TechValues techValues(countries);

	ASSERT_EQ(techValues.getNormalizedNavyTech(*country), 1.0);
}


TEST(Helpers_TechValuesTests, getNormalizedNavyTechReturnsScaledScore)
{
	std::shared_ptr<mockEU4Country> country = std::make_shared<mockEU4Country>();
	EXPECT_CALL(*country, getAdmTech).WillRepeatedly(testing::Return(32));
	EXPECT_CALL(*country, getDipTech).WillRepeatedly(testing::Return(32));
	EXPECT_CALL(*country, getMilTech).WillRepeatedly(testing::Return(32));
	std::set<std::string> nationalIdeas;
	EXPECT_CALL(*country, getNationalIdeas).WillRepeatedly(testing::ReturnRef(nationalIdeas));

	mockVic2Country testCountry;
	EXPECT_CALL(testCountry, isCivilized).WillRepeatedly(testing::Return(true));
	std::map<int, std::shared_ptr<V2::Province>> provinces;
	std::shared_ptr<V2::Province> province;
	provinces.insert(std::make_pair(1, province));
	EXPECT_CALL(testCountry, getProvinces).WillRepeatedly(testing::ReturnRef(provinces));
	EXPECT_CALL(testCountry, getSourceCountry).WillRepeatedly(testing::Return(country));

	std::shared_ptr<mockEU4Country> country2 = std::make_shared<mockEU4Country>();
	EXPECT_CALL(*country2, getAdmTech).WillRepeatedly(testing::Return(8));
	EXPECT_CALL(*country2, getDipTech).WillRepeatedly(testing::Return(8));
	EXPECT_CALL(*country2, getMilTech).WillRepeatedly(testing::Return(8));
	EXPECT_CALL(*country2, getNationalIdeas).WillRepeatedly(testing::ReturnRef(nationalIdeas));

	mockVic2Country testCountry2;
	EXPECT_CALL(testCountry2, isCivilized).WillRepeatedly(testing::Return(true));
	EXPECT_CALL(testCountry2, getProvinces).WillRepeatedly(testing::ReturnRef(provinces));
	EXPECT_CALL(testCountry2, getSourceCountry).WillRepeatedly(testing::Return(country2));

	std::shared_ptr<mockEU4Country> country3 = std::make_shared<mockEU4Country>();
	EXPECT_CALL(*country3, getAdmTech).WillRepeatedly(testing::Return(0));
	EXPECT_CALL(*country3, getDipTech).WillRepeatedly(testing::Return(0));
	EXPECT_CALL(*country3, getMilTech).WillRepeatedly(testing::Return(0));
	EXPECT_CALL(*country3, getNationalIdeas).WillRepeatedly(testing::ReturnRef(nationalIdeas));

	mockVic2Country testCountry3;
	EXPECT_CALL(testCountry3, isCivilized).WillRepeatedly(testing::Return(true));
	EXPECT_CALL(testCountry3, getProvinces).WillRepeatedly(testing::ReturnRef(provinces));
	EXPECT_CALL(testCountry3, getSourceCountry).WillRepeatedly(testing::Return(country3));

	std::map<std::string, std::shared_ptr<V2::Country>> countries;
	countries.insert(std::make_pair("TST", &testCountry));
	countries.insert(std::make_pair("MID", &testCountry2));
	countries.insert(std::make_pair("LOW", &testCountry3));

	helpers::TechValues techValues(countries);

	ASSERT_NEAR(techValues.getNormalizedNavyTech(*country2), (-2.0 / 7.0), 0.00001);
}


TEST(Helpers_TechValuesTests, getNormalizedCommerceTechReturnsNegativeOneIfInitializedWithNoCountries)
{
	std::map<std::string, std::shared_ptr<V2::Country>> countries;
	helpers::TechValues techValues(countries);

	mockEU4Country country;
	EXPECT_CALL(country, getDipTech).WillOnce(testing::Return(32));
	EXPECT_CALL(country, getAdmTech).WillOnce(testing::Return(32));

	ASSERT_EQ(techValues.getNormalizedCommerceTech(country), -1.0);
}


TEST(Helpers_TechValuesTests, getNormalizedCommerceTechReturnsOneIfInitializedWithOneCountries)
{
	std::shared_ptr<mockEU4Country> country = std::make_shared<mockEU4Country>();
	EXPECT_CALL(*country, getAdmTech).WillRepeatedly(testing::Return(32));
	EXPECT_CALL(*country, getDipTech).WillRepeatedly(testing::Return(32));
	EXPECT_CALL(*country, getMilTech).WillRepeatedly(testing::Return(32));
	std::set<std::string> nationalIdeas;
	EXPECT_CALL(*country, getNationalIdeas).WillRepeatedly(testing::ReturnRef(nationalIdeas));

	mockVic2Country testCountry;
	EXPECT_CALL(testCountry, isCivilized).WillOnce(testing::Return(true));
	std::map<int, std::shared_ptr<V2::Province>> provinces;
	std::shared_ptr<V2::Province> province;
	provinces.insert(std::make_pair(1, province));
	EXPECT_CALL(testCountry, getProvinces).WillOnce(testing::ReturnRef(provinces));
	EXPECT_CALL(testCountry, getSourceCountry).WillRepeatedly(testing::Return(country));
	std::map<std::string, std::shared_ptr<V2::Country>> countries;
	countries.insert(std::make_pair("TST", &testCountry));

	helpers::TechValues techValues(countries);

	ASSERT_EQ(techValues.getNormalizedCommerceTech(*country), 1.0);
}


TEST(Helpers_TechValuesTests, getNormalizedCommerceTechReturnsOneForHighestScore)
{
	std::shared_ptr<mockEU4Country> country = std::make_shared<mockEU4Country>();
	EXPECT_CALL(*country, getAdmTech).WillRepeatedly(testing::Return(32));
	EXPECT_CALL(*country, getDipTech).WillRepeatedly(testing::Return(32));
	EXPECT_CALL(*country, getMilTech).WillRepeatedly(testing::Return(32));
	std::set<std::string> nationalIdeas;
	EXPECT_CALL(*country, getNationalIdeas).WillRepeatedly(testing::ReturnRef(nationalIdeas));

	mockVic2Country testCountry;
	EXPECT_CALL(testCountry, isCivilized).WillRepeatedly(testing::Return(true));
	std::map<int, std::shared_ptr<V2::Province>> provinces;
	std::shared_ptr<V2::Province> province;
	provinces.insert(std::make_pair(1, province));
	EXPECT_CALL(testCountry, getProvinces).WillRepeatedly(testing::ReturnRef(provinces));
	EXPECT_CALL(testCountry, getSourceCountry).WillRepeatedly(testing::Return(country));

	std::shared_ptr<mockEU4Country> country2 = std::make_shared<mockEU4Country>();
	EXPECT_CALL(*country2, getAdmTech).WillRepeatedly(testing::Return(8));
	EXPECT_CALL(*country2, getDipTech).WillRepeatedly(testing::Return(8));
	EXPECT_CALL(*country2, getMilTech).WillRepeatedly(testing::Return(8));
	EXPECT_CALL(*country2, getNationalIdeas).WillRepeatedly(testing::ReturnRef(nationalIdeas));

	mockVic2Country testCountry2;
	EXPECT_CALL(testCountry2, isCivilized).WillRepeatedly(testing::Return(true));
	EXPECT_CALL(testCountry2, getProvinces).WillRepeatedly(testing::ReturnRef(provinces));
	EXPECT_CALL(testCountry2, getSourceCountry).WillRepeatedly(testing::Return(country2));

	std::shared_ptr<mockEU4Country> country3 = std::make_shared<mockEU4Country>();
	EXPECT_CALL(*country3, getAdmTech).WillRepeatedly(testing::Return(0));
	EXPECT_CALL(*country3, getDipTech).WillRepeatedly(testing::Return(0));
	EXPECT_CALL(*country3, getMilTech).WillRepeatedly(testing::Return(0));
	EXPECT_CALL(*country3, getNationalIdeas).WillRepeatedly(testing::ReturnRef(nationalIdeas));

	mockVic2Country testCountry3;
	EXPECT_CALL(testCountry3, isCivilized).WillRepeatedly(testing::Return(true));
	EXPECT_CALL(testCountry3, getProvinces).WillRepeatedly(testing::ReturnRef(provinces));
	EXPECT_CALL(testCountry3, getSourceCountry).WillRepeatedly(testing::Return(country3));

	std::map<std::string, std::shared_ptr<V2::Country>> countries;
	countries.insert(std::make_pair("TST", &testCountry));
	countries.insert(std::make_pair("MID", &testCountry3));
	countries.insert(std::make_pair("LOW", &testCountry3));

	helpers::TechValues techValues(countries);

	ASSERT_EQ(techValues.getNormalizedCommerceTech(*country), 1.0);
}


TEST(Helpers_TechValuesTests, getNormalizedCommerceTechReturnsScaledScore)
{
	std::shared_ptr<mockEU4Country> country = std::make_shared<mockEU4Country>();
	EXPECT_CALL(*country, getAdmTech).WillRepeatedly(testing::Return(32));
	EXPECT_CALL(*country, getDipTech).WillRepeatedly(testing::Return(32));
	EXPECT_CALL(*country, getMilTech).WillRepeatedly(testing::Return(32));
	std::set<std::string> nationalIdeas;
	EXPECT_CALL(*country, getNationalIdeas).WillRepeatedly(testing::ReturnRef(nationalIdeas));

	mockVic2Country testCountry;
	EXPECT_CALL(testCountry, isCivilized).WillRepeatedly(testing::Return(true));
	std::map<int, std::shared_ptr<V2::Province>> provinces;
	std::shared_ptr<V2::Province> province;
	provinces.insert(std::make_pair(1, province));
	EXPECT_CALL(testCountry, getProvinces).WillRepeatedly(testing::ReturnRef(provinces));
	EXPECT_CALL(testCountry, getSourceCountry).WillRepeatedly(testing::Return(country));

	std::shared_ptr<mockEU4Country> country2 = std::make_shared<mockEU4Country>();
	EXPECT_CALL(*country2, getAdmTech).WillRepeatedly(testing::Return(8));
	EXPECT_CALL(*country2, getDipTech).WillRepeatedly(testing::Return(8));
	EXPECT_CALL(*country2, getMilTech).WillRepeatedly(testing::Return(8));
	EXPECT_CALL(*country2, getNationalIdeas).WillRepeatedly(testing::ReturnRef(nationalIdeas));

	mockVic2Country testCountry2;
	EXPECT_CALL(testCountry2, isCivilized).WillRepeatedly(testing::Return(true));
	EXPECT_CALL(testCountry2, getProvinces).WillRepeatedly(testing::ReturnRef(provinces));
	EXPECT_CALL(testCountry2, getSourceCountry).WillRepeatedly(testing::Return(country2));

	std::shared_ptr<mockEU4Country> country3 = std::make_shared<mockEU4Country>();
	EXPECT_CALL(*country3, getAdmTech).WillRepeatedly(testing::Return(0));
	EXPECT_CALL(*country3, getDipTech).WillRepeatedly(testing::Return(0));
	EXPECT_CALL(*country3, getMilTech).WillRepeatedly(testing::Return(0));
	EXPECT_CALL(*country3, getNationalIdeas).WillRepeatedly(testing::ReturnRef(nationalIdeas));

	mockVic2Country testCountry3;
	EXPECT_CALL(testCountry3, isCivilized).WillRepeatedly(testing::Return(true));
	EXPECT_CALL(testCountry3, getProvinces).WillRepeatedly(testing::ReturnRef(provinces));
	EXPECT_CALL(testCountry3, getSourceCountry).WillRepeatedly(testing::Return(country3));

	std::map<std::string, std::shared_ptr<V2::Country>> countries;
	countries.insert(std::make_pair("TST", &testCountry));
	countries.insert(std::make_pair("MID", &testCountry2));
	countries.insert(std::make_pair("LOW", &testCountry3));

	helpers::TechValues techValues(countries);

	ASSERT_NEAR(techValues.getNormalizedCommerceTech(*country2), (-2.0 / 7.0), 0.00001);
}


TEST(Helpers_TechValuesTests, getNormalizedCultureTechReturnsNegativeOneIfInitializedWithNoCountries)
{
	std::map<std::string, std::shared_ptr<V2::Country>> countries;

	helpers::TechValues techValues(countries);

	mockEU4Country country;
	EXPECT_CALL(country, getDipTech).WillOnce(testing::Return(32));

	ASSERT_EQ(techValues.getNormalizedCultureTech(country), -1.0);
}


TEST(Helpers_TechValuesTests, getNormalizedCultureTechReturnsOneIfInitializedWithOneCountries)
{
	std::shared_ptr<mockEU4Country> country = std::make_shared<mockEU4Country>();
	EXPECT_CALL(*country, getAdmTech).WillRepeatedly(testing::Return(32));
	EXPECT_CALL(*country, getDipTech).WillRepeatedly(testing::Return(32));
	EXPECT_CALL(*country, getMilTech).WillRepeatedly(testing::Return(32));
	std::set<std::string> nationalIdeas;
	EXPECT_CALL(*country, getNationalIdeas).WillRepeatedly(testing::ReturnRef(nationalIdeas));

	mockVic2Country testCountry;
	EXPECT_CALL(testCountry, isCivilized).WillOnce(testing::Return(true));
	std::map<int, std::shared_ptr<V2::Province>> provinces;
	std::shared_ptr<V2::Province> province;
	provinces.insert(std::make_pair(1, province));
	EXPECT_CALL(testCountry, getProvinces).WillOnce(testing::ReturnRef(provinces));
	EXPECT_CALL(testCountry, getSourceCountry).WillRepeatedly(testing::Return(country));
	std::map<std::string, std::shared_ptr<V2::Country>> countries;
	countries.insert(std::make_pair("TST", &testCountry));

	helpers::TechValues techValues(countries);

	ASSERT_EQ(techValues.getNormalizedCultureTech(*country), 1.0);
}


TEST(Helpers_TechValuesTests, getNormalizedCultureTechReturnsOneForHighestScore)
{
	std::shared_ptr<mockEU4Country> country = std::make_shared<mockEU4Country>();
	EXPECT_CALL(*country, getAdmTech).WillRepeatedly(testing::Return(32));
	EXPECT_CALL(*country, getDipTech).WillRepeatedly(testing::Return(32));
	EXPECT_CALL(*country, getMilTech).WillRepeatedly(testing::Return(32));
	std::set<std::string> nationalIdeas;
	EXPECT_CALL(*country, getNationalIdeas).WillRepeatedly(testing::ReturnRef(nationalIdeas));

	mockVic2Country testCountry;
	EXPECT_CALL(testCountry, isCivilized).WillRepeatedly(testing::Return(true));
	std::map<int, std::shared_ptr<V2::Province>> provinces;
	std::shared_ptr<V2::Province> province;
	provinces.insert(std::make_pair(1, province));
	EXPECT_CALL(testCountry, getProvinces).WillRepeatedly(testing::ReturnRef(provinces));
	EXPECT_CALL(testCountry, getSourceCountry).WillRepeatedly(testing::Return(country));

	std::shared_ptr<mockEU4Country> country2 = std::make_shared<mockEU4Country>();
	EXPECT_CALL(*country2, getAdmTech).WillRepeatedly(testing::Return(8));
	EXPECT_CALL(*country2, getDipTech).WillRepeatedly(testing::Return(8));
	EXPECT_CALL(*country2, getMilTech).WillRepeatedly(testing::Return(8));
	EXPECT_CALL(*country2, getNationalIdeas).WillRepeatedly(testing::ReturnRef(nationalIdeas));

	mockVic2Country testCountry2;
	EXPECT_CALL(testCountry2, isCivilized).WillRepeatedly(testing::Return(true));
	EXPECT_CALL(testCountry2, getProvinces).WillRepeatedly(testing::ReturnRef(provinces));
	EXPECT_CALL(testCountry2, getSourceCountry).WillRepeatedly(testing::Return(country2));

	std::shared_ptr<mockEU4Country> country3 = std::make_shared<mockEU4Country>();
	EXPECT_CALL(*country3, getAdmTech).WillRepeatedly(testing::Return(0));
	EXPECT_CALL(*country3, getDipTech).WillRepeatedly(testing::Return(0));
	EXPECT_CALL(*country3, getMilTech).WillRepeatedly(testing::Return(0));
	EXPECT_CALL(*country3, getNationalIdeas).WillRepeatedly(testing::ReturnRef(nationalIdeas));

	mockVic2Country testCountry3;
	EXPECT_CALL(testCountry3, isCivilized).WillRepeatedly(testing::Return(true));
	EXPECT_CALL(testCountry3, getProvinces).WillRepeatedly(testing::ReturnRef(provinces));
	EXPECT_CALL(testCountry3, getSourceCountry).WillRepeatedly(testing::Return(country3));

	std::map<std::string, std::shared_ptr<V2::Country>> countries;
	countries.insert(std::make_pair("TST", &testCountry));
	countries.insert(std::make_pair("MID", &testCountry3));
	countries.insert(std::make_pair("LOW", &testCountry3));

	helpers::TechValues techValues(countries);

	ASSERT_EQ(techValues.getNormalizedCultureTech(*country), 1.0);
}


TEST(Helpers_TechValuesTests, getNormalizedCultureTechReturnsScaledScore)
{
	std::shared_ptr<mockEU4Country> country = std::make_shared<mockEU4Country>();
	EXPECT_CALL(*country, getAdmTech).WillRepeatedly(testing::Return(32));
	EXPECT_CALL(*country, getDipTech).WillRepeatedly(testing::Return(32));
	EXPECT_CALL(*country, getMilTech).WillRepeatedly(testing::Return(32));
	std::set<std::string> nationalIdeas;
	EXPECT_CALL(*country, getNationalIdeas).WillRepeatedly(testing::ReturnRef(nationalIdeas));

	mockVic2Country testCountry;
	EXPECT_CALL(testCountry, isCivilized).WillRepeatedly(testing::Return(true));
	std::map<int, std::shared_ptr<V2::Province>> provinces;
	std::shared_ptr<V2::Province> province;
	provinces.insert(std::make_pair(1, province));
	EXPECT_CALL(testCountry, getProvinces).WillRepeatedly(testing::ReturnRef(provinces));
	EXPECT_CALL(testCountry, getSourceCountry).WillRepeatedly(testing::Return(country));

	std::shared_ptr<mockEU4Country> country2 = std::make_shared<mockEU4Country>();
	EXPECT_CALL(*country2, getAdmTech).WillRepeatedly(testing::Return(8));
	EXPECT_CALL(*country2, getDipTech).WillRepeatedly(testing::Return(8));
	EXPECT_CALL(*country2, getMilTech).WillRepeatedly(testing::Return(8));
	EXPECT_CALL(*country2, getNationalIdeas).WillRepeatedly(testing::ReturnRef(nationalIdeas));

	mockVic2Country testCountry2;
	EXPECT_CALL(testCountry2, isCivilized).WillRepeatedly(testing::Return(true));
	EXPECT_CALL(testCountry2, getProvinces).WillRepeatedly(testing::ReturnRef(provinces));
	EXPECT_CALL(testCountry2, getSourceCountry).WillRepeatedly(testing::Return(country2));

	std::shared_ptr<mockEU4Country> country3 = std::make_shared<mockEU4Country>();
	EXPECT_CALL(*country3, getAdmTech).WillRepeatedly(testing::Return(0));
	EXPECT_CALL(*country3, getDipTech).WillRepeatedly(testing::Return(0));
	EXPECT_CALL(*country3, getMilTech).WillRepeatedly(testing::Return(0));
	EXPECT_CALL(*country3, getNationalIdeas).WillRepeatedly(testing::ReturnRef(nationalIdeas));

	mockVic2Country testCountry3;
	EXPECT_CALL(testCountry3, isCivilized).WillRepeatedly(testing::Return(true));
	EXPECT_CALL(testCountry3, getProvinces).WillRepeatedly(testing::ReturnRef(provinces));
	EXPECT_CALL(testCountry3, getSourceCountry).WillRepeatedly(testing::Return(country3));

	std::map<std::string, std::shared_ptr<V2::Country>> countries;
	countries.insert(std::make_pair("TST", &testCountry));
	countries.insert(std::make_pair("MID", &testCountry2));
	countries.insert(std::make_pair("LOW", &testCountry3));

	helpers::TechValues techValues(countries);

	ASSERT_NEAR(techValues.getNormalizedCultureTech(*country2), (-2.0 / 7.0), 0.00001);
}


TEST(Helpers_TechValuesTests, getNormalizedIndustryTechReturnsNegativeOneIfInitializedWithNoCountries)
{
	std::map<std::string, std::shared_ptr<V2::Country>> countries;

	helpers::TechValues techValues(countries);

	mockEU4Country country;
	EXPECT_CALL(country, getAdmTech).WillOnce(testing::Return(32));
	EXPECT_CALL(country, getDipTech).WillOnce(testing::Return(32));
	EXPECT_CALL(country, getMilTech).WillOnce(testing::Return(32));

	ASSERT_EQ(techValues.getNormalizedIndustryTech(country), -1.0);
}


TEST(Helpers_TechValuesTests, getNormalizedIndustryTechReturnsOneIfInitializedWithOneCountries)
{
	std::shared_ptr<mockEU4Country> country = std::make_shared<mockEU4Country>();
	EXPECT_CALL(*country, getAdmTech).WillRepeatedly(testing::Return(32));
	EXPECT_CALL(*country, getDipTech).WillRepeatedly(testing::Return(32));
	EXPECT_CALL(*country, getMilTech).WillRepeatedly(testing::Return(32));
	std::set<std::string> nationalIdeas;
	EXPECT_CALL(*country, getNationalIdeas).WillRepeatedly(testing::ReturnRef(nationalIdeas));

	mockVic2Country testCountry;
	EXPECT_CALL(testCountry, isCivilized).WillOnce(testing::Return(true));
	std::map<int, std::shared_ptr<V2::Province>> provinces;
	std::shared_ptr<V2::Province> province;
	provinces.insert(std::make_pair(1, province));
	EXPECT_CALL(testCountry, getProvinces).WillOnce(testing::ReturnRef(provinces));
	EXPECT_CALL(testCountry, getSourceCountry).WillRepeatedly(testing::Return(country));
	std::map<std::string, std::shared_ptr<V2::Country>> countries;
	countries.insert(std::make_pair("TST", &testCountry));

	helpers::TechValues techValues(countries);

	ASSERT_EQ(techValues.getNormalizedIndustryTech(*country), 1.0);
}


TEST(Helpers_TechValuesTests, getNormalizedIndustryTechReturnsOneForHighestScore)
{
	std::shared_ptr<mockEU4Country> country = std::make_shared<mockEU4Country>();
	EXPECT_CALL(*country, getAdmTech).WillRepeatedly(testing::Return(32));
	EXPECT_CALL(*country, getDipTech).WillRepeatedly(testing::Return(32));
	EXPECT_CALL(*country, getMilTech).WillRepeatedly(testing::Return(32));
	std::set<std::string> nationalIdeas;
	EXPECT_CALL(*country, getNationalIdeas).WillRepeatedly(testing::ReturnRef(nationalIdeas));

	mockVic2Country testCountry;
	EXPECT_CALL(testCountry, isCivilized).WillRepeatedly(testing::Return(true));
	std::map<int, std::shared_ptr<V2::Province>> provinces;
	std::shared_ptr<V2::Province> province;
	provinces.insert(std::make_pair(1, province));
	EXPECT_CALL(testCountry, getProvinces).WillRepeatedly(testing::ReturnRef(provinces));
	EXPECT_CALL(testCountry, getSourceCountry).WillRepeatedly(testing::Return(country));

	std::shared_ptr<mockEU4Country> country2 = std::make_shared<mockEU4Country>();
	EXPECT_CALL(*country2, getAdmTech).WillRepeatedly(testing::Return(8));
	EXPECT_CALL(*country2, getDipTech).WillRepeatedly(testing::Return(8));
	EXPECT_CALL(*country2, getMilTech).WillRepeatedly(testing::Return(8));
	EXPECT_CALL(*country2, getNationalIdeas).WillRepeatedly(testing::ReturnRef(nationalIdeas));

	mockVic2Country testCountry2;
	EXPECT_CALL(testCountry2, isCivilized).WillRepeatedly(testing::Return(true));
	EXPECT_CALL(testCountry2, getProvinces).WillRepeatedly(testing::ReturnRef(provinces));
	EXPECT_CALL(testCountry2, getSourceCountry).WillRepeatedly(testing::Return(country2));

	std::shared_ptr<mockEU4Country> country3 = std::make_shared<mockEU4Country>();
	EXPECT_CALL(*country3, getAdmTech).WillRepeatedly(testing::Return(0));
	EXPECT_CALL(*country3, getDipTech).WillRepeatedly(testing::Return(0));
	EXPECT_CALL(*country3, getMilTech).WillRepeatedly(testing::Return(0));
	EXPECT_CALL(*country3, getNationalIdeas).WillRepeatedly(testing::ReturnRef(nationalIdeas));

	mockVic2Country testCountry3;
	EXPECT_CALL(testCountry3, isCivilized).WillRepeatedly(testing::Return(true));
	EXPECT_CALL(testCountry3, getProvinces).WillRepeatedly(testing::ReturnRef(provinces));
	EXPECT_CALL(testCountry3, getSourceCountry).WillRepeatedly(testing::Return(country3));

	std::map<std::string, std::shared_ptr<V2::Country>> countries;
	countries.insert(std::make_pair("TST", &testCountry));
	countries.insert(std::make_pair("MID", &testCountry2));
	countries.insert(std::make_pair("LOW", &testCountry3));

	helpers::TechValues techValues(countries);

	ASSERT_EQ(techValues.getNormalizedIndustryTech(*country), 1.0);
}


TEST(Helpers_TechValuesTests, getNormalizedIndustryTechReturnsScaledScore)
{
	std::shared_ptr<mockEU4Country> country = std::make_shared<mockEU4Country>();
	EXPECT_CALL(*country, getAdmTech).WillRepeatedly(testing::Return(32));
	EXPECT_CALL(*country, getDipTech).WillRepeatedly(testing::Return(32));
	EXPECT_CALL(*country, getMilTech).WillRepeatedly(testing::Return(32));
	std::set<std::string> nationalIdeas;
	EXPECT_CALL(*country, getNationalIdeas).WillRepeatedly(testing::ReturnRef(nationalIdeas));

	mockVic2Country testCountry;
	EXPECT_CALL(testCountry, isCivilized).WillRepeatedly(testing::Return(true));
	std::map<int, std::shared_ptr<V2::Province>> provinces;
	std::shared_ptr<V2::Province> province;
	provinces.insert(std::make_pair(1, province));
	EXPECT_CALL(testCountry, getProvinces).WillRepeatedly(testing::ReturnRef(provinces));
	EXPECT_CALL(testCountry, getSourceCountry).WillRepeatedly(testing::Return(country));

	std::shared_ptr<mockEU4Country> country2 = std::make_shared<mockEU4Country>();
	EXPECT_CALL(*country2, getAdmTech).WillRepeatedly(testing::Return(8));
	EXPECT_CALL(*country2, getDipTech).WillRepeatedly(testing::Return(8));
	EXPECT_CALL(*country2, getMilTech).WillRepeatedly(testing::Return(8));
	EXPECT_CALL(*country2, getNationalIdeas).WillRepeatedly(testing::ReturnRef(nationalIdeas));

	mockVic2Country testCountry2;
	EXPECT_CALL(testCountry2, isCivilized).WillRepeatedly(testing::Return(true));
	EXPECT_CALL(testCountry2, getProvinces).WillRepeatedly(testing::ReturnRef(provinces));
	EXPECT_CALL(testCountry2, getSourceCountry).WillRepeatedly(testing::Return(country2));

	std::shared_ptr<mockEU4Country> country3 = std::make_shared<mockEU4Country>();
	EXPECT_CALL(*country3, getAdmTech).WillRepeatedly(testing::Return(0));
	EXPECT_CALL(*country3, getDipTech).WillRepeatedly(testing::Return(0));
	EXPECT_CALL(*country3, getMilTech).WillRepeatedly(testing::Return(0));
	EXPECT_CALL(*country3, getNationalIdeas).WillRepeatedly(testing::ReturnRef(nationalIdeas));

	mockVic2Country testCountry3;
	EXPECT_CALL(testCountry3, isCivilized).WillRepeatedly(testing::Return(true));
	EXPECT_CALL(testCountry3, getProvinces).WillRepeatedly(testing::ReturnRef(provinces));
	EXPECT_CALL(testCountry3, getSourceCountry).WillRepeatedly(testing::Return(country3));

	std::map<std::string, std::shared_ptr<V2::Country>> countries;
	countries.insert(std::make_pair("TST", &testCountry));
	countries.insert(std::make_pair("MID", &testCountry2));
	countries.insert(std::make_pair("LOW", &testCountry3));

	helpers::TechValues techValues(countries);

	ASSERT_NEAR(techValues.getNormalizedIndustryTech(*country2), (-2.0 / 7.0), 0.00001);
}