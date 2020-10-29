#ifndef EVENT_H
#define EVENT_H

#include <random>
#include <string>
#include <unordered_set>
#include <vector>
#include "../../V2World/Province/Province.h"
#include "Parser.h"

namespace V2
{

        class Event : commonItems::parser
        {
        public:
                Event(std::istream& theStream);
                void
                shuffle(const std::map<int, std::shared_ptr<Province>>& provMap,
                        std::default_random_engine& shuffler);
                friend std::ostream& operator<<(std::ostream& output,
                                                const Event& event);

        private:
                // Vicky event ID.
                int eventID;
                // Trade good to generate.
                std::string tradeGood;
                // Mean time to happen in months.
                int mtth_months;
                // Key strings, e.g. "EVTNAME1234", "EVTDESC1234".
                std::string title;
                std::string desc;
                std::string buttonText;
                // Year the event can happen.
                int year;
                // Provinces to trigger in (randomly generated).
                std::vector<int> provinces;
                // Resources to exclude from the list of candidates.
                std::unordered_set<std::string> exclude;
                // Year to trigger event without inventions or techs.
                int triggerYear;
                // Inventions that allow the event to trigger early,
                // for example "invention = daimlers_automobile".
                std::unordered_set<std::string> inventions;
                // Technologies that allow the event to trigger early,
                // e.g. "infiltration = 1".
                std::unordered_set<std::string> techs;
        };

} // namespace V2

#endif // EVENT_H
