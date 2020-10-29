#include "output.h"

std::ostream& V2::operator<<(std::ostream& output, const Event& event)
{
        if (event.provinces.empty())
        {
                return output;
        }
        output << "province_event={\n";
	output << "\tid = " << event.eventID << "\n";

	output << "\ttrigger = {\n";
        output << "\t\tyear = " << event.year << "\n";
        for (const auto& prov : event.provinces) {
                output << "\t\tprovince_id = " << prov << "\n";
        }
        output << "\t\tnot = { trade_goods = " << event.tradeGood << " }\n";
        if (event.triggerYear >= 0 ||
            event.inventions.size() + event.techs.size() > 0)
        {
          output << "\t\tOR = {\n";
          if (event.triggerYear >= 0) {
            output << "\t\t\tyear = " << event.triggerYear << "\n";
          }
          // Spherelings
          output << "\t\t\towner = {\n";
          output << "\t\t\t\tpart_of_sphere = yes\n";
          output << "\t\t\t\tsphere_owner = {\n";
          output << "\t\t\t\t\tor = {\n";
          for (const auto& invention : event.inventions) {
            output << "\t\t\t\t\t\tinvention = " << invention << "\n";
          }
          for (const auto& tech : event.techs) {
            output << "\t\t\t\t\t\t" << tech << " = 1\n";
          }
          output << "\t\t\t\t\t}\n";
          output << "\t\t\t\t}\n";
          output << "\t\t\t}\n";

          // Independents
          output << "\t\t\towner = {\n";
          output << "\t\t\t\tor = {\n";
          for (const auto& invention : event.inventions) {
            output << "\t\t\t\t\tinvention = " << invention << "\n";
          }
          for (const auto& tech : event.techs) {
            output << "\t\t\t\t\t" << tech << " = 1\n";
          }

          output << "\t\t\t\t}\n";
          output << "\t\t\t}\n";
          output << "\t\t}\n";
        }
        output << "\t}\n";

        output << "\tmean_time_to_happen = {\n";
        output << "\t\tmonths = " << event.mtth_months << "\n";
        output << "\t}\n";

        output << "\ttitle = \"" << event.title << "\"\n";
        output << "\tdesc = \"" << event.desc << "\"\n";
        output << "\toption = {\n";
        output << "\t\tname = \"" << event.buttonText << "\"\n";
        output << "\t\ttrade_goods = " << event.tradeGood << "\n";
        output << "\t}\n";
	output << "}\n";

	return output;
}
