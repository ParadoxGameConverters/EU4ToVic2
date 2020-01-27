#ifndef EU4_AGREEMENT_H
#define EU4_AGREEMENT_H

#include "Date.h"
#include "newParser.h"

namespace EU4
{
	class EU4Agreement : commonItems::parser
	{
	public:
		explicit EU4Agreement(std::istream& theStream);

		void setAgreementType(const std::string& _type) { agreementType = _type; }
		void setTargetTag(const std::string& tag) { targetTag = tag; }

		[[nodiscard]] const auto& getAgreementType() const { return agreementType; }
		[[nodiscard]] const auto& getOriginTag() const { return originTag; }
		[[nodiscard]] const auto& getTargetTag() const { return targetTag; }
		[[nodiscard]] const auto& getStartDate() const { return startDate; }
		[[nodiscard]] const auto& getEndDate() const { return endDate; }

	private:
		std::string	agreementType; // combined type of agreement (cb_core, guarantee, vassal)
		std::string originTag; // origin country
		std::string targetTag; // target country
		date startDate; // the date this agreement began
		date endDate = date("1.1.1"); // the date this agreement ends, relevant for PUs
	};
}

#endif // EU4_AGREEMENT_H