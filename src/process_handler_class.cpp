
#include <sstream>
#include "process_handler_class.h"

using namespace std;

ProcessHandler::ProcessHandler() : m_prev_order(new OrderQueue2)
{
    m_prev_order->symbol = "";
    m_prev_order->event_type = "";
    m_prev_order->bid.clear();
    m_prev_order->ask.clear();
    m_prev_order->order_trade.order_count = 0;
    m_prev_order->order_trade.order_quanity = 0;
    m_prev_order->order_trade.price = 0.0;
}

ProcessHandler::~ProcessHandler()
{
    //dtor
}

void ProcessHandler::InitProcessThread()
{
    //cout << "Entering func: " << __func__ << endl;
    //processApp_pth = new thread([&]() noexcept {this->handleRequestWorker();});
    processApp_pth = new thread(&ProcessHandler::handleRequestWorker, this);

    if(processApp_pth->joinable()) {
        auto myid = processApp_pth->get_id();
        stringstream ss;
        ss << myid;
        thread_id_str = ss.str();
        cout << "process_thread_id: " << processApp_pth->get_id() << " string: " <<thread_id_str << endl;
        processApp_pth->detach();
    }

    //std::this_thread::sleep_for(std::chrono::seconds(1));
    //cout << "Leaving func: " << __func__ << endl;
    //return 0;

}

bool ProcessHandler::ProcessRequestArrived(OrderQueue2 order_queue)
{
    //cout << "Entering func: " << __func__ << endl;
    OrderQueue2 *_order_queue = new OrderQueue2(order_queue);
    m_order_queue.enqueue(_order_queue);
    //cout << "[Enqueue] Current Size: " << m_order_queue.queue_size() << endl;
    //cout << "Leaving func: " << __func__ << endl;
    return 0;
}

void ProcessHandler::handleRequestWorker()
{
    //cout << "Entering func: " << __func__ << endl;
    while(1) {
        if(m_order_queue.queue_size() > 0) {
            //cout << "\n[Dequeue] Current Size: " << m_order_queue.queue_size() << endl;
            OrderQueue2* get_order = m_order_queue.dequeue();

            /* Handle for BOOK event */
            if(get_order->event_type == BOOK) {
                //cout << "\n---Handler BID order list received\n";
                for(auto &it : get_order->bid) {
                    //cout << "price:" << it.first << "|count:" << it.second.order_count << "|quantity:" << it.second.order_quanity << endl;
                    auto find_change = m_prev_order->bid.find(it.first);
                    // if this price is not existing in the previous book
                    if(find_change == m_prev_order->bid.end()) {
                        auto find_in_trade = m_trade_map.find(it.first);
                        if(find_in_trade != m_trade_map.end()) {
                        // case 1: Quantity buy is greater than Quantity SELL
                            cout << get_order->lineNo << ": AGGRESSIVE BUY " << find_in_trade->second + it.second.order_quanity << "@" << it.first << endl;
                            // Remove this item from the map
                            m_trade_map.erase(it.first);
                        }
                        // case 2: This is new order buy
                        else cout << get_order->lineNo << ": PASSIVE_ BUY " << it.second.order_quanity << "@" << it.first << endl;
                    }
                    else {
                        if(find_change->second.order_quanity < it.second.order_quanity) {
                            //if(find_change->second.order_count < it.second.order_count) {
                                cout << get_order->lineNo << ": PASSIVE BUY " << (it.second.order_quanity - find_change->second.order_quanity)<< "@" << it.first << endl;
                            //}
                        }
                        else if(find_change->second.order_quanity > it.second.order_quanity){
                            // check if having a trade in between
                            auto find_in_trade = m_trade_map.find(it.first);
                            if(find_in_trade != m_trade_map.end()) {
                                cout << get_order->lineNo << ": AGGRESSIVE SELL " << find_in_trade->second << "@" << it.first << endl;
                                // Remove this item from the map
                                m_trade_map.erase(it.first);
                            }
                            else cout << get_order->lineNo << ": CANCEL BUY " << (find_change->second.order_quanity - it.second.order_quanity)<< "@" << it.first << endl;
                        }
                    }
                }
                //cout << "\n--- Handle ASK order list received\n";
                for(auto &itr : get_order->ask) {
                    //cout << "price:" << itr.first << "|count:" << itr.second.order_count << "|quantity:" << itr.second.order_quanity << endl;
                    auto find_change = m_prev_order->ask.find(itr.first);
                    if(find_change == m_prev_order->ask.end()) {
                        auto find_in_trade = m_trade_map.find(itr.first);
                        if(find_in_trade != m_trade_map.end()) {
                            cout << get_order->lineNo << ": AGGRESSIVE SELL " << find_in_trade->second + itr.second.order_quanity << "@" << itr.first << endl;
                            // Remove this item from the map
                            m_trade_map.erase(itr.first);
                        }
                        else cout << get_order->lineNo << ": PASSIVE SELL " << itr.second.order_quanity << "@" << itr.first << endl;
                    }
                    else {
                        // case 1: quantity in new order is greater than previous order
                        if(find_change->second.order_quanity < itr.second.order_quanity) {
                            //if(find_change->second.order_count < it.second.order_count) {
                                cout << get_order->lineNo << ": PASSIVE SELL " << (itr.second.order_quanity - find_change->second.order_quanity)<< "@" << itr.first << endl;
                            //}
                        }
                        // case 2: quantity in new order is less than previous order
                        else if(find_change->second.order_quanity > itr.second.order_quanity){
                            // check if having a trade in between
                            auto find_in_trade = m_trade_map.find(itr.first);
                            if(find_in_trade != m_trade_map.end()) {
                                cout << get_order->lineNo << ": AGGRESSIVE BUY " << find_in_trade->second << "@" << itr.first << endl;
                                // Remove this item from the map
                                m_trade_map.erase(itr.first);

                            }
                            else cout << get_order->lineNo << ": CANCEL SELL " << (find_change->second.order_quanity - itr.second.order_quanity)<< "@" << itr.first << endl;
                        }
                    }
                }

                // save the previous order before deleting it. can use copy assignment here
                m_prev_order = get_order;
            }
            /* Handle for TRADE event */
            else if(get_order->event_type == TRADE) {
                //cout << "\n---TRADE order received\n";
                //cout << "price:" << get_order->order_trade.price << "|quantity:" << get_order->order_trade.order_quanity << endl;
                // update to the previous order and set trade event is true
                auto find_in_bid = m_prev_order->bid.find(get_order->order_trade.price);
                auto find_in_ask = m_prev_order->ask.find(get_order->order_trade.price);
                if(find_in_bid != m_prev_order->bid.end()) {
                    find_in_bid->second.order_quanity -= get_order->order_trade.order_quanity;
                }
                else if(find_in_ask != m_prev_order->ask.end()) {
                    find_in_ask->second.order_quanity -= get_order->order_trade.order_quanity;
                }
                //1. check if this item already in the map, then update the quantity for it only
                auto find_in_trade = m_trade_map.find(get_order->order_trade.price);
                if(find_in_trade != m_trade_map.end()) {
                    find_in_trade->second += get_order->order_trade.order_quanity;
                }
                else {
                    m_trade_map.insert(std::pair<double, uint32_t>(get_order->order_trade.price, get_order->order_trade.order_quanity));
                }
            }

            delete get_order;
        }
        else {
            //std::this_thread::sleep_for(std::chrono::seconds(1));
        }
    }
}



void ProcessHandler::WriteToFile(std::string file_path)
{
    cout << "Entering func: " << __func__ << endl;

    cout << "Leaving func: " << __func__ << endl;
}
