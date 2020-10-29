#ifndef EVENT_LIST_H
#define EVENT_LIST_H

#include <memory>
#include <random>
#include <vector>
#include "../../V2World/Province/Province.h"
#include "../../V2World/Event/Event.h"
#include "Parser.h"

namespace mappers
{
        // Class to parse and manage a list of province events with
        // random targets.
        class EventList : commonItems::parser
        {
        public:
                EventList();
                const std::vector<V2::Event>& getEvents() const;
                void shuffle(
                    const std::map<int, std::shared_ptr<V2::Province>>& provMap,
                    std::default_random_engine& shuffler);

              private:
                std::vector<V2::Event> events_;
        };

} // namespace mappers

#endif // EVENT_LIST_H
