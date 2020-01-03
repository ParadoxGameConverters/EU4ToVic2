#ifndef V2_FACTORY_INVENTION_LOADER_H_
#define V2_FACTORY_INVENTION_LOADER_H_

#include <map>
#include <string>


namespace mappers
{
	class V2FactoryInventionLoader
	{
	public:
		V2FactoryInventionLoader();
		std::map<std::string, std::string> getFactoryInventionMap() const { return factoryInventionMap; }

	private:
		std::map<std::string, std::string> factoryInventionMap;
		void ParseV2InventionFile(const std::string& filename);

	};
}

#endif // V2_FACTORY_INVENTION_LOADER_H_