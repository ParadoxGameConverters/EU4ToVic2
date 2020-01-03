#ifndef V2_FACTORY_TECH_LOADER_H_
#define V2_FACTORY_TECH_LOADER_H_

#include <map>
#include <string>


namespace mappers
{
	class V2FactoryTechLoader
	{
	public:
		V2FactoryTechLoader();
		std::map<std::string, std::string> getFactoryTechMap() const { return factoryTechMap; }

	private:
		std::map<std::string, std::string> factoryTechMap;
		void ParseV2TechFile(const std::string& filename);
	};
}

#endif // V2_FACTORY_TECH_LOADER_H_