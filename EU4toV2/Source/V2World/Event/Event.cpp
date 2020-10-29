#include "Event.h"

#include <random>
#include "Log.h"
#include "ParserHelpers.h"

V2::Event::Event(std::istream& theStream) {
        triggerYear = -1;
        registerKeyword("id", [this](const std::string unused, std::istream& theStream)
               {
                       const commonItems::singleInt eventIdInt(theStream);
                       eventID = eventIdInt.getInt();
               });
        registerKeyword("trigger_year", [this](const std::string unused, std::istream& theStream)
               {
                       const commonItems::singleInt triggerYearInt(theStream);
                       triggerYear = triggerYearInt.getInt();
               });
        registerKeyword("year", [this](const std::string unused, std::istream& theStream)
               {
                       const commonItems::singleInt yearInt(theStream);
                       year = yearInt.getInt();
               });
        registerKeyword("trade_goods", [this](const std::string unused, std::istream& theStream)
               {
                       const commonItems::singleString goodsString(theStream);
                       tradeGood = goodsString.getString();
               });
        registerKeyword("mtth", [this](const std::string unused, std::istream& theStream)
               {
                       const commonItems::singleInt mtthInt(theStream);
                       mtth_months = mtthInt.getInt();
               });
        registerKeyword("title", [this](const std::string unused, std::istream& theStream)
               {
                       const commonItems::singleString titleString(theStream);
                       title = titleString.getString();
               });
        registerKeyword("desc", [this](const std::string unused, std::istream& theStream)
               {
                       const commonItems::singleString descString(theStream);
                       desc = descString.getString();
               });
        registerKeyword("button", [this](const std::string unused, std::istream& theStream)
               {
                       const commonItems::singleString buttonString(theStream);
                       buttonText = buttonString.getString();
               });
        registerKeyword("exclude", [this](const std::string unused, std::istream& theStream)
               {
                       const commonItems::singleString excludeString(theStream);
                       exclude.insert(excludeString.getString());
               });
        registerKeyword("invention", [this](const std::string unused, std::istream& theStream)
               {
                       const commonItems::singleString inventionString(theStream);
                       inventions.insert(inventionString.getString());
               });
        registerKeyword("tech", [this](const std::string unused, std::istream& theStream)
               {
                       const commonItems::singleString techString(theStream);
                       techs.insert(techString.getString());
               });
        int numProvs = 0;
        registerKeyword("num_provinces", [&numProvs](const std::string unused, std::istream& theStream)
               {
                       const commonItems::singleInt numProvsInt(theStream);
                       numProvs = numProvsInt.getInt();
               });
	registerRegex("[a-zA-Z0-9\\_.:]+", commonItems::ignoreItem);
	parseStream(theStream);
	clearRegisteredKeywords();

        // Placeholder number until we can shuffle.
        for (int i = 0; i < numProvs; ++i)
        {
                provinces.push_back(-1);
        }
}

void V2::Event::shuffle(const std::map<int, std::shared_ptr<Province>>& provMap,
                   std::default_random_engine& shuffler)
{
  std::vector<int> candidates;
  for (const auto& prov : provMap)
  {
          if (exclude.find(prov.second->getRgoType()) != exclude.end())
          {
                  continue;
          }
          candidates.push_back(prov.first);
  }
  if (candidates.empty())
  {
          LOG(LogLevel::Warning)
              << "No candidates for event " << eventID << ", skipping shuffle.";
          provinces.clear();
          return;
  }

  std::shuffle(candidates.begin(), candidates.end(), shuffler);
  if (provinces.size() > candidates.size())
  {
          LOG(LogLevel::Warning)
              << "Found only " << candidates.size() << " candidates for event "
              << eventID << ", cannot generate intended " << provinces.size()
              << " events.";
          provinces.resize(candidates.size());
  }
  for (unsigned int i = 0; i < provinces.size(); i++)
  {
          provinces[i] = candidates[i];
  }
}
