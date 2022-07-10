#ifndef ORDER_PROPERTY_H_INCLUDED
#define ORDER_PROPERTY_H_INCLUDED

#include <string>

class OrderQueue {
public:
    std::string symbol;
    std::string event_type; // book or trade
    std::string order_type; // SELL or BUY
    uint32_t order_count;
    uint32_t order_quanity;
    double order_price;

    OrderQueue() {};
    OrderQueue(const OrderQueue &other) {
        symbol = other.symbol;
        event_type = other.event_type;
        order_type = other.order_type;
        order_count = other.order_count;
        order_quanity = other.order_quanity;
        order_price = other.order_price;
    }

private:

};

#endif // ORDER_PROPERTY_H_INCLUDED
