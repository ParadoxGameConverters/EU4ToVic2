#ifndef PROVINCE_STATS_H
#define PROVINCE_STATS_H

namespace EU4
{
	class ProvinceStats
	{
	public:
		double getGoodsProduced() const { return goodsProduced; }
		double getPrice() const { return price; }
		double getTradeEfficiency() const { return tradeEfficiency; }
		double getProductionEfficiency() const { return productionEfficiency; }
		double getTradeValue() const { return tradeValue; }
		double getTotalTradeValue() const { return totalTradeValue; }

		double getBaseTax() const { return baseTax; }
		double getBuildingsIncome() const { return buildingsIncome; }
		double getTaxEfficiency() const { return taxEfficiency; }
		double getTotalTaxIncome() const { return totalTaxIncome; }

		void setGoodsProduced(double _goodsProduced) { goodsProduced = _goodsProduced; }
		void setPrice(double _price) { price = _price; }
		void setTradeEfficiency(double _tradeEfficiency) { tradeEfficiency = _tradeEfficiency; }
		void setProductionEfficiency(double _productionEfficiency) { productionEfficiency = _productionEfficiency; }
		void setTradeValue(double _tradeValue) { tradeValue = _tradeValue; }
		void setTotalTradeValue(double _totalTradeValue) { totalTradeValue = _totalTradeValue; }

		void setBaseTax(double _baseTax) { baseTax = _baseTax; }
		void setBuildingsIncome(double _buildingsIncome) { buildingsIncome = _buildingsIncome; }
		void setTaxEfficiency(double _taxEfficiency) { taxEfficiency = _taxEfficiency; }
		void setTotalTaxIncome(double _totalTaxIncome) { totalTaxIncome = _totalTaxIncome; }

	private:
		double goodsProduced = 0.0;
		double price = 0.0;
		double tradeEfficiency = 0.0;
		double productionEfficiency = 0.0;
		double tradeValue = 0.0;
		double totalTradeValue = 0.0;

		double baseTax = 0.0;
		double buildingsIncome = 0.0;
		double taxEfficiency = 0.0;
		double totalTaxIncome = 0.0;
	};
}

#endif // PROVINCE_STATS_H