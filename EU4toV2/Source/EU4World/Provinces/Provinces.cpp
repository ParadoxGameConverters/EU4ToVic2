#include "Log.h"
#include "Provinces.h"
#include "ParserHelpers.h"
#include <fstream>

EU4::Provinces::Provinces(std::istream& theStream, const mappers::Buildings& buildingTypes, const Modifiers& modifierTypes) 
{
	registerRegex("-[0-9]+", [this, buildingTypes, modifierTypes](const std::string& numberString, std::istream& theStream)
	{
		Province newProvince(numberString, theStream, buildingTypes, modifierTypes);
		provinces.insert(std::make_pair(newProvince.getNum(), std::move(newProvince)));
	});
	registerRegex("[a-zA-Z0-9_\\.:]+", commonItems::ignoreItem);

	parseStream(theStream);
}

EU4::Province& EU4::Provinces::getProvince(int provinceNumber)
{
	std::map<int, Province>::iterator province = provinces.find(provinceNumber);
	if (province == provinces.end())
	{
		std::range_error exception(std::string("EU4 province ") + std::to_string(provinceNumber) + std::string(" does not exist (bad mapping?)"));
		throw exception;
	}
	else
	{
		return province->second;
	}
}

void EU4::Provinces::checkAllProvincesMapped(const mappers::ProvinceMapper& provinceMapper) const
{
	for (auto& province: provinces)
	{
		auto Vic2Provinces = provinceMapper.getVic2ProvinceNumbers(province.first);
		if ((Vic2Provinces.size() == 0) && (provinceMapper.isValidProvince(province.first)))
		{
			LOG(LogLevel::Warning) << "No mapping for province " << province.first << ", check province_mappings.txt!";
		}
	}
}

void EU4::Provinces::determineTotalProvinceWeights(const Configuration& configuration)
{
	for (const auto& province: provinces) totalProvinceWeights += province.second.getTotalWeight();
	if (configuration.getDebug()) logTotalProvinceWeights();
}

void EU4::Provinces::logTotalProvinceWeights() const
{
	std::ofstream EU4_Production("EU4_Production.csv");
	std::ofstream EU4_Tax("EU4_TaxIncome.csv");
	std::ofstream EU4_World("EU4_World.csv");

	std::map<std::string, std::vector<double> > world_tag_weights;

	// Heading
	EU4_Production << "PROV NAME" << ",";
	EU4_Production << "OWNER" << ",";
	EU4_Production << "TRADE GOOD" << ",";
	EU4_Production << "GOODS PROD" << ",";
	EU4_Production << "PRICE" << ",";
	EU4_Production << "TRADE EFF" << ",";
	EU4_Production << "PROD EFF" << ",";
	EU4_Production << "PROV TRADE VAL" << ",";
	EU4_Production << "TOTAL TRADE VAL" << ",";
	EU4_Production << "TOTAL PRODUCTION\n";

	// Heading
	EU4_World << "COUNTRY" << ",";
	EU4_World << "BASE TAX (2x)" << ",";
	EU4_World << "TAX INCOME" << ",";
	EU4_World << "PRODUCTION" << ",";
	EU4_World << "BUILDINGS" << ",";
	EU4_World << "MANPOWER" << ",";
	EU4_World << "SUBTOTAL SAN BUILD" << ",";
	EU4_World << "TOTAL WEIGHT\n";

	// Heading
	EU4_Tax << "PROV NAME" << ",";
	EU4_Tax << "OWNER" << ",";
	EU4_Tax << "BASE TAX" << ",";
	EU4_Tax << "BUILD INCOME" << ",";
	EU4_Tax << "TAX EFF" << ",";
	EU4_Tax << "TOTAL TAX INCOME\n";

	for (auto& province: provinces)
	{
		ProvinceStats provinceStats = province.second.getProvinceStats();
		EU4_Production << province.second.getName() << ",";
		EU4_Production << province.second.getOwnerString() << ",";
		EU4_Production << province.second.getTradeGoods() << ",";
		EU4_Production << provinceStats.getGoodsProduced() << ",";
		EU4_Production << provinceStats.getPrice() << ",";
		EU4_Production << provinceStats.getTradeEfficiency() << ",";
		EU4_Production << provinceStats.getProductionEfficiency() << ",";
		EU4_Production << provinceStats.getTradeValue() << ",";
		EU4_Production << provinceStats.getTotalTradeValue() << ",";
		EU4_Production << province.second.getProductionIncome() << ",\n";

		EU4_Tax << province.second.getName() << ",";
		EU4_Tax << province.second.getOwnerString() << ",";
		EU4_Tax << provinceStats.getBaseTax() << ",";
		EU4_Tax << provinceStats.getBuildingsIncome() << ",";
		EU4_Tax << provinceStats.getTaxEfficiency() << ",";
		EU4_Tax << provinceStats.getTotalTaxIncome() << ",\n";

		std::vector<double> map_values;
		// Total Base Tax, Total Tax Income, Total Production, Total Buildings, Total Manpower, total province weight //
		map_values.push_back((2 * province.second.getBaseTax()));
		map_values.push_back(province.second.getTaxIncome());
		map_values.push_back(province.second.getProductionIncome());
		map_values.push_back(province.second.getTotalBuildingWeight());
		map_values.push_back(province.second.getManpowerWeight());
		map_values.push_back(province.second.getTotalWeight());

		if (world_tag_weights.count(province.second.getOwnerString()))
		{
			std::vector<double> new_map_values;
			new_map_values = world_tag_weights[province.second.getOwnerString()];
			new_map_values[0] += map_values[0];
			new_map_values[1] += map_values[1];
			new_map_values[2] += map_values[2];
			new_map_values[3] += map_values[3];
			new_map_values[4] += map_values[4];
			new_map_values[5] += map_values[5];

			world_tag_weights[province.second.getOwnerString()] = new_map_values;

		}
		else
		{
			world_tag_weights.insert(std::pair<std::string, std::vector<double> >(province.second.getOwnerString(), map_values));
		}

	}
	LOG(LogLevel::Info) << "Sum of all Province Weights: " << totalProvinceWeights;

	// Total Base Tax, Total Tax Income, Total Production, Total Buildings, Total Manpower, total province weight //
	LOG(LogLevel::Info) << "World Tag Map Size: " << world_tag_weights.size();

	for (std::map<std::string, std::vector<double> >::iterator i = world_tag_weights.begin(); i != world_tag_weights.end(); i++)
	{
		EU4_World << i->first << ",";
		EU4_World << i->second[0] << ",";
		EU4_World << i->second[1] << ",";
		EU4_World << i->second[2] << ",";
		EU4_World << i->second[3] << ",";
		EU4_World << i->second[4] << ",";
		EU4_World << (i->second[5] - i->second[3]) << ",";
		EU4_World << i->second[5] << "\n";
	}

	EU4_Production.close();
	EU4_Tax.close();
	EU4_World.close();
}