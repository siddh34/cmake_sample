#include <iostream>
#include <fstream>
#include <sstream>
#include <string>
#include <map>

class RealEstateInvestment
{
private:
    double initial_cost_price;
    int purchase_year;
    std::map<int, double> inflation_rates;
    std::map<int, double> property_growth_rates;

public:
    RealEstateInvestment(double initial_cost_price, int purchase_year)
        : initial_cost_price(initial_cost_price), purchase_year(purchase_year) {}

    void loadRatesFromCSV(const std::string &csv_file)
    {
        std::ifstream file(csv_file);
        std::string line, word;
        if (file.is_open())
        {
            std::getline(file, line);
            while (std::getline(file, line))
            {
                std::stringstream ss(line);
                int year;
                double inflation_rate, property_growth_rate;
                std::getline(ss, word, ',');
                year = std::stoi(word);
                std::getline(ss, word, ',');
                inflation_rate = std::stod(word);
                std::getline(ss, word, ',');
                property_growth_rate = std::stod(word);
                inflation_rates[year] = inflation_rate;
                property_growth_rates[year] = property_growth_rate;
            }
            file.close();
        }
        else
        {
            std::cerr << "Unable to open file";
        }
    }

    double calculateSellingPrice(int selling_year)
    {
        double price = initial_cost_price;
        for (int year = purchase_year + 1; year <= selling_year; ++year)
        {
            double growth_rate = property_growth_rates[year];
            price *= (1 + growth_rate / 100);
        }
        return price;
    }

    std::pair<double, double> calculateLTCG(int selling_year)
    {
        double selling_price = calculateSellingPrice(selling_year);
        double inflation_adjusted_price = initial_cost_price;
        for (int year = purchase_year + 1; year <= selling_year; ++year)
        {
            double inflation_rate = inflation_rates[year];
            inflation_adjusted_price *= (1 + inflation_rate / 100);
        }
        double actual_profit = selling_price - inflation_adjusted_price;
        double ltcg = actual_profit * 0.20;
        return {selling_price, ltcg};
    }
};

int main()
{

    double initial_cost_price;
    std::cout << "Enter the initial cost price: ";
    std::cin >> initial_cost_price;

    int purchase_year;
    std::cout << "Enter the year of purchase: ";
    std::cin >> purchase_year;

    std::string csv_file = "inflation_property_growth.csv";

    RealEstateInvestment investment(initial_cost_price, purchase_year);
    investment.loadRatesFromCSV(csv_file);
    int selling_year;
    std::cout << "Enter the year of selling: ";
    std::cin >> selling_year;

    auto [selling_price, ltcg] = investment.calculateLTCG(selling_year);

    std::cout << "Estimated Selling Price: Rs " << selling_price << std::endl;
    std::cout << "Long-term Capital Gains Tax (LTCG): Rs " << ltcg << std::endl;

    return 0;
}