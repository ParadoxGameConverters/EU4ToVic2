#ifndef FLAGS_H
#define FLAGS_H

#include <map>
#include <set>
#include <string>
#include <vector>
#include "../../Mappers/ColonialFlags/ColonialFlagsMapper.h"
#include "../../EU4World/Country/EU4NationalSymbol.h"
#include "../../Mappers/CountryMappings/CountryMappings.h"
#include "../../Mappers/FlagColors/FlagColorMapper.h"

namespace V2
{
	class V2Country;
	class Flags 
	{
	public:
		void setV2Tags(const std::map<std::string, std::shared_ptr<Country>>& V2Countries, const mappers::CountryMappings& countryMapper);
		void output() const;

	private:
		void determineUseableFlags();
		std::set<std::string> determineAvailableFlags();
		void getRequiredTags(const std::map<std::string, std::shared_ptr<Country>>& V2Countries);
		void mapTrivialTags();

		static void createOutputFolders();
		void copyFlags() const;
		void createCustomFlags() const;
		void createColonialFlags() const;

		std::set<std::string> usableFlagTags;
		std::set<std::string> requiredTags;

		static const std::vector<std::string> flagFileSuffixes;

		std::map<std::string, std::string> tagMap;
		std::map<std::string, mappers::ColonialFlag> colonialFlagMapping; // tag, {base,overlordtag}
		std::map<std::string, EU4::CustomColors> customFlagMapping;

		mappers::ColonialFlagsMapper colonialFlagsMapper;
		mappers::FlagColorMapper flagColorMapper;
	};
}
#endif // FLAGS_H