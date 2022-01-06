/*
QuoteManager Implementation File - Quoter.cpp

Made by Modou Niang
*/
#include "Quoter.h"
#include <limits>
#include <algorithm>
#include <iostream>
#include <time.h>

using namespace std;

namespace QuoteManager {
    unordered_map<string, Quote*> SimpleQuoteManager::quoteMapById() const {
        return quoteMapById_;
    }
    unordered_map<string, vector<Quote*>> SimpleQuoteManager::quoteMapBySymbol() const {
        return quoteMapBySymbol_;
    }

    void SimpleQuoteManager::AddOrUpdateQuote(Quote& quote){
        // Adding quote to quoteMapById_
        string quoteId = quote.id();
        this->quoteMapById_[quoteId] = &quote;

        // Checking whether quote is in the symbol map
        string quoteSymbol = quote.symbol();
        if (this->quoteMapBySymbol_.find(quoteSymbol) != this->quoteMapBySymbol_.end()) {
            this->quoteMapBySymbol_[quoteSymbol].push_back(&quote);
        } else {
            this->quoteMapBySymbol_[quoteSymbol] = vector<Quote*>{&quote};
        }

    }

    void SimpleQuoteManager::RemoveQuote(const string& id) {
        // Checking if item is in map
        if (this->quoteMapById_.find(id) != this->quoteMapById_.end()) {
            // Retrieving the quote information
            Quote* quotePtr = this->quoteMapById_[id];
            string symbol = quotePtr->symbol();

            // Erasing all the quotes of a given symbol
            this->quoteMapBySymbol_[symbol].clear();

            // Removing the quotes from the maps
            this->quoteMapBySymbol_.erase(symbol);
            this->quoteMapById_.erase(id);
        }
    }

    void SimpleQuoteManager::RemoveAllQuotes(const string& symbol){
        // Erasing all quotes of a given symbol
        for (Quote* currQuote : this->quoteMapBySymbol_[symbol]) {
            // Erase from quotes id map
            string id = currQuote->id();
            this->quoteMapById_.erase(id);
        }

        // Clearing the quotes vector for the symbol
        this->quoteMapBySymbol_[symbol].clear();
    }

    Quote* SimpleQuoteManager::GetBestQuoteWithAvailableVolume(const string& symbol) {
        // Checking whether the symbol is not within our Symbol book
        if (this->quoteMapBySymbol_.find(symbol) == this->quoteMapBySymbol_.end()) {
            return nullptr;
        }
       
        // Retrieve all the quotes of a symbol
        vector<Quote*> symbolQuotes = this->quoteMapBySymbol_[symbol];

        // Iterate through and find the one with the lowest price
        Quote* bestQuote = nullptr;
        double lowestPrice = std::numeric_limits<double>::max();
   
        for (size_t idx = 0; idx < symbolQuotes.size(); ++idx) {
            Quote* currQuote = symbolQuotes[idx];
        
            if(!this->isExpired(*currQuote) && currQuote->price() < lowestPrice) {
                bestQuote = currQuote;
                lowestPrice = currQuote->price();
            }
        }
        
        return bestQuote;
    }

    bool SimpleQuoteManager::isExpired(const Quote& quote) {
        // Retrieving expiration time
        tm* quoteExpirationTime = quote.expiration_date();

        // Check whether current time is passed the expiration time
        time_t t = std::time(0); 
        tm* currentTime = std::localtime(&t);

        return difftime(mktime(quoteExpirationTime), mktime(currentTime)) < 0; 
    }

    bool compareByPrice(const Quote* quote1, const Quote* quote2) {
        return quote1->price() < quote2->price();
    }


    TradeResult* SimpleQuoteManager::ExecuteTrade(const string& symbol,
                                                 uint32_t volumeRequested) {
        // Retrieves all the quotes of a symbol
        vector<Quote*> symbolQuotes = this->quoteMapBySymbol_[symbol];

        // Sort these quotes by best price to worst
        sort(symbolQuotes.begin(), symbolQuotes.end(), compareByPrice);
        
        // Iterate through quotes and try to fulfill order
        for (size_t idx = 0; idx < symbolQuotes.size(); ++idx) {
            Quote* quotePtr = symbolQuotes[idx];

            // Check whether available volume is greater than volume requested
            // If so, we fulfill order 
            // Else we decrease volume and check the next available quote
            if (!this->isExpired(*quotePtr) && 
                quotePtr->available_volume() >= volumeRequested) {

                uint16_t newVolume = quotePtr->available_volume() - volumeRequested;
                quotePtr->setNewVolume(newVolume);

                // Calculate volumeWeightedAveragePrice
                double volumeWeightedAveragePrice =  (quotePtr->price() * quotePtr->available_volume()) / quotePtr->available_volume();
        
                TradeResult* tradeResult = new TradeResult(quotePtr->id(),
                                                      quotePtr->symbol(),
                                                      volumeWeightedAveragePrice,
                                                      volumeRequested,
                                                      volumeRequested);

                return tradeResult;
            } else {
                uint16_t newVolume = volumeRequested - quotePtr->available_volume();
                
                quotePtr->setNewVolume(newVolume);
            }
        }

        return nullptr;
    }
} // namespace QuoteManager