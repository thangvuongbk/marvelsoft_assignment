#ifndef ORDER_PROPERTY_H_INCLUDED
#define ORDER_PROPERTY_H_INCLUDED

#include <map>
#include <string>
using namespace std;

#define BOOK "book"
#define TRADE "trade"
#define SYMBOL "symbol"
#define BID "bid"
#define ASK "ask"
#define COUNT "count"
#define QUANTITY "quantity"
#define PRICE "price"

//class OrderQueue {
//public:
//    std::string symbol;
//    std::string event_type; // book or trade
//    std::string order_type; // SELL or BUY
//    uint32_t order_count;
//    uint32_t order_quanity;
//    double order_price;
//
//    OrderQueue() {};
//    OrderQueue(const OrderQueue &other) {
//        symbol = other.symbol;
//        event_type = other.event_type;
//        order_type = other.order_type;
//        order_count = other.order_count;
//        order_quanity = other.order_quanity;
//        order_price = other.order_price;
//    }
//
//private:
//
//};


class OrderQueue2 {
public:
	typedef struct order_attr_ {
	    uint32_t order_count;
		uint32_t order_quanity;
		double price;
	} order_attr_t;

    uint32_t lineNo; // for debug purpose only
	std::string event_type; // book or trade
    std::string symbol;
	std::map<double, order_attr_t, greater<double>> bid; // BUY. first is price value
	std::map<double, order_attr_t> ask; // SELL. first is price value
	order_attr_t order_trade;

	OrderQueue2() {};
	OrderQueue2(const OrderQueue2& other) {
        event_type = other.event_type;
        symbol = other.symbol;
        bid = std::map<double, order_attr_t, greater<double>>();
        ask = std::map<double, order_attr_t>();
        bid = other.bid;
        ask = other.ask;
        order_trade = other.order_trade;
        lineNo = other.lineNo;
	}

    OrderQueue2& operator=(const OrderQueue2& other)
    {
        event_type = other.event_type;
        symbol = other.symbol;
        bid = std::map<double, order_attr_t, greater<double>>();
        ask = std::map<double, order_attr_t>();
        bid = other.bid;
        ask = other.ask;
        order_trade = other.order_trade;
        lineNo = other.lineNo;
        return *this;
    }
};

#endif // ORDER_PROPERTY_H_INCLUDED
