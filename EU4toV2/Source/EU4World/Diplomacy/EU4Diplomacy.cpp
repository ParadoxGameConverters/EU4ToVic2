#include "EU4Diplomacy.h"
#include "ParserHelpers.h"

EU4::EU4Diplomacy::EU4Diplomacy(std::istream& theStream)
{
	registerKeyword(std::regex("royal_marriage|guarantee|alliance"), [this](const std::string& agreementClass, std::istream& theStream)
		{
			EU4::EU4Agreement newAgreement(theStream);
			newAgreement.setAgreementType(agreementClass);
			agreements.push_back(newAgreement);
		}
	);
	// new saves use dependancy for subjects. We alter nothing.
	registerKeyword(std::regex("dependency"), [this](const std::string& agreementClass, std::istream& theStream)
		{
			EU4::EU4Agreement newAgreement(theStream);
			agreements.push_back(newAgreement);
		}
	);
	// support for old saves, colonial is a special case we don't drop to vasal.
	registerKeyword(std::regex("is_colonial"), [this](const std::string& agreementClass, std::istream& theStream)
		{
			EU4::EU4Agreement newAgreement(theStream);
			newAgreement.setAgreementType("colonial");
			agreements.push_back(newAgreement);
		}
	);
	// support for old saves, all of these drop to vassal.
	registerKeyword(std::regex("vassal|protectorate|is_march|sphere|union"), [this](const std::string& agreementClass, std::istream& theStream)
		{
			EU4::EU4Agreement newAgreement(theStream);
			newAgreement.setAgreementType("vassal");
			agreements.push_back(newAgreement);
		}
	);
	registerKeyword(std::regex("[a-zA-Z0-9_\\.:]+"), commonItems::ignoreItem);

	parseStream(theStream);
}


