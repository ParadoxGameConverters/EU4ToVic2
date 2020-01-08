#ifndef POP_RATIO_H
#define POP_RATIO_H

#include <string>

namespace EU4
{
	class PopRatio
	{
		public:
			PopRatio(const std::string& _culture, const std::string& _religion);

			void decay(float diffInYears, const EU4::PopRatio& currentPop);
			void increase(float diffInYears);
			void convertFrom();
			void convertToCulture(const std::string& culture);
			void convertToReligion(const std::string& religion);
			void convertTo(const std::string& culture, const std::string& religion);

			std::string getCulture() const { return culture; }
			std::string getReligion() const { return religion; }
			double getUpperRatio() const { return upperRatio; }
			double getMiddleRatio() const { return middleRatio; }
			double getLowerRatio() const { return lowerRatio; }

		private:
			std::string culture;
			std::string religion;
			double upperRatio = 1.0;
			double middleRatio = 1.0;
			double lowerRatio = 1.0;
	};
}

#endif // POP_RATIO_H