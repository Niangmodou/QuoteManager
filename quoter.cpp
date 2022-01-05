#include "Quoter.h"

namespace QuoteManager {
    void SimpleQuoteManager::AddOrUpdateQuote(Quote &quote){}

    void SimpleQuoteManager::RemoveQuote(const string& id) {}

    void SimpleQuoteManager::RemoveAllQuotes(const string& symbol){}

    Quote SimpleQuoteManager::GetBestQuoteWithAvailableVolume(const string& symbol){}

    SimpleQuoteManager::TradeResult ExecuteTrade(const string& symbol, uint32_t volume_requested){}
} // namespace QuoteManager