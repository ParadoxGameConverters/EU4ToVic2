#ifndef EU4AGREEMENT_H
#define EU4AGREEMENT_H

#include "Date.h"
#include "newParser.h"

namespace EU4
{
	class EU4Agreement : commonItems::parser
	{
	public:
		EU4Agreement(std::istream& theStream);
		std::string getAgreementType() const { return agreementType; }
		void setAgreementType(std::string _type) { agreementType = _type; }
		std::string getOriginTag() const { return originTag; }
		std::string getTargetTag() const { return targetTag; }
		void setTargetTag(std::string tag) { targetTag = tag; }
		date getStartDate() const { return startDate; }
		date getEndDate() const { return endDate; }

	private:
		std::string	agreementType; // combined type of agreement (cb_core, guarantee, vassal)
		std::string originTag; // origin country
		std::string targetTag; // target country
		date startDate; // the date this agreement began
		date endDate = date("1.1.1"); // the date this agreement ends, releevant for PUs
	};
}

#endif // EU4AGREEMENT_H