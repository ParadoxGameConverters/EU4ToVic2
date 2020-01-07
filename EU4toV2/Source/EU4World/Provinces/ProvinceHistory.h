#ifndef PROVINCE_HISTORY_H
#define PROVINCE_HISTORY_H

#include "Date.h"
#include "PopRatio.h"
#include "../Religions/Religions.h"
#include "newParser.h"
#include <optional>
#include <vector>

namespace EU4
{
	class ProvinceHistory: commonItems::parser
	{
		public:
			ProvinceHistory() = default;
			ProvinceHistory(std::istream& theStream);

			std::optional<date> getFirstOwnedDate() const;
			bool hasOriginalCulture() const;
			bool wasColonized() const;
			double getOriginalDevelopment() const { return originalTax + originalProduction + originalManpower; }
			const std::vector<PopRatio>& getPopRatios() const { return popRatios; }

		private:
			void buildPopRatios();
			void decayPopRatios(const date& oldDate, const date& newDate, EU4::PopRatio& currentPop);

			std::vector<std::pair<date, std::string>> ownershipHistory;
			std::vector<std::pair<date, std::string>> religionHistory;
			std::vector<std::pair<date, std::string>> cultureHistory;

			std::vector<PopRatio> popRatios;
			double originalTax = 0.0;
			double originalProduction = 0.0;
			double originalManpower = 0.0;
	};
}

#endif // PROVINCE_HISTORY_H