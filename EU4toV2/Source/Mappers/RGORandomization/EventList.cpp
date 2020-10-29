#include "EventList.h"
#include "../../Configuration.h"
#include "ParserHelpers.h"

mappers::EventList::EventList()
{
        registerKeyword("event", [this](const std::string& key, std::istream& theStream) 
		{
                        const V2::Event newEvent(theStream);
			events_.push_back(newEvent);
		}
	);
	registerRegex("[a-zA-Z0-9\\_.:]+", commonItems::ignoreItem);

	parseFile("configurables/rgo_events.txt");
	clearRegisteredKeywords();
}

const std::vector<V2::Event>& mappers::EventList::getEvents() const
{
        return events_;
}

void mappers::EventList::shuffle(
    const std::map<int, std::shared_ptr<V2::Province>>& provMap,
    std::default_random_engine& shuffler)
{
        for (auto& event : events_)
        {
                event.shuffle(provMap, shuffler);
        }
}
