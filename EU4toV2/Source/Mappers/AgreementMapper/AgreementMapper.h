#ifndef AGREEMENT_MAPPER_H
#define AGREEMENT_MAPPER_H

#include "newParser.h"
#include <set>

namespace mappers
{
	class AgreementMapper : commonItems::parser
	{
	public:
		AgreementMapper();
		AgreementMapper(std::istream& theStream);
		bool isAgreementInColonies(const std::string& subject) const { return colonies.count(subject) > 0; }
		bool isAgreementInOnesiders(const std::string& subject) const { return onesiders.count(subject) > 0; }
		bool isAgreementInDoublesiders(const std::string& subject) const { return doublesiders.count(subject) > 0; }
		bool isAgreementInTributaries(const std::string& subject) const { return tributaries.count(subject) > 0; }
		bool isAgreementInVassals(const std::string& subject) const { return vassals.count(subject) > 0; }

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
