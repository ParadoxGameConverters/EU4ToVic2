#include "Agreement.h"

V2::Agreement::Agreement(const std::string& _first, const std::string& _second, const std::string& _type, const date& _start_date):
	first(_first), second(_second),type(_type), start_date(_start_date) {}
