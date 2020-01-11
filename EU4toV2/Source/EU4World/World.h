#ifndef EU4_WORLD_H
#define EU4_WORLD_H

#include "Army/EU4Army.h"
#include "Diplomacy/EU4Diplomacy.h"
#include "EU4Version.h"
#include "Provinces/Provinces.h"
#include "Regions/Regions.h"
#include "Religions/Religions.h"
#include "Modifiers/Modifiers.h"
#include "../Mappers/CultureMapper.h"
#include "../Mappers/UnitTypeMapper.h"
#include "../Mappers/ProvinceMappings/ProvinceMapper.h"
#include "../Mappers/ReligionMapper.h"
#include "../Mappers/Buildings/Buildings.h"
#include "../Mappers/CultureGroups/CultureGroups.h"
#include "newParser.h"
#include <istream>
#include <memory>

namespace mappers
{
class IdeaEffectMapper;
}

namespace EU4
{

class Country;
class Province;


	class world: private commonItems::parser
	{
	public:
		world(const std::string& EU4SaveFileName, const mappers::IdeaEffectMapper& ideaEffectMapper);

		const Province& getProvince(int provNum) const;

		void checkAllEU4CulturesMapped(const mappers::CultureMapper& cultureMapper) const;
		void checkAllEU4ReligionsMapped(const mappers::ReligionMapper& religionMapper) const;
		void checkAllProvincesMapped(const mappers::ProvinceMapper& provinceMapper) const;

		const EU4::Version& getVersion() const { return *version; };
		std::map<std::string, std::shared_ptr<EU4::Country>> getCountries() const { return theCountries; };
		std::vector<EU4Agreement> getDiplomaticAgreements() const { return diplomacy; };
		double getTotalProvinceWeights() const { return provinces->geTotalProvinceWeights(); };
		const Regions& getRegions() const { return *regions; }
		const Religions& getAllReligions() const { return theReligions; }

		bool isRandomWorld() const;
		const mappers::CultureGroups& getCultureGroupsMapper() const { return cultureGroupsMapper; }

	private:
		void verifySave(const std::string& EU4SaveFileName);

		void loadEU4Version(const std::shared_ptr<Object> EU4SaveObj);

		void loadRevolutionTarget();
		void dropMinoritiesFromCountries();
		void addProvinceInfoToCountries();

		void loadRegions();
		void loadEU4RegionsNewVersion();
		void loadEU4RegionsOldVersion();

		void readCommonCountries();
		void readCommonCountriesFile(std::istream&, const std::string& rootPath);

		void setLocalisations();

		void resolveRegimentTypes();

		void mergeNations();
		void uniteJapan();

		void importReligions();

		void removeEmptyNations();
		void removeDeadLandlessNations();
		void removeLandlessNations();

		void setEmpires();
	   void assignProvincesToAreas(const std::map<std::string, std::set<int>>& theAreas);

	   std::shared_ptr<EU4::Country> getCountry(std::string tag) const;

		std::string holyRomanEmperor;
		std::string celestialEmperor;
		std::unique_ptr<Regions> regions;
		std::unique_ptr<Provinces> provinces;
		std::map<std::string, std::shared_ptr<EU4::Country>> theCountries;
		std::vector<EU4Agreement> diplomacy;
		std::unique_ptr<EU4::Version> version;

		std::string revolutionTargetString;

		Religions theReligions;
		mappers::Buildings buildingTypes;
		mappers::CultureGroups cultureGroupsMapper;
		Modifiers modifierTypes;

		std::map<std::string, mappers::UnitType> unitTypeMap;
	};

}



#endif // EU4_WORLD_H
