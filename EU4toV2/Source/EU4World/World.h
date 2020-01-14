#ifndef EU4_WORLD_H
#define EU4_WORLD_H

#include "Army/EU4Army.h"
#include "Diplomacy/EU4Diplomacy.h"
#include "EU4Version.h"
#include "Provinces/Provinces.h"
#include "Regions/Regions.h"
#include "Religions/Religions.h"
#include "Modifiers/Modifiers.h"
#include "Country/EU4Country.h"
#include "../Mappers/CultureMapper/CultureMapper.h"
#include "../Mappers/UnitTypes/UnitTypeMapper.h"
#include "../Mappers/ProvinceMappings/ProvinceMapper.h"
#include "../Mappers/ReligionMapper/ReligionMapper.h"
#include "../Mappers/Buildings/Buildings.h"
#include "../Mappers/CultureGroups/CultureGroups.h"
#include "../Mappers/Ideas/IdeaEffectMapper.h"
#include "newParser.h"
#include <memory>

namespace EU4
{
	class World: private commonItems::parser
	{
	public:
		World(const std::string& EU4SaveFileName, const mappers::IdeaEffectMapper& ideaEffectMapper);
		std::map<std::string, std::shared_ptr<EU4::Country>> getCountries() const { return theCountries; };
		std::vector<EU4Agreement> getDiplomaticAgreements() const { return diplomacy; };
		double getTotalProvinceWeights() const { return provinces->geTotalProvinceWeights(); };
		const Regions& getRegions() const { return *regions; }
		const Religions& getAllReligions() const { return theReligions; }
		const EU4::Version& getVersion() const { return *version; };
		const Province& getProvince(int provNum) const;
		const mappers::CultureGroups& getCultureGroupsMapper() const { return cultureGroupsMapper; }
		void checkAllEU4CulturesMapped(const mappers::CultureMapper& cultureMapper) const;
		void checkAllEU4ReligionsMapped(const mappers::ReligionMapper& religionMapper) const;
		void checkAllProvincesMapped(const mappers::ProvinceMapper& provinceMapper) const;
		bool isRandomWorld() const;

	private:
		void verifySave(const std::string& EU4SaveFileName);
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
		std::map<std::string, mappers::UnitType> unitTypeMap;

		Religions theReligions;
		Modifiers modifierTypes;
		mappers::UnitTypeMapper unitTypeMapper;
		mappers::Buildings buildingTypes;
		mappers::CultureGroups cultureGroupsMapper;

	};
}

#endif // EU4_WORLD_H
