#ifndef PROVINCE_STATS_H
#define PROVINCE_STATS_H

namespace EU4
{
struct ProvinceStats // This is a debug struct for logging province stats into a csv.
{
	double goodsProduced = 0;
	double price = 0;
	double tradeEfficiency = 0;
	double productionEfficiency = 0;
	double tradeValue = 0;
	double totalTradeValue = 0;

	double baseTax = 0;
	double buildingsIncome = 0;
	double taxEfficiency = 0;
	double totalTaxIncome = 0;
};
} // namespace EU4

#endif // PROVINCE_STATS_H