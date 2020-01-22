#include "AgreementMapper.h"
#include "ParserHelpers.h"
#include "Log.h"

mappers::AgreementMapper::AgreementMapper()
{
	Log(LogLevel::Info) << "Parsing Agreement Types.";
	registerKeys();
	parseFile("configurables/agreement_types.txt");
	clearRegisteredKeywords();
}

mappers::AgreementMapper::AgreementMapper(std::istream& theStream)
{
	registerKeys();
	parseStream(theStream);
	clearRegisteredKeywords();
}

void mappers::AgreementMapper::registerKeys()
{
	registerKeyword("colonies", [this](const std::string& unused, std::istream& theStream)
		{
			commonItems::stringList subjectList(theStream);
			auto theList = subjectList.getStrings();
			colonies = std::set(theList.begin(), theList.end());
		});
	registerKeyword("onesiders", [this](const std::string& unused, std::istream& theStream)
		{
			commonItems::stringList subjectList(theStream);
			auto theList = subjectList.getStrings();
			onesiders = std::set(theList.begin(), theList.end());
		});
	registerKeyword("doublesiders", [this](const std::string& unused, std::istream& theStream)
		{
			commonItems::stringList subjectList(theStream);
			auto theList = subjectList.getStrings();
			doublesiders = std::set(theList.begin(), theList.end());
		});
	registerKeyword("tributaries", [this](const std::string& unused, std::istream& theStream)
		{
			commonItems::stringList subjectList(theStream);
			auto theList = subjectList.getStrings();
			tributaries = std::set(theList.begin(), theList.end());
		});
	registerKeyword("vassals", [this](const std::string& unused, std::istream& theStream)
		{
			commonItems::stringList subjectList(theStream);
			auto theList = subjectList.getStrings();
			vassals = std::set(theList.begin(), theList.end());
		});
	registerRegex("[a-zA-Z0-9\\_.:]+", commonItems::ignoreItem);
}
