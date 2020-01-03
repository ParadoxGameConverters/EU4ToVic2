#include "V2Leader.h"
#include "Log.h"


V2Leader::V2Leader(const EU4::Leader& oldLeader, const mappers::V2LeaderTraitMapper& traitTypes)
{
	name = oldLeader.getName();
	activationDate	= oldLeader.getActivationDate();
	isLand = oldLeader.isLand();
	eu4ID = oldLeader.getID();
	std::optional<std::string> incPersonality = traitTypes.getPersonality(oldLeader.getFire(), oldLeader.getShock(), oldLeader.getManuever(), oldLeader.getSiege());
	std::optional<std::string> incBackground = traitTypes.getBackground(oldLeader.getFire(), oldLeader.getShock(), oldLeader.getManuever(), oldLeader.getSiege());
	if (incPersonality) personality = *incPersonality;
	if (incBackground) background = *incBackground;
}

std::ostream& operator<<(std::ostream& output, const V2Leader& leader)
{
	output << "leader = {\n";
	output << "\tname=\"" << leader.name << "\"\n";
	output << "\tdate=\"" << leader.activationDate << "\"\n";
	if (leader.isLand)
	{
		output << "\ttype=land\n";
	}
	else
	{
		output << "\ttype=sea\n";
	}
	output << "\tpersonality=\"" << leader.personality << "\"\n";
	output << "\tbackground=\"" << leader.background << "\"\n";
	output << "}\n";
	output << "\n";

	return output;
}
