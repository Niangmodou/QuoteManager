/*
QuoteManager Implementation File - Quoter.cpp

Made by Modou Niang
*/
#include "Quoter.h"
#include <limits>
#include <algorithm>
#include <time.h>

using namespace std;

namespace QuoteManager {
    void SimpleQuoteManager::AddOrUpdateQuote(Quote& quote){
        // Adding quote to quoteMapById
        string quoteId = quote.id();
        this->quoteMapById[quoteId] = &quote;

        // Adding quote to quoteMapBySymbol
        string quoteSymbol = quote.symbol();
        this->quoteMapBySymbol[quoteSymbol] = &quote;
    }

    void SimpleQuoteManager::RemoveQuote(const string& id) {
        // Checking if item is in map
        if (this->quoteMapById.find(id) != this->quoteMapById.end()) {
            // Retrieving the quote information
            Quote* quotePtr = this->quoteMapById[id];
            string symbol = quotePtr->symbol();

            // Erasing all the quotes of a given symbol
            this->quoteMapBySymbol[symbol].clear();
            this->quoteMapById[id].clear();
        }
    }

    void SimpleQuoteManager::RemoveAllQuotes(const string& symbol){
        Quote* quotePtr = this->quoteMapBySymbol[symbol];
        string quoteId = quotePtr->id();

       // Erasing the quote from both maps
        this->quoteMapBySymbol.erase(symbol);
        this->quoteMapById.erase(quoteId);
    }

    Quote SimpleQuoteManager::GetBestQuoteWithAvailableVolume(const string& symbol) {
        // Checking whether the symbol is not within our Symbol book
        if (this->quoteMapBySymbol.find(symbol) == this->quoteMapBySymbol.end()) {
            return;
        }

        // Retrieve all the quotes of a symbol
        vector<Quote*> symbolQuotes = this->quoteMapBySymbol[symbol];

        // Iterate through and find the one with the lowest price
        Quote bestQuote;;
        double lowestPrice = std::numeric_limits<double>::max();
        for (size_t idx = 0; idx < symbolQuotes.size(); ++idx) {
            Quote* currQuote = symbolQuotes[idx];
    
            if(!this->isExpired(currQuote) && currQuote->price() < lowestPrice) {
                bestQuote = *currQuote;
                lowestPrice = currQuote->price();
            }
        }
        
        return bestQuote;
    }

    bool SimpleQuoteManager::isExpired(Quote& quote) {
        // Retrieving expiration time
        tm* quoteExpirationTime = quote.expiration_date();

        // Check whether current time is passed the expiration time
        time_t t = std::time(0);   // get time now
        tm* currentTime = std::localtime(&t);

        return difftime(quoteExpiratonTime, currentTime); 
    }

    bool compareByPrice(const Quote& quote1, const Quote& quote2) {
        return quote1.price() < quote2.price();
    }


    TradeResult SimpleQuoteManager::ExecuteTrade(const string& symbol,
                                                 uint32_t volumeRequested) {
        // Retrieves all the quotes of a symbol
        vector<Quote*> symbolQuotes = this->quoteMapBySymbol[symbol];

        // Sort these quotes by best price to worst
        sort(symbolQuotes.begin(), symbolQuotes.end(), compareByPrice)
        
        // Iterate through quotes and try to fulfill order
        for (size_t idx = 0; idx < symbolQuotes.size(); ++idx) {
            Quote* quotePtr = symbolQuotes[idx];

            // Check whether available volume is greater than volume requested
            // If so, we fulfill order 
            // Else we decrease volume and check the next available quote
            if (!this->isExpired(quotePtr) && 
                quotePtr->available_volume() >= volumeRequested) {

                uint16_t newVolume = quotePtr->available_volume() - volumeRequested;
                quotePtr->setNewVolume(newVolume);

                TradeResult tradeResult = TradeResult(quotePtr->id(),
                                                      quotePtr->symbol(),
                                                      volumeRequested,
                                                      volumeRequested);

                return tradeResult;
            } else {
                uint16_t newVolume = volumeRequested - quotePtr->available_volume();
                
                quotePtr->setNewVolume(newVolume);
            }
        }

        return NULL;
    }
} // namespace QuoteManager