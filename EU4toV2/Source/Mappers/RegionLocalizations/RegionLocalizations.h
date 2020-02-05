#ifndef REGION_LOCALIZATIONS_H
#define REGION_LOCALIZATIONS_H

#include <map>
#include <string>

namespace mappers
{
	class RegionLocalizations
	{
	public:
		RegionLocalizations();

	private:
		std::map<std::string, std::string> spaLocalisations; // region, adj_loc
		std::map<std::string, std::string> engLocalisations;
		std::map<std::string, std::string> gerLocalisations;
		std::map<std::string, std::string> fraLocalisations;

		std::map<std::string, std::string> chopFile(const std::string& locFile) const;
	};
}

#endif // REGION_LOCALIZATIONS_H