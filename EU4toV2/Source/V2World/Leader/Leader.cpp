#include "Leader.h"

V2::Leader::Leader(const EU4::Leader& oldLeader, const mappers::LeaderTraitMapper& leaderTraitMapper):
	name(oldLeader.getName()),
	activationDate(oldLeader.getActivationDate()),
	isLand(oldLeader.isLand())
{
	std::optional<std::string> incPersonality = leaderTraitMapper.getPersonality(oldLeader.getFire(), oldLeader.getShock(), oldLeader.getManuever(), oldLeader.getSiege());
	std::optional<std::string> incBackground = leaderTraitMapper.getBackground(oldLeader.getFire(), oldLeader.getShock(), oldLeader.getManuever(), oldLeader.getSiege());
	if (incPersonality) personality = *incPersonality;
	if (incBackground) background = *incBackground;
}
