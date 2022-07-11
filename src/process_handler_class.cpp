/*!**************************************************************************
 *  \No COPYRIGHT
 *  \file   process_handler_class.cpp
 *  \brief  This class is to handle the main logic for handling the event changed and write to file
 *  \author thangvv
 *  \date   07/11/22
 *
 *  \brief Requirements Covered:
 *  \n 1)   REQ_XXXX_Assignment

 *  \note
 *   Revision History:
 *   Date        Author              Description
 *   ------      --------            --------------
 *   06/17/22    thangvv            Initial version

*****************************************************************************/

#include <sstream>

#include "process_handler_class.h"

#define ITENTION_PASSIVE "PASSIVE"
#define ITENTION_AGGRESSIVE "AGGRESSIVE"
#define ITENTION_CANCEL "CANCEL"

#define SIDE_BUY "BUY"
#define SIDE_SELL "SELL"


using namespace std;

ProcessHandler::ProcessHandler()
{
    //m_prev_order = new OrderQueue2();
    m_prev_order.lineNo = 0;
    m_prev_order.symbol = "";
    m_prev_order.event_type = "";
    m_prev_order.bid.clear();
    m_prev_order.ask.clear();
    m_prev_order.order_trade.order_count = 0;
    m_prev_order.order_trade.order_quanity = 0;
    m_prev_order.order_trade.price = 0.0;
    m_out_event = "";
}

ProcessHandler::~ProcessHandler()
{
    //dtor
}

/** \brief the main entry for the thread created. use the detach method instead of join
 *
 * \param
 * \param
 * \return void
 *
 */

void ProcessHandler::InitProcessThread()
{
    processApp_pth = new thread(&ProcessHandler::handleRequestWorker, this);

    if(processApp_pth->joinable()) {
        auto myid = processApp_pth->get_id();
        stringstream ss;
        ss << myid;
        thread_id_str = ss.str();
        cout << "process_thread_id: " << processApp_pth->get_id() << " string: " <<thread_id_str << endl;
        processApp_pth->detach();
    }

}


/** \brief This function is called from other thread (Input handling thread) to enqueue the Order information.
 *
 * \param order_queue is a OrderQueue2
 * \param
 * \return always return true
 *
 */

bool ProcessHandler::ProcessRequestArrived(OrderQueue2 order_queue)
{
    //cout << "Entering func: " << __func__ << endl;
    OrderQueue2 *_order_queue = new OrderQueue2(order_queue);
    m_order_queue.enqueue(_order_queue);
    //cout << "[Enqueue] Current Size: " << m_order_queue.queue_size() << endl;
    //cout << "Leaving func: " << __func__ << endl;
    return 0;
}


/** \brief main worker for this class which is to handling the every book and trade and send to write to file
 *
 * \return void
 *
 */

void ProcessHandler::handleRequestWorker()
{
    //cout << "Entering func: " << __func__ << endl;
    while(1) {
        if(m_order_queue.queue_size() > 0) {
            //cout << "\n[Dequeue] Current Size: " << m_order_queue.queue_size() << endl;
            OrderQueue2* get_order = m_order_queue.dequeue();
            m_out_event = "";

            /**< Handle for BOOK event */
            if(get_order->event_type == BOOK) {
                //cout << "\n---Handler BOOK order list received\n";
                for(auto &it : get_order->bid) {

                    auto find_change = m_prev_order.bid.find(it.first);
                    /**< if this key price is not existing in the previous book */
                    if(find_change == m_prev_order.bid.end()) {

                        auto find_in_trade = m_trade_map.find(it.first);
                        if(find_in_trade != m_trade_map.end()) {
                        /**< case 1: Quantity buy is greater than Quantity SELL */
                            cout << get_order->lineNo << ": AGGRESSIVE BUY " << find_in_trade->second + it.second.order_quanity << "@" << it.first << endl;
                            m_out_event = constructMesg(ITENTION_AGGRESSIVE, SIDE_BUY, find_in_trade->second + it.second.order_quanity, it.first);
                            m_trade_map.erase(it.first);
                        }
                        /**< case 2: This is new order buy */
                        else {
                            cout << get_order->lineNo << ": PASSIVE BUY " << it.second.order_quanity << "@" << it.first << endl;
                            m_out_event = constructMesg(ITENTION_PASSIVE, SIDE_BUY, it.second.order_quanity, it.first);
                        }

                    }
                    /**< if this key price is existing in the previous book */
                    else {
                        if(find_change->second.order_quanity < it.second.order_quanity) {
                            cout << get_order->lineNo << ": PASSIVE BUY " << (it.second.order_quanity - find_change->second.order_quanity)<< "@" << it.first << endl;
                            m_out_event = constructMesg(ITENTION_PASSIVE, SIDE_BUY, it.second.order_quanity - find_change->second.order_quanity, it.first);
                        }
                        else if(find_change->second.order_quanity >= it.second.order_quanity){
                            // check if having a trade in between
                            auto find_in_trade = m_trade_map.find(it.first);
                            if(find_in_trade != m_trade_map.end()) {
                                cout << get_order->lineNo << ": AGGRESSIVE SELL " << find_in_trade->second << "@" << it.first << endl;
                                m_out_event = constructMesg(ITENTION_AGGRESSIVE, SIDE_SELL, find_in_trade->second, it.first);
                                // Remove this item from the map
                                m_trade_map.erase(it.first);
                            }
                            else if(find_change->second.order_quanity > it.second.order_quanity) {
                                cout << get_order->lineNo << ": CANCEL BUY " << (find_change->second.order_quanity - it.second.order_quanity)<< "@" << it.first << endl;
                                m_out_event = constructMesg(ITENTION_CANCEL, SIDE_BUY, find_change->second.order_quanity - it.second.order_quanity, it.first);
                            }
                        }
                        //else cout << get_order->lineNo << ": [BUY SIDE] INVALID\n";
                    }
                }
                /**< Handle ASK order list received */
                for(auto &itr : get_order->ask) {
                    auto find_in_previous = m_prev_order.ask.find(itr.first);
                    if(find_in_previous == m_prev_order.ask.end()) {
                        auto find_in_trade = m_trade_map.find(itr.first);
                        if(find_in_trade != m_trade_map.end()) {
                            cout << get_order->lineNo << ": AGGRESSIVE SELL " << find_in_trade->second + itr.second.order_quanity << "@" << itr.first << endl;
                            m_out_event = constructMesg(ITENTION_AGGRESSIVE, SIDE_SELL, find_in_trade->second + itr.second.order_quanity, itr.first);
                            // Remove this item from the map
                            m_trade_map.erase(itr.first);
                        }
                        else {
                            cout << get_order->lineNo << ": PASSIVE SELL " << itr.second.order_quanity << "@" << itr.first << endl;
                            m_out_event = constructMesg(ITENTION_PASSIVE, SIDE_SELL, itr.second.order_quanity, itr.first);
                        }
                    }
                    else {
                        /**< case 1: quantity in new order is greater than previous order */
                        if(find_in_previous->second.order_quanity < itr.second.order_quanity) {
                            cout << get_order->lineNo << ": PASSIVE SELL " << (itr.second.order_quanity - find_in_previous->second.order_quanity)<< "@" << itr.first << endl;
                            m_out_event = constructMesg(ITENTION_PASSIVE, SIDE_SELL, itr.second.order_quanity - find_in_previous->second.order_quanity, itr.first);
                        }
                        /**< case 2: quantity in new order is less or equal than previous order */
                        else if(find_in_previous->second.order_quanity >= itr.second.order_quanity){
                            // check if having a trade in between
                            auto find_in_trade = m_trade_map.find(itr.first);
                            if(find_in_trade != m_trade_map.end()) {
                                cout << get_order->lineNo << ": AGGRESSIVE BUY " << find_in_trade->second << "@" << itr.first << endl;
                                m_out_event = constructMesg(ITENTION_AGGRESSIVE, SIDE_BUY, find_in_trade->second, itr.first);
                                // Remove this item from the map
                                m_trade_map.erase(itr.first);

                            }
                            else if(find_in_previous->second.order_quanity > itr.second.order_quanity) {
                                cout << get_order->lineNo << ": CANCEL SELL " << (find_in_previous->second.order_quanity - itr.second.order_quanity)<< "@" << itr.first << endl;
                                m_out_event = constructMesg(ITENTION_CANCEL, SIDE_SELL, find_in_previous->second.order_quanity - itr.second.order_quanity, itr.first);
                            }
                        }
                    }
                }

                /**< save the previous order before deleting it. can use copy assignment here */
                m_prev_order = *get_order;
            }
            /* Handle for TRADE event */
            else if(get_order->event_type == TRADE) {
                /**< update to the previous order and set trade event is true */
                auto find_in_bid = m_prev_order.bid.find(get_order->order_trade.price);
                auto find_in_ask = m_prev_order.ask.find(get_order->order_trade.price);
                if(find_in_bid != m_prev_order.bid.end()) {
                    find_in_bid->second.order_quanity -= get_order->order_trade.order_quanity;
                }
                else if(find_in_ask != m_prev_order.ask.end()) {
                    find_in_ask->second.order_quanity -= get_order->order_trade.order_quanity;
                }
                /**< check if this item already in the map, then update the quantity for it only */
                auto find_in_trade = m_trade_map.find(get_order->order_trade.price);
                if(find_in_trade != m_trade_map.end()) {
                    find_in_trade->second += get_order->order_trade.order_quanity;
                }
                else {
                    m_trade_map.insert(std::pair<double, uint32_t>(get_order->order_trade.price, get_order->order_trade.order_quanity));
                }
            }
            /**< write the file */
            if(m_out_event != "") {
                std::thread t(&ProcessHandler::WriteToFile, this, m_prev_order.symbol + ".txt", m_out_event );
                if(t.joinable()) {
                   t.detach();
                }
            }
            /**< free the dequeue class */
            delete get_order;
        }
        else {
            cout << "No more request, sleeping 1s... thread id: " << thread_id_str << " associated with symbol: " << m_prev_order.symbol << endl;
            std::this_thread::sleep_for(std::chrono::seconds(1));
        }
    }
}

/** \brief construct the message which shall be used to write out the file
 *
 * \param intention: PASSIVE, AGGRESSIVE, CANCEL
 * \param side: BUY or SELL
 * \param quantity: quantity
 * \param price: price
 * \return string is constructed
 *
 */

std::string ProcessHandler::constructMesg(const std::string& intention, const std::string& side, uint32_t quantity, double price) {
    stringstream ss{};
    std::string ret{};
    ret +=  intention + " " + side + " ";
    ss <<  quantity;
    ret += ss.str() + "@";
    ss.str("");
    ss << price;
    ret += ss.str();
    return ret;
}

/** \brief write a content to a file. here, file path is relative path which under /out/symbol_name.txt
 *
 * \param file_path: path to file
 * \param content: which is constructed in function constructMesg
 * \return void
 *
 */

void ProcessHandler::WriteToFile(std::string file_path, const std::string& content)
{
    std::lock_guard<std::mutex> lock(m_mutex);
    std::ofstream m_fout;
    m_fout.open("out/"  + file_path, ios::app); // create a file in /out folder
    if(m_fout) {
        m_fout << content << endl;
    }
    m_fout.close();
}
