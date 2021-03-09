#ifndef EVENTS_H
#define EVENTS_H

#include "Parser.h"
#include <set>

namespace V2
{
class Events: commonItems::parser
{
  public:
	Events();
	explicit Events(const std::string& filename);
	explicit Events(const std::set<std::string>& evtFiles);

	void updateEvents(const std::map<std::string, std::string>& stateMap, const std::map<int, int>& provinceMap);
	void updateStateIDs(std::string& theEvent, std::regex expression, std::map<std::string, std::string> map);
	void updateIDs(std::string& theEvent, std::regex expression, int captureGroup, std::map<int, int> map);

	[[nodiscard]] const auto& getEvents() const { return events; }

  private:
	void registerKeys();
	std::vector<std::string> events;
};
} // namespace V2

#endif // EVENTS_H