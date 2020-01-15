#include "outParty.h"

std::ostream& V2::operator<<(std::ostream& output, const Party& party)
{
	output << '\n';
	output << "party = {\n";
	output << "\tname = \"" << party.name << "\"\n";
	output << "\tstart_date = " << party.start_date << '\n';
	output << "\tend_date = " << party.end_date << "\n\n";
	output << "\tideology = " << party.ideology << "\n\n";
	output << "\teconomic_policy = " << party.economic_policy << '\n';
	output << "\ttrade_policy = " << party.trade_policy << '\n';
	output << "\treligious_policy = " << party.religious_policy << '\n';
	output << "\tcitizenship_policy = " << party.citizenship_policy << '\n';
	output << "\twar_policy = " << party.war_policy << '\n';
	output << "}\n";
	return output;
}
