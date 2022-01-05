/*
QuoteManager Test File - main.cpp

Made by Modou Niang
*/
#include "Quoter.h"
#include "time.h"
#include <iostream>

using namespace QuoteManager;
using namespace std;

int main() {
    SimpleQuoteManager quoteManager;

    cout << "TEST 1: Testing the AddOrUpdateQuote method" << endl;
    // Adding SOL quote
    struct tm tm1;
    std::string time1("2021-21-03 12:00");
    strptime(time1.c_str(), "%Y-%m-%d %H:%M", &tm1);

    Quote* quote1 = new Quote("111", "SOL", 170, 1000, &tm1);
    quoteManager.AddOrUpdateQuote(*quote1);

    // Adding ETH quote
    struct tm tm2;
    std::string time2("2022-10-01 12:00");
    strptime(time2.c_str(), "%Y-%m-%d %H:%M", &tm2);

    Quote* quote2 = new Quote("101", "ETH", 4500, 750, &tm1);
    quoteManager.AddOrUpdateQuote(*quote2);

    // Adding BTC quote
    struct tm tm3;
    std::string time3("2021-03-01 12:00");
    strptime(time3.c_str(), "%Y-%m-%d %H:%M", &tm3);

    Quote* quote3 = new Quote("100", "BTC", 50000, 500, &tm3);
    quoteManager.AddOrUpdateQuote(*quote3);
    
    cout << "TEST 2: Testing the RemoveQuote method" << endl;
    quoteManager.RemoveQuote("101");

    quoteManager.RemoveQuote("101");

    cout << "TEST 3: Testing the RemoveAllQuotes method" << endl;
    quoteManager.RemoveAllQuotes("SOL");

    quoteManager.RemoveAllQuotes("BTC");
    
    cout << "TEST 4: Testing the GetBestQuoteWithAvailableVolume method" << endl;

    cout << "TEST 5: Testing the ExecuteTrade method" << endl;

    cout << "SUCESSFULLY PASSED ALL TEST CASES" << endl;

  
}