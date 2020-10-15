#include "Decisions.h"
#include "ParserHelpers.h"
#include "OSCompatibilityLayer.h"
#include "Log.h"

V2::Decisions::Decisions()
{
	const auto& decisionsFiles = Utils::GetAllFilesInFolder("blankMod/output/decisions");
	Log(LogLevel::Info) << "Loading decisions";
	for (const auto& decisionsFile: decisionsFiles)
	{
		LOG(LogLevel::Info) << " -> " << decisionsFile;
		registerKeyword("political_decisions", [this](const std::string& unused, std::istream& theStream) {
			Decisions newDecisions(theStream);
		});
		parseFile("blankMod/output/decisions/" + decisionsFile);
		clearRegisteredKeywords();
	}
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