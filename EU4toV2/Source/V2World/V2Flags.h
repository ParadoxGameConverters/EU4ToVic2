#ifndef V2FLAGS_H
#define V2FLAGS_H

#include <map>
#include <set>
#include <string>
#include <vector>
#include "../Mappers/ColonyFlagsetMapper.h"
#include "../EU4World/Country/EU4NationalSymbol.h"
#include "../Mappers/CountryMappings/CountryMappings.h"


class V2Country;

class V2Flags 
{
	public:
		void SetV2Tags(const std::map<std::string, V2Country*>& V2Countries, const mappers::CountryMappings& countryMapper);
		void output() const;

	private:
		void determineUseableFlags();
		std::set<std::string> determineAvailableFlags();
		void getRequiredTags(const std::map<std::string, V2Country*>& V2Countries);
		void mapTrivialTags();

		void createOutputFolders() const;
		void copyFlags() const;
		void createCustomFlags() const;
		void createColonialFlags() const;

		std::set<std::string> usableFlagTags;
		std::set<std::string> requiredTags;

		static const std::vector<std::string> flagFileSuffixes;

		typedef std::map<std::string, std::string> V2TagToFlagTagMap;
		typedef std::map<std::string, std::shared_ptr<mappers::colonyFlag> > V2TagToColonyFlagMap; // tag, {base,overlordtag}
		typedef std::map<std::string, EU4::CustomColors> V2TagToCustomFlagMap;

		V2TagToFlagTagMap tagMap;
		V2TagToColonyFlagMap colonialFlagMapping;
		V2TagToCustomFlagMap customFlagMapping;
};



#endif