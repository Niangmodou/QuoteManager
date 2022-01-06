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
    cout << "TEST 0: Creating the QuoteManager object" << endl;
    SimpleQuoteManager quoteManager;

    cout << "TEST 1: Testing the AddOrUpdateQuote method" << endl;
    // Adding SOL quote
    struct tm tm1;
    std::string time1("2022-04-01 12:00");
    strptime(time1.c_str(), "%Y-%m-%d %H:%M", &tm1);

    Quote* quote1 = new Quote("111", "SOL", 170, 1000, &tm1);
    quoteManager.AddOrUpdateQuote(*quote1);

    // Adding SOL quote
    struct tm tm2;
    std::string time2("2022-10-01 12:00");
    strptime(time2.c_str(), "%Y-%m-%d %H:%M", &tm2);

    Quote* quote2 = new Quote("101", "SOL", 175, 1000, &tm2);
    quoteManager.AddOrUpdateQuote(*quote2);

    // Adding BTC quote
    struct tm tm3;
    std::string time3("2021-03-01 12:00");
    strptime(time3.c_str(), "%Y-%m-%d %H:%M", &tm3);

    Quote* quote3 = new Quote("100", "BTC", 50000, 500, &tm3);
    quoteManager.AddOrUpdateQuote(*quote3);

    // Checking whether the quotes were correctly added 
    if (quoteManager.quoteMapBySymbol().find("SOL") == quoteManager.quoteMapBySymbol().end()) {
        cout << "FAILED TO ADD SOL" << endl;
    } else {
        cout << "PASS" << endl;
    }

    // Checking whether the quotes were correctly added 
    if (quoteManager.quoteMapBySymbol().find("BTC") == quoteManager.quoteMapBySymbol().end()) {
        cout << "FAILED TO ADD BTC" << endl;
    } else {
        cout << "PASS" << endl;
    }
    
    cout << "TEST 2: Testing the RemoveQuote method" << endl;
    quoteManager.RemoveQuote("111");
    quoteManager.RemoveQuote("111");

    // Check whether the quote was correctly removed
    if (quoteManager.quoteMapById().find("111") != quoteManager.quoteMapById().end()) {
        cout << "FAILED TO REMOVE 111" << endl;
    } else {
        cout << "PASS" << endl;
    }

    cout << "TEST 3: Testing the RemoveAllQuotes method" << endl;
    quoteManager.RemoveAllQuotes("SOL");

    quoteManager.RemoveAllQuotes("BTC");

    // Checkinng whether all quotes of the symbol's have been removed
    if (quoteManager.quoteMapBySymbol()["SOL"].size() != 0) {
        cout << "FAILED TO REMOVE ALL QUOTES of SOL";
    } else {
        cout << "PASS" << endl;
    }

    if (quoteManager.quoteMapBySymbol()["BTC"].size() != 0) {
        cout << "FAILED TO REMOVE ALL QUOTES of BTC";
    } else {
        cout << "PASS" << endl;
    }

    cout << "TEST 4: Testing the GetBestQuoteWithAvailableVolume method" << endl;
    // Adding back the quotes
    quoteManager.AddOrUpdateQuote(*quote1);
    quoteManager.AddOrUpdateQuote(*quote2);
    quoteManager.AddOrUpdateQuote(*quote3);

    Quote* bestQuote = quoteManager.GetBestQuoteWithAvailableVolume("SOL");
    if(bestQuote == nullptr || bestQuote->price() != 175) cout << "FAILED TO FIND BEST QUOTE"  << endl;
    else cout << "PASS" << endl;

    cout << "TEST 5: Testing the ExecuteTrade method" << endl;

    TradeResult* result1 = quoteManager.ExecuteTrade("SOL", 750);
    TradeResult* result2 = quoteManager.ExecuteTrade("BTC", 1000);

    if (result1 == nullptr) cout << "FAILED TO PRODUCE TradeResult" << endl;
    else cout << "PASS" << endl;

    if (result2 != nullptr) cout << "FAILED TO RETURN NULL" << endl;
    else cout << "PASS" << endl;

    cout << "SUCESSFULLY PASSED ALL TEST CASES" << endl;

  
}