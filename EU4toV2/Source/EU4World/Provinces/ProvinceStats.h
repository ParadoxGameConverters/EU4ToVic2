/*Copyright(c) 2019 The Paradox Game Converters Project

Permission is hereby granted, free of charge, to any person obtaining a copy
of this software and associated documentation files(the "Software"), to deal
in the Software without restriction, including without limitation the rights
to use, copy, modify, merge, publish, distribute, sublicense, and / or sell
copies of the Software, and to permit persons to whom the Software is
furnished to do so, subject to the following conditions :

The above copyright notice and this permission notice shall be included in
all copies or substantial portions of the Software.

THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT.IN NO EVENT SHALL THE
AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN
THE SOFTWARE. */



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