#include "Decisions.h"
#include "Taiping.h"
#include "../Country/Country.h"
#include "ParserHelpers.h"
#include "OSCompatibilityLayer.h"
#include "Log.h"
#include <fstream>

V2::Decisions::Decisions(const std::string& filename)
{
	registerKeyword("political_decisions", [this](const std::string& unused, std::istream& theStream) {
		Decisions newDecisions(theStream);
		decisions = newDecisions.getDecisions();
	});
	parseFile("blankMod/output/decisions/" + filename);
	clearRegisteredKeywords();
}

V2::Decisions::Decisions(std::istream& theStream)
{
	registerKeys();
	parseStream(theStream);
	clearRegisteredKeywords();
}

void V2::Decisions::registerKeys()
{
	registerRegex("[a-zA-z0-9_]+", [this](const std::string& decisionTitle, std::istream& theStream) {
		Decision newDecision(theStream);
		newDecision.setTitle(decisionTitle);
		decisions.insert(make_pair(decisionTitle, newDecision));
	});
}

void V2::Decisions::updateDecisions(const std::map<std::string, std::shared_ptr<Country>>& countries)
{
	if (const auto& theDecision = decisions.find("centralize_hre"); theDecision == decisions.end())
		Log(LogLevel::Warning) << "Could not load centralize_hre decision";
	else
	{
		std::string potential;
		potential += "= {\n";
		potential += "\t\t\tOR = {\n";
		potential += "\t\t\t\tAND = {\n";
		potential += "\t\t\t\t\tNOT = { has_global_flag = liberal_revolutions_should_now_fire }\n";
		potential += "\t\t\t\t\thas_country_flag = emperor_hre\n";
		potential += "\t\t\t\t}\n";
		potential += "\t\t\t\tAND = {\n";
		potential += "\t\t\t\t\thas_country_flag = union_is_hre\n";
		potential += "\t\t\t\t\tNOT = {\n";
		potential += "\t\t\t\t\t\tcapital_scope = { continent = africa }\n";
		potential += "\t\t\t\t\t\tcapital_scope = { continent = north_america }\n";
		potential += "\t\t\t\t\t\tcapital_scope = { continent = south_america }\n";
		potential += "\t\t\t\t\t\tcapital_scope = { continent = oceania }\n";
		if (countries.find("FSA") != countries.end())
			potential += "\t\t\t\t\t\t#tag = FSA\n";
		if (countries.find("CSA") != countries.end())
			potential += "\t\t\t\t\t\ttag = CSA\n";
		if (countries.find("MGL") != countries.end())
			potential += "\t\t\t\t\t\ttag = MGL\n";
		if (countries.find("SPQ") != countries.end())
			potential += "\t\t\t\t\t\ttag = SPQ\n";
		if (countries.find("SLA") != countries.end())
			potential += "\t\t\t\t\t\ttag = SLA\n";
		potential += "\t\t\t\t\t\ttag = HRE\n";
		potential += "\t\t\t\t\t}\n";
		potential += "\t\t\t\t}\n";
		potential += "\t\t\t}\n";
		potential += "\t\t\tNOT = {\n";
		potential += "\t\t\t\texists = HRE\n";
		potential += "\t\t\t}\n";
		potential += "\t\t}\n";
		(theDecision->second).updateDecision("potential", potential);
	}

	if (const auto& theDecision = decisions.find("form_two_sicilies"); theDecision == decisions.end())
		Log(LogLevel::Warning) << "Could not load form_two_sicilies decision";
	else
	{
		std::string potential;
		potential += "= {\n";
		potential += "\t\t\tOR = {\n";
		potential += "\t\t\t\tAND = {\n";
		potential += "\t\t\t\t\tprimary_culture = south_italian\n";
		if (countries.find("SCY") != countries.end())
			potential += "\t\t\t\t\tNOT = { exists = SCY }\n";
		if (countries.find("NAP") != countries.end())
			potential += "\t\t\t\t\tNOT = { exists = NAP }\n";
		potential += "\t\t\t\t}\n";
		if (countries.find("NAP") != countries.end())
		{
			potential += "\t\t\t\tAND = {\n";
			potential += "\t\t\t\t\ttag = NAP\n";
			potential += "\t\t\t\t\tNOT = { exists = SCY }\n";
			potential += "\t\t\t\t}\n";
		}
		if (countries.find("SCY") != countries.end())
		{
			potential += "\t\t\t\tAND = {\n";
			potential += "\t\t\t\t\ttag = SCY\n";
			potential += "\t\t\t\t\tNOT = { exists = NAP }\n";
			potential += "\t\t\t\t}\n";
		}
		potential += "\t\t\t}\n";
		if (countries.find("SIC") != countries.end())
			potential += "\t\t\tNOT = { exists = SIC }\n";
		if (countries.find("ITA") != countries.end())
			potential += "\t\t\tNOT = { exists = ITA }\n";
		if (countries.find("SPQ") != countries.end())
			potential += "\t\t\tNOT = { tag = SPQ }\n";
		if (countries.find("BYZ") != countries.end())
			potential += "\t\t\tNOT = { tag = BYZ }\n";
		if (countries.find("PAP") != countries.end())
			potential += "\t\t\tNOT = { tag = PAP }\n";
		potential += "\t\t}\n";
		(theDecision->second).updateDecision("potential", potential);
	}

	if (const auto& theDecision = decisions.find("mongol_khagan"); theDecision == decisions.end())
		Log(LogLevel::Warning) << "Could not load mongol_khagan decision";
	else
	{
		if (countries.find("KHA") == countries.end())
			decisions.erase(theDecision);
	}

	if (const auto& theDecision = decisions.find("taiping_and_csa"); theDecision == decisions.end())
		Log(LogLevel::Warning) << "Could not load taiping_and_csa decision";
	else
	{
		std::istringstream effectString(theDecision->second.getEffect());
		Taiping taiping(effectString);

		std::vector<std::string> heimaten = {"PER", "TUR", "RUS", "SLA"};
		std::string effect;
		effect += "= {\n";
		for (const auto& nonCountrySpecificEffect: taiping.getNonCountrySpecificEffects())
		{
			effect += "\t\t\t" + nonCountrySpecificEffect + "\n";
		}
		for (const auto& tag: taiping.getTagEffectMap())
		{
			if (countries.find(tag.first) != countries.end()
				 && std::find(heimaten.begin(), heimaten.end(), tag.first) == heimaten.end()
				 && tag.first != "CHI")
			{
				effect += "\t\t\t" + tag.second + "\n";
			}
		}
		for (const auto& coreEffect: taiping.getCountryCores())
		{
			effect += "\t\t\t" + coreEffect + "\n";
		}

		//Heimaten
		std::vector<std::string> coresToRemove = {"GEO", "ARM", "AZB"};
		bool anythingToRemove = false;
		for (const auto& tag: coresToRemove)
		{
			if (countries.find(tag) != countries.end())
			{
				anythingToRemove = true;
				break;
			}
		}
		if (anythingToRemove)
		{
			for (const auto& tag: heimaten)
			{
				if (countries.find(tag) == countries.end())
					continue;
				effect += "\t\t\t" + tag + " = {\n";
				effect += "\t\t\t\tany_owned_province = {\n";
				effect += "\t\t\t\t\tlimit = {\n";
				effect += "\t\t\t\t\t\tNOT = {\n";
				effect += "\t\t\t\t\t\t\tregion = RUS_1090 #Georgia\n";
				effect += "\t\t\t\t\t\t\tregion = RUS_1098 #Armenia\n";
				effect += "\t\t\t\t\t\t\tregion = RUS_1102 #Azerbaijan\n";
				effect += "\t\t\t\t\t\t}\n";
				effect += "\t\t\t\t\t}\n";
				if (countries.find("GEO") != countries.end())
					effect += "\t\t\t\t\tremove_core = GEO\n";
				if (countries.find("ARM") != countries.end())
					effect += "\t\t\t\t\tremove_core = ARM\n";
				if (countries.find("AZB") != countries.end())
					effect += "\t\t\t\t\tremove_core = AZB\n";
				effect += "\t\t\t\t}\n";
				effect += "\t\t\t}\n";
			}
		}

		if (countries.find("CHI") != countries.end())
		{
			effect += "\t\t\tCHI = {\n";
			effect += "\t\t\t\tany_owned_province = {\n";
			effect += "\t\t\t\t\tlimit = {\n";
			effect += "\t\t\t\t\t\tNOT = {\n";
			if (countries.find("MGL") != countries.end())
				effect += "\t\t\t\t\t\t\tis_core = MGL\n";
			effect += "\t\t\t\t\t\t\tregion = CHI_2608 #Mongolia proper\n";
			effect += "\t\t\t\t\t\t}\n";
			effect += "\t\t\t\t\t}\n";
			effect += "\t\t\t\t}\n";
			if (countries.find("KHA") != countries.end())
				effect += "\t\t\t\tremove_core = KHA\n";
			if (countries.find("MGL") != countries.end())
				effect += "\t\t\t\tremove_core = MGL\n";
			effect += "\t\t\t\tadd_core = IMG\n";
			effect += "\t\t\t}\n";
		}

		effect += "\t\t\tset_global_flag = taiping_and_csa\n";
		effect += "\t\t}\n";
		(theDecision->second).updateDecision("effect", effect);
	}
}