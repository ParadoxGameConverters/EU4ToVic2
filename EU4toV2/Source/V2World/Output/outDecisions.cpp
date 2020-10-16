#include "output.h"

std::ostream& V2::operator<<(std::ostream& output, const Decisions& decisions)
{
	output << "political_decisions = {\n";
	for (const auto& decision : decisions.getDecisions())
	{
		output << decision.second << "\n";
	}
	output << "}";

	return output;
}

std::ostream& V2::operator<<(std::ostream& output, const Decision& decision)
{
	output << "\t" << decision.title << " = {\n";
	if (!decision.picture.empty())
		output << "\t\tpicture = " << decision.picture << "\n";
	if (!decision.news.empty())
	{
		output << "\t\tnews = " << decision.news << "\n";
		output << "\t\tnews_desc_long = \"" << decision.title << "_NEWS_LONG\"\n";
		output << "\t\tnews_desc_medium = \"" << decision.title << "_NEWS_MEDIUM\"\n";
		output << "\t\tnews_desc_short = \"" << decision.title << "_NEWS_SHORT\"\n";
	}
	output << "\t\tpotential " << decision.potential << "\n";
	output << "\t\tallow " << decision.allow << "\n";
	output << "\t\teffect " << decision.effect << "\n";
	if(!decision.aiWillDo.empty())
		output << "\t\tai_will_do " << decision.aiWillDo << "\n";
	output << "\t}\n";

	return output;
}
