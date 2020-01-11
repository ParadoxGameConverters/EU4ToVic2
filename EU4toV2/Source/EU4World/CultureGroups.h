#ifndef CULTURE_GROUPS_H
#define CULTURE_GROUPS_H

#include "newParser.h"
#include <map>
#include <optional>
#include <string>
#include <vector>

namespace EU4
{
	class culture: commonItems::parser
	{
		public:
			culture(std::istream& theStream);

		private:
			std::string primaryTag;
			std::string graphicalCulture;
			std::vector<std::string> maleNames;
			std::vector<std::string> femaleNames;
			std::vector<std::string> dynastyNames;
	};


	class cultureGroup: commonItems::parser
	{
		public:
			cultureGroup(const std::string& name_, std::istream& theStream);

			std::string getName() const { return name; }
			std::map<std::string, culture> getCultures() const { return cultures; }

		private:
			std::string name;
			std::string graphicalCulture;
			std::vector<std::string> maleNames;
			std::vector<std::string> femaleNames;
			std::vector<std::string> dynastyNames;
			std::map<std::string, culture> cultures;
	};


	class cultureGroups: commonItems::parser
	{
		public:
			static std::optional<cultureGroup> getCulturalGroup(const std::string& culture)
			{
				return getInstance()->GetCulturalGroup(culture);
			}

			static std::map<std::string, cultureGroup> getCultureToGroupMap()
			{
				return getInstance()->cultureToGroupMap;
			}

			static std::vector<culture> getCulturesInGroup(const std::string& group)
			{
				return getInstance()->GetCulturesInGroup(group);
			}

		private:
			static cultureGroups* instance;
			static cultureGroups* getInstance()
			{
				if (instance == nullptr)
				{
					instance = new cultureGroups;
				}
				return instance;
			}

			cultureGroups();
			void addCulturesFromFile(const std::string& filename);

			std::optional<cultureGroup> GetCulturalGroup(const std::string& culture);
			std::vector<culture> GetCulturesInGroup(const std::string& group);

			std::map<std::string, std::vector<culture>> groupToCulturesMap;
			std::map<std::string, cultureGroup> cultureToGroupMap;
	};
}

#endif // CULTURE_GROUPS_H
