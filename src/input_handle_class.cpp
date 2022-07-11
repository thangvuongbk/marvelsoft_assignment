
#include <iostream>
#include <fstream>
#include <memory>

#include "input_handle_class.h"
#include "process_object_pool_class.h"


using namespace std;

class OrderQueue2;

static inline uint32_t ParseMandatory(Json::Value json, std::string key, Json::Value& result)
{
    // find at level 1
    if (json.isMember(key)) {
        result = json[key];
        return 0;
    }

    // find at level 2
    for (auto const& ObjId : json.getMemberNames()) {
        Json::Value id = json[ObjId];
        if (id.isMember(key)) {
            result = id[key];
            return 0;
        }

    }
    // if not found
    result = Json::Value::null;
    return -1;
}

//===================================================


InputHandler::InputHandler()
{
    //ctor
}

InputHandler::~InputHandler()
{
    //dtor
}

bool InputHandler::convertString2Json(std::string inp_string, Json::Value& out_json)
{
	Json::CharReaderBuilder builder;
    auto reader = std::unique_ptr<Json::CharReader>(builder.newCharReader());
    std::string s_error{};
    Json::Value ret_json{};
    if (!reader->parse(inp_string.c_str(), inp_string.c_str() + strlen(inp_string.c_str()), &ret_json, &s_error))
    {
        cout << "Parse Error" << endl;
        return -1;
    }
    out_json = ret_json;
    return 0;
}

void InputHandler::InitInputHandlerThread(std::string path_to_json)
{
    m_input_handler_pth = new thread(&InputHandler::handleInputWorker, this, path_to_json);

    if(m_input_handler_pth->joinable()) {
        m_input_handler_pth->join();
    }
}

void InputHandler::handleInputWorker(std::string path_to_json)
{
    std::ifstream file(path_to_json);

    Json::Value triage_json{};

    if(file.is_open()) {
        std::string line{};
        int idx = 0;
        while(std::getline(file, line))  {
            ++idx;
            if(convertString2Json(line, triage_json) != 0) {
                cout << "[Error] input string info: \n";
                cout << line << endl;
            }
            else {
                Json::Value j_event_type;
                Json::Value j_symbol;
                Json::Value j_order_bid_type;
                Json::Value j_order_ask_type;

                Json::Value j_order_quantity;
                Json::Value j_order_price;

                OrderQueue2 order; // we only need the object created in stack
                OrderQueue2::order_attr_ order_attr;
                order.lineNo = idx;
                order.bid.clear();
                order.ask.clear();

                bool isTrade = ParseMandatory(triage_json, TRADE, j_event_type);

              	if(isTrade != 0){
                    if(ParseMandatory(triage_json, BOOK, j_event_type) == 0) {
                        //cout << "found a book event at line: " << idx << endl;

                        order.event_type = BOOK;
                        if(ParseMandatory(triage_json, SYMBOL, j_symbol) == 0) {
                            //cout << "symbol: " << j_symbol.asString() << endl;
                            order.symbol = j_symbol.asString();
                        }
                        if(ParseMandatory(triage_json, BID, j_order_bid_type) == 0) {
                            if(j_order_bid_type.isArray()) {
                                    //cout << "--------- [BID] ---------\n";
                                for(int idx = 0; idx < j_order_bid_type.size(); ++idx) {
                                    //cout << "Element: " << idx << endl;
                                    if (j_order_bid_type[idx].isMember(COUNT)) {
                                      //cout << "count:" <<j_order_bid_type[idx][COUNT].asUInt64() << endl;
                                    }
                                    if (j_order_bid_type[idx].isMember(QUANTITY)) {
                                      //cout << "quantity:" << j_order_bid_type[idx][QUANTITY].asUInt64() << endl;
                                    }
                                    if (j_order_bid_type[idx].isMember(PRICE)) {
                                      //cout << "price:" << j_order_bid_type[idx][PRICE].asDouble() << endl;
                                    }
                                    // push to the bid ordered map
                                    order_attr.order_count = j_order_bid_type[idx][COUNT].asUInt64();
                                    order_attr.order_quanity = j_order_bid_type[idx][QUANTITY].asUInt64();
                                    order.bid.insert(std::pair<double, OrderQueue2::order_attr_>(j_order_bid_type[idx][PRICE].asDouble(), order_attr));
                                }
                            }
                        }
                        if(ParseMandatory(triage_json, ASK, j_order_ask_type) == 0) {
                            if(j_order_ask_type.isArray()) {
                                //cout << "--------- [ASK] ---------\n";
                                for(int idx = 0; idx < j_order_ask_type.size(); ++idx) {
                                    //cout << "Element: " << idx << endl;
                                    if (j_order_ask_type[idx].isMember(COUNT)) {
                                      //cout << "count:" <<j_order_ask_type[idx][COUNT].asUInt64() << endl;
                                    }
                                    if (j_order_ask_type[idx].isMember(QUANTITY)) {
                                     // cout << "quantity:" << j_order_ask_type[idx][QUANTITY].asUInt64() << endl;
                                    }
                                    if (j_order_ask_type[idx].isMember(PRICE)) {
                                      //cout << "price:" << j_order_ask_type[idx][PRICE].asDouble() << endl;
                                    }
                                    // push to the bid ordered map
                                    order_attr.order_count = j_order_ask_type[idx][COUNT].asUInt64();
                                    order_attr.order_quanity = j_order_ask_type[idx][QUANTITY].asUInt64();
                                    order.ask.insert(std::pair<double, OrderQueue2::order_attr_>(j_order_ask_type[idx][PRICE].asDouble(), order_attr));
                                }
                            }

                        }

                    }
                }
                else {
                    order.event_type = TRADE;
                    //cout << "found a trade event at line: " << idx << endl;
                    if(ParseMandatory(triage_json, SYMBOL, j_symbol) == 0) {
                        //cout << "symbol: " << j_symbol.asString() << endl;
                        order.symbol = j_symbol.asString();
                    }
                    if(ParseMandatory(triage_json, QUANTITY, j_order_quantity) == 0) {
                        //cout << "quantity: " << j_order_quantity.asUInt64() << endl;
                        order.order_trade.order_quanity = j_order_quantity.asUInt64();
                    }
                    if(ParseMandatory(triage_json, PRICE, j_order_price) == 0) {
                        //cout << "price: " << j_order_price.asDouble() << endl;
                        order.order_trade.price = j_order_price.asDouble();
                    }
                }
                //cout << "Get the process object and send to queue\n";
                sendOrderToProcess(order);
            }
        }
        file.close();
    }
}


void
InputHandler::sendOrderToProcess(OrderQueue2& pack_order) {
        //cout << "Entering func: " << __func__ << endl;
        ProcessOBjectPool* get_ins = ProcessOBjectPool::getIns();
        ProcessHandler* get_process_obj = get_ins->getProcessObj(pack_order.symbol);
        get_process_obj->ProcessRequestArrived(pack_order);
        //cout << "Leaving func: " << __func__ << endl;
}

