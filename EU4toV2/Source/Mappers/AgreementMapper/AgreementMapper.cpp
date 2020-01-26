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
			const commonItems::stringList subjectList(theStream);
			for (const auto& subject : subjectList.getStrings()) colonies.insert(subject);
		});
	registerKeyword("onesiders", [this](const std::string& unused, std::istream& theStream)
		{
			const commonItems::stringList subjectList(theStream);
			for (const auto& subject : subjectList.getStrings()) onesiders.insert(subject);
		});
	registerKeyword("doublesiders", [this](const std::string& unused, std::istream& theStream)
		{
			const commonItems::stringList subjectList(theStream);
			for (const auto& subject : subjectList.getStrings()) doublesiders.insert(subject);
		});
	registerKeyword("tributaries", [this](const std::string& unused, std::istream& theStream)
		{
			const commonItems::stringList subjectList(theStream);
			for (const auto& subject : subjectList.getStrings()) tributaries.insert(subject);
		});
	registerKeyword("vassals", [this](const std::string& unused, std::istream& theStream)
		{
			const commonItems::stringList subjectList(theStream);
			for (const auto& subject : subjectList.getStrings()) vassals.insert(subject);
		});
	registerRegex("[a-zA-Z0-9\\_.:]+", commonItems::ignoreItem);
}
