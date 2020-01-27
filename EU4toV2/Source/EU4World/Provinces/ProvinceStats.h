#ifndef PROVINCE_STATS_H
#define PROVINCE_STATS_H

namespace EU4
{
	class ProvinceStats
	{
	public:
		[[nodiscard]] auto getGoodsProduced() const { return goodsProduced; }
		[[nodiscard]] auto getPrice() const { return price; }
		[[nodiscard]] auto getTradeEfficiency() const { return tradeEfficiency; }
		[[nodiscard]] auto getProductionEfficiency() const { return productionEfficiency; }
		[[nodiscard]] auto getTradeValue() const { return tradeValue; }
		[[nodiscard]] auto getTotalTradeValue() const { return totalTradeValue; }

		[[nodiscard]] auto getBaseTax() const { return baseTax; }
		[[nodiscard]] auto getBuildingsIncome() const { return buildingsIncome; }
		[[nodiscard]] auto getTaxEfficiency() const { return taxEfficiency; }
		[[nodiscard]] auto getTotalTaxIncome() const { return totalTaxIncome; }

		void setGoodsProduced(const double _goodsProduced) { goodsProduced = _goodsProduced; }
		void setPrice(const double _price) { price = _price; }
		void setTradeEfficiency(const double _tradeEfficiency) { tradeEfficiency = _tradeEfficiency; }
		void setProductionEfficiency(const double _productionEfficiency) { productionEfficiency = _productionEfficiency; }
		void setTradeValue(const double _tradeValue) { tradeValue = _tradeValue; }
		void setTotalTradeValue(const double _totalTradeValue) { totalTradeValue = _totalTradeValue; }

		void setBaseTax(const double _baseTax) { baseTax = _baseTax; }
		void setBuildingsIncome(const double _buildingsIncome) { buildingsIncome = _buildingsIncome; }
		void setTaxEfficiency(const double _taxEfficiency) { taxEfficiency = _taxEfficiency; }
		void setTotalTaxIncome(const double _totalTaxIncome) { totalTaxIncome = _totalTaxIncome; }

	private:
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
}

#endif // PROVINCE_STATS_H