#include <ostream>
#include "../Party/Party.h"

std::ostream& V2::operator<<(std::ostream& output, const Party& party)
{
	output << '\n';
	output << "party = {\n";
	output << "\tname = \"" << party.name << "\"\n";
	output << party.partyDetails;
	output << "}\n";
	return output;
}

std::ostream& mappers::operator<<(std::ostream& output, const mappers::PartyType& partyDetails)
{
	output << "\tstart_date = " << partyDetails.start_date << '\n';
	output << "\tend_date = " << partyDetails.end_date << "\n\n";
	output << "\tideology = " << partyDetails.ideology << "\n\n";
	output << "\teconomic_policy = " << partyDetails.economic_policy << '\n';
	output << "\ttrade_policy = " << partyDetails.trade_policy << '\n';
	output << "\treligious_policy = " << partyDetails.religious_policy << '\n';
	output << "\tcitizenship_policy = " << partyDetails.citizenship_policy << '\n';
	output << "\twar_policy = " << partyDetails.war_policy << '\n';
	return output;
}
