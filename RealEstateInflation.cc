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

    std::pair<double, double> calculateLTCGOldScheme(int selling_year)
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

    std::pair<double, double> calculateLTCGNewScheme(int selling_year)
    {
        double selling_price = calculateSellingPrice(selling_year);
        double actual_profit = selling_price - initial_cost_price;
        double ltcg = actual_profit * 0.125;
        return {selling_price, ltcg};
    }

    void compareTaxSchemes(int selling_year)
    {
        auto [selling_price_old, ltcg_old] = calculateLTCGOldScheme(selling_year);
        auto [selling_price_new, ltcg_new] = calculateLTCGNewScheme(selling_year);

        std::cout << "Old Scheme LTCG: Rs " << ltcg_old << std::endl;
        std::cout << "New Scheme LTCG: Rs " << ltcg_new << std::endl;

        if (ltcg_old > ltcg_new)
        {
            std::cout << "Old scheme leads to higher taxes by Rs " << (ltcg_old - ltcg_new) << std::endl;
        }
        else
        {
            std::cout << "New scheme leads to higher taxes by Rs " << (ltcg_new - ltcg_old) << std::endl;
        }
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
    std::string csv_file = "rates.csv"; // CSV file containing rates from 2001 to 2030

    RealEstateInvestment investment(initial_cost_price, purchase_year);
    investment.loadRatesFromCSV(csv_file);

    int selling_year;
    std::cout << "Enter the year of selling: ";
    std::cin >> selling_year;

    investment.compareTaxSchemes(selling_year);

    return 0;
}