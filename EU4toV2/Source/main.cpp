#include "Log.h"
#include "EU4ToVic2Converter.h"
#include "OSCompatibilityLayer.h"
#include <string>
#include "Mappers/VersionParser/VersionParser.h"

int main(const int argc, const char * argv[])
{
	try
	{
		const mappers::VersionParser versionParser;
		LOG(LogLevel::Info) << versionParser;
		LOG(LogLevel::Info) << "Built on " << __TIMESTAMP__;
		LOG(LogLevel::Info) << "\tCurrent directory is " << Utils::getCurrentDirectory();

		std::string eu4SaveFileName;
		if (argc >= 2)
		{
			LOG(LogLevel::Info) << "Using input file " << argv[1];
			eu4SaveFileName = argv[1];
		}
		else
		{
			LOG(LogLevel::Info) << "No input file given, defaulting to input.eu4";
			eu4SaveFileName = "input.eu4";
		}
		convertEU4ToVic2(eu4SaveFileName, versionParser);

		return 0;
	}

	catch (const std::exception& e)
	{
		LOG(LogLevel::Error) << e.what();
		return -1;
	}
}