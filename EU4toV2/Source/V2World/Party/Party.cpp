#include "Party.h"
#include "ParserHelpers.h"
#include "Log.h"

V2::Party::Party(std::istream& theStream)
{
	registerKeyword(std::regex("name"), [this](const std::string& unused, std::istream& theStream)
		{
			commonItems::singleString nameStr(theStream);
			name = nameStr.getString();
		});
	registerKeyword(std::regex("ideology"), [this](const std::string& unused, std::istream& theStream)
		{
			commonItems::singleString ideologyStr(theStream);
			ideology = ideologyStr.getString();
		});
	registerKeyword(std::regex("start_date"), [this](const std::string& unused, std::istream& theStream)
		{
			commonItems::singleString dateStr(theStream);
			start_date = date(dateStr.getString());
		});
	registerKeyword(std::regex("end_date"), [this](const std::string& unused, std::istream& theStream)
		{
			commonItems::singleString dateStr(theStream);
			end_date = date(dateStr.getString());
		});
	registerKeyword(std::regex("economic_policy"), [this](const std::string& unused, std::istream& theStream)
		{
			commonItems::singleString econStr(theStream);
			economic_policy = econStr.getString();
		});
	registerKeyword(std::regex("trade_policy"), [this](const std::string& unused, std::istream& theStream)
		{
			commonItems::singleString tradeStr(theStream);
			trade_policy = tradeStr.getString();
		});
	registerKeyword(std::regex("religious_policy"), [this](const std::string& unused, std::istream& theStream)
		{
			commonItems::singleString relStr(theStream);
			religious_policy = relStr.getString();
		});
	registerKeyword(std::regex("citizenship_policy"), [this](const std::string& unused, std::istream& theStream)
		{
			commonItems::singleString citiStr(theStream);
			citizenship_policy = citiStr.getString();
		});
	registerKeyword(std::regex("war_policy"), [this](const std::string& unused, std::istream& theStream)
		{
			commonItems::singleString warStr(theStream);
			war_policy = warStr.getString();
		});
	registerKeyword(std::regex("[a-zA-Z0-9\\_.:]+"), commonItems::ignoreItem);

	parseStream(theStream);
}


V2::Party::Party(const std::string& name, const std::string& ideology)
: name(name), ideology(ideology)
{
	if (ideology == "conservative")
	{
		start_date = date("1820.1.1");
		end_date = date("2000.1.1");
		economic_policy = "interventionism";
		trade_policy = "protectionism";
		religious_policy = "moralism";
		citizenship_policy = "limited_citizenship";
		war_policy = "pro_military";
	}
	else if (ideology == "liberal")
	{
		start_date = date("1820.1.1");
		end_date = date("2000.1.1");
		economic_policy = "laissez_faire";
		trade_policy = "free_trade";
		religious_policy = "pluralism";
		citizenship_policy = "limited_citizenship";
		war_policy = "anti_military";
	}
	else if (ideology == "reactionary")
	{
		start_date = date("1820.1.1");
		end_date = date("2000.1.1");
		economic_policy = "state_capitalism";
		trade_policy = "protectionism";
		religious_policy = "moralism";
		citizenship_policy = "residency";
		war_policy = "jingoism";
	}
	else if (ideology == "socialist")
	{
		start_date = date("1849.1.1");
		end_date = date("2000.1.1");
		economic_policy = "state_capitalism";
		trade_policy = "free_trade";
		religious_policy = "secularized";
		citizenship_policy = "full_citizenship";
		war_policy = "anti_military";
	}
	else if (ideology == "communist")
	{
		start_date = date("1849.1.1");
		end_date = date("2000.1.1");
		economic_policy = "planned_economy";
		trade_policy = "protectionism";
		religious_policy = "pro_atheism";
		citizenship_policy = "full_citizenship";
		war_policy = "pro_military";
	}
	else if (ideology == "anarcho_liberal")
	{
		start_date = date("1830.1.1");
		end_date = date("2000.1.1");
		economic_policy = "laissez_faire";
		trade_policy = "free_trade";
		religious_policy = "pro_atheism";
		citizenship_policy = "full_citizenship";
		war_policy = "pro_military";
	}
	else if (ideology == "fascist")
	{
		start_date = date("1905.1.1");
		end_date = date("2000.1.1");
		economic_policy = "state_capitalism";
		trade_policy = "protectionism";
		religious_policy = "moralism";
		citizenship_policy = "residency";
		war_policy = "jingoism";
	}
	else
	{
		LOG(LogLevel::Warning) << "Unknown party ideology \"" << ideology << "\" for party \"" << name << '"';
	}
}
