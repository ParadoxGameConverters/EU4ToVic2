#ifndef PROVINCE_HISTORY_H
#define PROVINCE_HISTORY_H

#include "Date.h"
#include "PopRatio.h"
#include "Parser.h"
#include <optional>
#include <vector>

namespace EU4
{
	class ProvinceHistory: commonItems::parser
	{
	public:
		ProvinceHistory() = default;
		explicit ProvinceHistory(std::istream& theStream);

		void updatePopRatioCulture(const std::string& oldCultureName, const std::string& neoCultureName, const std::string& superRegion);
		void buildPopRatios(double assimilationFactor);
		void setStartingCulture(const std::string& culture) { startingCulture = culture; }
		void setStartingReligion(const std::string& religion) { startingCulture = religion; }

		[[nodiscard]] std::optional<date> getFirstOwnedDate() const;
		[[nodiscard]] bool hasOriginalCulture() const;
		[[nodiscard]] const auto& getOriginalCulture() const { return startingCulture; }
		[[nodiscard]] bool wasColonized() const;
		[[nodiscard]] bool hasInitializedHistory() const { return !religionHistory.empty() && !cultureHistory.empty(); }
		
		[[nodiscard]] auto getOriginalDevelopment() const { return originalTax + originalProduction + originalManpower; }
		[[nodiscard]] const auto& getPopRatios() const { return popRatios; }
		
	private:
		void decayPopRatios(const date& oldDate, const date& newDate, PopRatio& currentPop, double assimilationFactor);

		std::string startingCulture;
		std::string startingReligion;

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