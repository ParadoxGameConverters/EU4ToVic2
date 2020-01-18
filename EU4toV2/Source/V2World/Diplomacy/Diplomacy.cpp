#include "Diplomacy.h"
#include "Log.h"
#include "../../Configuration.h"
#include "OSCompatibilityLayer.h"
#include "Relation.h"
#include "../../EU4World/Country/EU4Country.h"
#include <fstream>

void V2::Diplomacy::convertDiplomacy(
	std::vector<EU4::EU4Agreement> eu4agreements,
	const mappers::CountryMappings& countryMapper, 
	std::map<std::string, V2Country*>& countries)
{
	for (auto& agreement : eu4agreements)
	{
		auto EU4Tag1 = agreement.getOriginTag();
		auto V2Tag1 = countryMapper.getV2Tag(EU4Tag1);
		if (V2Tag1.empty()) continue;

		auto EU4Tag2 = agreement.getTargetTag();
		auto V2Tag2 = countryMapper.getV2Tag(EU4Tag2);
		if (V2Tag2.empty()) continue;

		const auto& country1 = countries.find(V2Tag1);
		const auto& country2 = countries.find(V2Tag2);
		if (country1 == countries.end())
		{
			LOG(LogLevel::Warning) << "Vic2 country " << V2Tag1 << " used in diplomatic agreement doesn't exist";
			continue;
		}
		if (country2 == countries.end())
		{
			LOG(LogLevel::Warning) << "Vic2 country " << V2Tag2 << " used in diplomatic agreement doesn't exist";
			continue;
		}
		
		auto& r1 = country1->second->getRelation(V2Tag2);
		auto& r2 = country2->second->getRelation(V2Tag1);

		if (agreement.getAgreementType() == "colonial" || agreement.getAgreementType() == "colony")
		{
			country2->second->setColonyOverlord(country1->second);
			if (country2->second->getSourceCountry()->getLibertyDesire() < theConfiguration.getLibertyThreshold())
			{
				LOG(LogLevel::Info) << " - " << country1->second->getTag() << " is absorbing " << country2->second->getTag() <<
					" (" << country2->second->getSourceCountry()->getLibertyDesire() << " vs " << theConfiguration.getLibertyThreshold() << " liberty desire)";
				country1->second->absorbVassal(country2->second);
				for (auto& agreement2 : eu4agreements)
				{
					if (agreement2.getTargetTag() == country2->second->getSourceCountry()->getTag())
					{
						agreement2.setTargetTag(country1->second->getSourceCountry()->getTag());
					}
				}
				continue;
			}
			
			LOG(LogLevel::Info) << " - " << country1->second->getTag() << " is not absorbing " << country2->second->getTag() <<
				" (" << country2->second->getSourceCountry()->getLibertyDesire() << " vs " << theConfiguration.getLibertyThreshold() << " liberty desire)";
			Agreement v2agreement(V2Tag1, V2Tag2, "vassal", agreement.getStartDate());
			agreements.push_back(v2agreement);
			r1.setLevel(5);
			country2->second->addPrestige(-country2->second->getPrestige());
		}

		if (agreement.getAgreementType() == "royal_marriage" || agreement.getAgreementType() == "guarantee")
		{
			// influence level +1, but never exceed 4
			if (r1.getLevel() < 4) r1.setLevel(r1.getLevel() + 1);
		}
		if (agreement.getAgreementType() == "royal_marriage")
		{
			// royal marriage is bidirectional; influence level +1, but never exceed 4
			if (r2.getLevel() < 4) r2.setLevel(r2.getLevel() + 1);
		}
		if (agreement.getAgreementType() == "vassal" || agreement.getAgreementType() == "client_vassal" || agreement.getAgreementType() == "daimyo_vassal" || agreement.getAgreementType() == "protectorate" || agreement.getAgreementType() == "tributary_state")
		{
			r1.setLevel(5);
			country2->second->addPrestige(-country2->second->getPrestige());
		}

		if (agreement.getAgreementType() == "is_march" || agreement.getAgreementType() == "march" || agreement.getAgreementType() == "union" || agreement.getAgreementType() == "personal_union")
		{
			// Yeah, we don't do marches or personal unions. PUs are a second relation beside existing vassal relation specifying when vassalage ends.
			// We assume all rulers are CK2 immortals and vassalage does not end with a specific date.
			agreement.setAgreementType("vassal");
			r1.setLevel(5);
			country2->second->addPrestige(-country2->second->getPrestige());
		}

		if (agreement.getAgreementType() == "alliance" || agreement.getAgreementType() == "vassal" || agreement.getAgreementType() == "client_vassal" || agreement.getAgreementType() == "daimyo_vassal" || agreement.getAgreementType() == "guarantee")
		{
			// copy agreement
			Agreement v2agreement(V2Tag1, V2Tag2, agreement.getAgreementType(), agreement.getStartDate());
			agreements.push_back(v2agreement);
		}
	}

}

void V2::Diplomacy::output() const
{
	Utils::TryCreateFolder("output/" + theConfiguration.getOutputName() + "/history/diplomacy");

	std::ofstream alliances("output/" + theConfiguration.getOutputName() + "/history/diplomacy/Alliances.txt");
	if (!alliances.is_open()) throw std::runtime_error("Could not create alliances history file!");

	std::ofstream guarantees("output/" + theConfiguration.getOutputName() + "/history/diplomacy/Guarantees.txt");
	if (!guarantees.is_open()) throw std::runtime_error("Could not create guarantees history file!");

	std::ofstream puppetStates("output/" + theConfiguration.getOutputName() + "/history/diplomacy/PuppetStates.txt");
	if (!puppetStates.is_open()) throw std::runtime_error("Could not create puppet states history file!");

	std::ofstream unions("output/" + theConfiguration.getOutputName() + "/history/diplomacy/Unions.txt");
	if (!unions.is_open()) throw std::runtime_error("Could not create unions history file!");
	
	for (auto agreement: agreements)
	{
		if (agreement.getType() == "guarantee")
		{
			guarantees << agreement;
		}
		else if (agreement.getType() == "union")
		{
			unions << agreement;
		}
		else if (agreement.getType() == "vassal")
		{
			puppetStates << agreement;
		}
		else if (agreement.getType() == "alliance")
		{
			alliances << agreement;
		}
		else
		{
			LOG(LogLevel::Warning) << "Cannot output diplomatic agreement type " << agreement.getType() << "!";
		}
	}
	
	alliances.close();
	guarantees.close();
	puppetStates.close();
	unions.close();
}