#ifndef COUNTRY_POP_LOGGER_H
#define COUNTRY_POP_LOGGER_H

#include <string>
#include <map>
#include "../Province/Province.h"

namespace mappers {
	class PopTypes;
}

namespace V2
{
	class Pop;

	class CountryPopLogger
	{
	public:
		void logPopsByCountry(const std::map<int, std::shared_ptr<Province>>& provinces);

	private:
		std::map<std::string, std::map<std::string, long int>> popsByCountry;
		long totalPopulation = 0;
		
		void logPopsFromFile(const std::map<int, std::shared_ptr<Province>>& provinces, const std::string& filename);
		void logPopsInProvince(const std::map<int, std::shared_ptr<Province>>& provinces, int provinceID, const mappers::PopTypes& popTypes);
		std::map<std::string, std::map<std::string, long int>>::iterator getCountryForPopLogging(const std::string&);
		void logPop(const std::string& popType, const Pop& pop, std::map<std::string, std::map<std::string, long int>>::iterator countryPopItr) const;
		void outputLog(const std::map<std::string, std::map<std::string, long int>>& popsByCountry) const;
	};
}
#endif // COUNTRY_POP_LOGGER_H
