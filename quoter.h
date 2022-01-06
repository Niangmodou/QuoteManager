/*
QuoteManager Header File - Quoter.h

Made by Modou Niang
*/
#ifndef QUOTER_H
#define QUOTER_H

#include <optional>
#include <chrono>
#include <unordered_map>
#include <string>
#include <vector>

namespace QuoteManager {
    struct Quote {
    private:
        std::string id_;
        std::string symbol_;
        double price_;
        uint32_t available_volume_;
        tm *expiration_date_;

    public:
        const std::string& id() const { return id_; }
        const std::string& symbol() const { return symbol_; }
        double price() const { return price_; }
        uint32_t available_volume() const { return available_volume_; }
        tm *expiration_date() const { return expiration_date_; }

        void setNewVolume(uint32_t newVolume) {available_volume_ = newVolume;}

        Quote(const std::string& id, const std::string& symbol, double price, uint32_t available_volume,
                tm *expiration_date) {
            this->id_ = id;
            this->symbol_ = symbol;
            this->price_ = price;
            this->available_volume_ = available_volume;
            this->expiration_date_ = expiration_date;
        }
    };

    struct TradeResult {
    private:
            std::string id_;
            std::string symbol_;
            double volume_weighted_average_price_;
            uint32_t volume_requested_;
            uint32_t volume_executed_;

    public:
            const std::string& id() const { return id_; }
            const std::string& symbol() const { return symbol_; }

        double volume_weighted_average_price() const {
            return volume_weighted_average_price_;
        }

        uint32_t volume_requested() const { return volume_requested_; }
        uint32_t volume_executed() const { return volume_executed_; }

        TradeResult(const std::string& id, const std::string& symbol, double volume_weighted_average_price,
                    uint32_t volume_requested, uint32_t volume_executed) {
            this->id_ = id;
            this->symbol_ = symbol;
            this->volume_weighted_average_price_ = volume_weighted_average_price;
            this->volume_requested_ = volume_requested;
            this->volume_executed_ = volume_executed;
        }
    };

    /**
     * Please create your own quote manager, which implements the IQuoteManager
     * "interface".
     *
     * Do not change the interface, or any other classes.
     *
     * Please adhere to good OOP concepts, and create whatever support code you feel
     * is necessary.
     *
     * Efficiency counts: think about what data structures you use and how each
     * method will perform.
     *
     * Though not required, feel free to includes any notes on any areas of this
     * interface that you would improve, or which you feel don't adhere to good
     * design concepts or implementation practices.
     */
    class IQuoteManager {
    public:
        // Add or update the quote (specified by Id) in symbol's book.
        // If quote is new or no longer in the book, add it. Otherwise update it to
        // match the given price, volume, and symbol.
        virtual void AddOrUpdateQuote(Quote&) = 0;

        // Remove quote by Id, if quote is no longer in symbol's book do nothing.
        virtual void RemoveQuote(const std::string&) = 0;

        // Remove all quotes on the specifed symbol's book.
        virtual void RemoveAllQuotes(const std::string&) = 0;

        // Get the best (i.e. lowest) price in the symbol's book that still has
        // available volume. If there is no quote on the symbol's book with available
        // volume, return null. Otherwise return a Quote object with all the fields
        // set. Don't return any quote which is past its expiration time, or has been
        // removed.
        virtual Quote* GetBestQuoteWithAvailableVolume(const std::string&) = 0;

        // Request that a trade be executed. For the purposes of this interface,
        // assume that the trade is a request to BUY, not sell. Do not trade on
        // expired quotes. To execute a trade:
        //   * Search available quotes of the specified symbol from best price to
        //   worst price.
        //   * Until the requested volume has been filled, use as much available
        //   volume as necessary (up to what is available) from each quote,
        //     subtracting the used amount from the available amount.
        // For example, we have two quotes:
        //   {Price: 1.0, Volume: 1,000, AvailableVolume: 750}
        //   {Price: 2.0, Volume: 1,000, AvailableVolume: 1,000}
        // After calling once for 500 volume, the quotes are:
        //   {Price: 1.0, Volume: 1,000, AvailableVolume: 250}
        //   {Price: 2.0, Volume: 1,000, AvailableVolume: 1,000}
        // And After calling this a second time for 500 volume, the quotes are:
        //   {Price: 1.0, Volume: 1,000, AvailableVolume: 0}
        //   {Price: 2.0, Volume: 1,000, AvailableVolume: 750}
        virtual TradeResult* ExecuteTrade(const std::string&, uint32_t) = 0;
    }; // IQuoteManager

    class SimpleQuoteManager : public IQuoteManager {
    private:
        std::unordered_map<std::string, Quote*> quoteMapById_;
        std::unordered_map<std::string, std::vector<Quote*>> quoteMapBySymbol_;
    public: 
        void AddOrUpdateQuote(Quote&);
        void RemoveQuote(const std::string&);
        void RemoveAllQuotes(const std::string&);
        Quote* GetBestQuoteWithAvailableVolume(const std::string&);
        TradeResult* ExecuteTrade(const std::string&, uint32_t);

        // Getters
        std::unordered_map<std::string, Quote*> quoteMapById() const;
        std::unordered_map<std::string, std::vector<Quote*>> quoteMapBySymbol() const;

        // Check whether quote has expired
        bool isExpired(const Quote&);

        
    }; // SimpleQuoteManager

} // namespace QuoteManager

#endif // QUOTER_H