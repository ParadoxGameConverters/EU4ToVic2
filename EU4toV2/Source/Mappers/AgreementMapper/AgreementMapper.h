#ifndef AGREEMENT_MAPPER_H
#define AGREEMENT_MAPPER_H

#include "Parser.h"
#include <set>

namespace mappers
{
	class AgreementMapper : commonItems::parser
	{
	public:
		AgreementMapper();
		explicit AgreementMapper(std::istream& theStream);
		
		[[nodiscard]] auto isAgreementInColonies(const std::string& subject) const { return colonies.count(subject) > 0; }
		[[nodiscard]] auto isAgreementInOnesiders(const std::string& subject) const { return onesiders.count(subject) > 0; }
		[[nodiscard]] auto isAgreementInDoublesiders(const std::string& subject) const { return doublesiders.count(subject) > 0; }
		[[nodiscard]] auto isAgreementInTributaries(const std::string& subject) const { return tributaries.count(subject) > 0; }
		[[nodiscard]] auto isAgreementInVassals(const std::string& subject) const { return vassals.count(subject) > 0; }

	private:
		void registerKeys();
		
		std::set<std::string> colonies;
		std::set<std::string> onesiders;
		std::set<std::string> doublesiders;
		std::set<std::string> tributaries;
		std::set<std::string> vassals;
	};
}

#endif // AGREEMENT_MAPPER_H
