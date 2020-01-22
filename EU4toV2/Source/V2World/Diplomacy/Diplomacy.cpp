#include "Diplomacy.h"
#include "Log.h"
#include "../../Configuration.h"
#include "OSCompatibilityLayer.h"
#include "Relation.h"
#include "../../EU4World/Country/EU4Country.h"
#include <fstream>
#include "../Country/Country.h"

void V2::Diplomacy::convertDiplomacy(
	std::vector<EU4::EU4Agreement> eu4agreements,
	const mappers::CountryMappings& countryMapper, 
	std::map<std::string, std::shared_ptr<Country>>& countries)
{
	for (auto& agreement : eu4agreements)
	{
		auto EU4Tag1 = agreement.getOriginTag();
		auto ifV2Tag1 = countryMapper.getV2Tag(EU4Tag1);
		if (!ifV2Tag1) continue;
		auto V2Tag1 = *ifV2Tag1;

		auto EU4Tag2 = agreement.getTargetTag();
		auto ifV2Tag2 = countryMapper.getV2Tag(EU4Tag2);
		if (!ifV2Tag2) continue;
		auto V2Tag2 = *ifV2Tag2;

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

		// Stop creating relations for/with nations that didn't survive province conversion!
		if (country1->second->getProvinces().empty()) continue;
		if (country2->second->getProvinces().empty()) continue;

		auto& r1 = country1->second->getRelation(V2Tag2);
		auto& r2 = country2->second->getRelation(V2Tag1);

		if (subjectMapper.isSubjectInColonies(agreement.getAgreementType()))
		{
			country2->second->setColonyOverlord(country1->second->getTag());
			// Do we annex or not?
			if (country2->second->getSourceCountry()->getLibertyDesire() < theConfiguration.getLibertyThreshold())
			{
				LOG(LogLevel::Info) << " - " << country1->second->getTag() << " is absorbing " << country2->second->getTag() <<
					" (" << country2->second->getSourceCountry()->getLibertyDesire() << " vs " << theConfiguration.getLibertyThreshold() << " liberty desire)";
				country1->second->absorbColony(*country2->second);
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
		}

		if (subjectMapper.isSubjectInOnesiders(agreement.getAgreementType()))
		{
			// influence level +1, but never exceed 4
			if (r1.getLevel() < 4) r1.setLevel(r1.getLevel() + 1);
			r1.increaseRelations(100);
			r1.setInfluence(20);
		}

		if (subjectMapper.isSubjectInDoublesiders(agreement.getAgreementType()))
		{
			// doublesiders are bidirectional; influence level +1, but never exceed 4
			if (r1.getLevel() < 4) r1.setLevel(r1.getLevel() + 1);
			r1.increaseRelations(100);
			r1.setInfluence(20);
			if (r2.getLevel() < 4) r2.setLevel(r2.getLevel() + 1);
			r2.increaseRelations(100);
			r1.setInfluence(20);
		}

		if (subjectMapper.isSubjectInTributaries(agreement.getAgreementType()))
		{
			// influence level 5 - sphere, but not vassal, and military access is expected.
			r1.setLevel(5);
			r1.increaseRelations(50);
			r1.setInfluence(50);
			r1.setAccess(true);
			r2.setAccess(true);
		}

		if (subjectMapper.isSubjectInVassals(agreement.getAgreementType()))
		{
			// Yeah, we don't do marches, clients or all that. Or personal unions. PUs are a second relation
			// beside existing vassal relation specifying when vassalage ends.
			// However, vanilla Vic2 has PU end dates based on historical events, and we don't simulate those (for now).
			agreement.setAgreementType("vassal");
			r1.setLevel(5);
			r1.increaseRelations(75);
			r1.setInfluence(75);
			// In vic2 military access through vassals is not automatic.
			r1.setAccess(true);
			r2.setAccess(true);
			country2->second->addPrestige(-country2->second->getPrestige());
		}

		// In essence we should only recognize 3 diplomacy categories and these are it.
		if (agreement.getAgreementType() == "alliance" || agreement.getAgreementType() == "vassal" || agreement.getAgreementType() == "guarantee")
		{
			// copy agreement
			Agreement v2agreement(V2Tag1, V2Tag2, agreement.getAgreementType(), agreement.getStartDate());
			agreements.push_back(v2agreement);
		}
	}

	// Reward good starting relations with a small amount of extra influence, which will be relevant 
	// to stating GPs, and will look natural.
	for (const auto& country: countries)
	{
		for (auto& relation: country.second->getRelations())
		{			
			if (relation.second.getRelations() > 50)
			{
				auto bonus = static_cast<int>((relation.second.getRelations() - 50) / 4);
				auto newInfluence = std::min(relation.second.getInfluence() + bonus, 100);
				relation.second.setInfluence(newInfluence);
			}
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
	
	for (const auto& agreement: agreements)
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