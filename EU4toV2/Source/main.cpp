#include "Log.h"
#include "EU4ToVic2Converter.h"
#include "Mappers/VersionParser/VersionParser.h"

int main(const int argc, const char * argv[])
{
	try
	{
		const mappers::VersionParser versionParser;
		LOG(LogLevel::Info) << versionParser;
		LOG(LogLevel::Info) << "Built on " << __TIMESTAMP__;
		if (argc >= 2)
		{			
			LOG(LogLevel::Error) << "EU4toVic2 no longer takes savegame location as a parameter.";
			LOG(LogLevel::Error) << "Please place the correct path to your save game in configuration.txt";
			return 0;
		}
		convertEU4ToVic2(versionParser);
		return 0;
	}

	catch (const std::exception& e)
	{
		LOG(LogLevel::Error) << e.what();
		return -1;
	}
}