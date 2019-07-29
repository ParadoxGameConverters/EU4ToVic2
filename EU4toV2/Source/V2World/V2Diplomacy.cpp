/*Copyright (c) 2019 The Paradox Game Converters Project

Permission is hereby granted, free of charge, to any person obtaining
a copy of this software and associated documentation files (the
"Software"), to deal in the Software without restriction, including
without limitation the rights to use, copy, modify, merge, publish,
distribute, sublicense, and/or sell copies of the Software, and to
permit persons to whom the Software is furnished to do so, subject to
the following conditions:

The above copyright notice and this permission notice shall be included
in all copies or substantial portions of the Software.

THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND,
EXPRESS OR IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF
MERCHANTABILITY, FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT.
IN NO EVENT SHALL THE AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY
CLAIM, DAMAGES OR OTHER LIABILITY, WHETHER IN AN ACTION OF CONTRACT,
TORT OR OTHERWISE, ARISING FROM, OUT OF OR IN CONNECTION WITH THE
SOFTWARE OR THE USE OR OTHER DEALINGS IN THE SOFTWARE.*/



#include "V2Diplomacy.h"
#include "Log.h"
#include "../Configuration.h"
#include "OSCompatibilityLayer.h"
#include <fstream>



std::ostream& operator<<(std::ostream& output, V2Agreement theAgreement)
{
	output << theAgreement.type << "=\n";
	output << "{\n";
	output << "\tfirst=\"" << theAgreement.country1 << "\"\n";
	output << "\tsecond=\"" << theAgreement.country2 << "\"\n";
	output << "\tstart_date=\"" << theAgreement.start_date << "\"\n";
	output << "\tend_date=\"1936.1.1\"\n";
	output << "}\n";
	output << "\n";

	return output;
}


void V2Diplomacy::output() const
{
	LOG(LogLevel::Debug) << "Writing diplomacy";
	Utils::TryCreateFolder("Output/" + theConfiguration.getOutputName() + "/history/diplomacy");

	std::ofstream alliances("Output/" + theConfiguration.getOutputName() + "/history/diplomacy/Alliances.txt");
	if (!alliances.is_open())
	{
		LOG(LogLevel::Error) << "Could not create alliances history file";
		exit(-1);
	}

	std::ofstream guarantees("Output/" + theConfiguration.getOutputName() + "/history/diplomacy/Guarantees.txt");
	if (!guarantees.is_open())
	{
		LOG(LogLevel::Error) << "Could not create guarantees history file";
		exit(-1);
	}

	std::ofstream puppetStates("Output/" + theConfiguration.getOutputName() + "/history/diplomacy/PuppetStates.txt");
	if (!puppetStates.is_open())
	{
		LOG(LogLevel::Error) << "Could not create puppet states history file";
		exit(-1);
	}

	std::ofstream unions("Output/" + theConfiguration.getOutputName() + "/history/diplomacy/Unions.txt");
	if (!unions.is_open())
	{
		LOG(LogLevel::Error) << "Could not create unions history file";
		exit(-1);
	}
	
	for (auto agreement: agreements)
	{
		if (agreement.type == "guarantee")
		{
			guarantees << agreement;
		}
		else if (agreement.type == "union")
		{
			unions << agreement;
		}
		else if (agreement.type == "vassal")
		{
			puppetStates << agreement;
		}
		else if (agreement.type == "alliance")
		{
			alliances << agreement;
		}
		else
		{
			LOG(LogLevel::Warning) << "Cannot ouput diplomatic agreement type " << agreement.type;
			continue;
		}
	}
	
	alliances.close();
	guarantees.close();
	puppetStates.close();
	unions.close();
}