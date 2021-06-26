#include "EU4ToVic2Converter.h"
#include "Log.h"
#include "Mappers/ConverterVersion/ConverterVersion.h"
#include <fstream>

int main(const int argc, const char* argv[])
{
	try
	{
		std::ofstream clearLog("log.txt");
		clearLog.close();
		const mappers::ConverterVersion converterVersion;
		LOG(LogLevel::Info) << converterVersion;
		LOG(LogLevel::Info) << "Built on " << __TIMESTAMP__;
		if (argc >= 2)
		{
			std::string argv1 = argv[1];
			if (argv1 != "input.eu4")
			{
				LOG(LogLevel::Info) << "EU4toVic2 no longer takes savegame path as a parameter";
				LOG(LogLevel::Info) << "It uses configuration.txt, configured manually or by the frontend.";
			}
		}
		convertEU4ToVic2(converterVersion);
		return 0;
	}

	catch (const std::exception& e)
	{
		LOG(LogLevel::Error) << e.what();
		return -1;
	}
}