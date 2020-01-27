#ifndef PROVINCE_HISTORY_H
#define PROVINCE_HISTORY_H

#include "Date.h"
#include "PopRatio.h"
#include "newParser.h"
#include <optional>
#include <vector>

namespace EU4
{
	class ProvinceHistory: commonItems::parser
	{
	public:
		ProvinceHistory() = default;
		explicit ProvinceHistory(std::istream& theStream);

		[[nodiscard]] std::optional<date> getFirstOwnedDate() const;
		[[nodiscard]] bool hasOriginalCulture() const;
		[[nodiscard]] bool wasColonized() const;
		
		[[nodiscard]] auto getOriginalDevelopment() const { return originalTax + originalProduction + originalManpower; }
		[[nodiscard]] const auto& getPopRatios() const { return popRatios; }

	private:
		void buildPopRatios();
		void decayPopRatios(const date& oldDate, const date& newDate, PopRatio& currentPop);

		std::vector<std::pair<date, std::string>> ownershipHistory;
		std::vector<std::pair<date, std::string>> religionHistory;
		std::vector<std::pair<date, std::string>> cultureHistory;

		std::vector<PopRatio> popRatios;
		double originalTax = 0;
		double originalProduction = 0;
		double originalManpower = 0;
	};
}

#endif // PROVINCE_HISTORY_H