#include "Configuration.h"
#include "output.h"

std::ostream& V2::operator<<(std::ostream& output, const ModFile& modFile)
{
	output << "name = \"Converted - " << Localisation::convert(theConfiguration.getActualName().string()) << "\"\n";
	output << "path = \"mod/" << theConfiguration.getOutputName().string() << "\"\n";
	output << "user_dir = \"" << theConfiguration.getOutputName().string() << "\"\n";
	output << "replace = \"history/provinces\"\n";
	output << "replace = \"history/countries\"\n";
	output << "replace = \"history/diplomacy\"\n";
	output << "replace = \"history/units\"\n";
	output << "replace = \"history/wars\"\n";
	output << "replace = \"history/pops/1836.1.1\"\n";
	output << "replace = \"common/religion.txt\"\n";
	output << "replace = \"common/cultures.txt\"\n";
	output << "replace = \"common/countries.txt\"\n";
	output << "replace = \"common/countries/\"\n";
	output << "replace = \"gfx/interface/icon_religion.dds\"\n";
	output << "replace = \"localisation/0_Names.csv\"\n";
	output << "replace = \"localisation/0_Cultures.csv\"\n";
	output << "replace = \"localisation/0_Religions.csv\"\n";

	return output;
}
