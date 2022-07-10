
#include <fstream>

#include "input_handle_class.h"


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


void InputHandler::InitInputHandlerThread(std::string path_to_json)
{
    cout << "Entering func: " << __func__ << endl;
    m_input_handler_pth = new thread(&InputHandler::handleInputWorker, this, path_to_json);

    if(m_input_handler_pth->joinable()) {
        m_input_handler_pth->join();
    }

    std::this_thread::sleep_for(std::chrono::seconds(1));
    cout << "Leaving func: " << __func__ << endl;
    return 0;
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

                bool isTrade = ParseMandatory(triage_json, "trade", j_event_type);

              	if(isTrade != 0){
                    if(ParseMandatory(triage_json, "book", j_event_type) == 0) {
                        cout << "found a book event at line: " << idx << endl;
                        if(ParseMandatory(triage_json, "symbol", j_symbol) == 0) {
                            cout << "symbol: " << j_symbol.asString() << endl;
                        }
                        if(ParseMandatory(triage_json, "bid", j_order_bid_type) == 0) {
                            if(j_order_bid_type.isArray()) {
                                    cout << "--------- [BID] ---------\n";
                                for(int idx = 0; idx < j_order_bid_type.size(); ++idx) {
                                    cout << "Element: " << idx << endl;
                                    if (j_order_bid_type[idx].isMember("count")) {
                                      cout << "count:" <<j_order_bid_type[idx]["count"].asUInt64() << endl;
                                    }
                                    if (j_order_bid_type[idx].isMember("quantity")) {
                                      cout << "quantity:" << j_order_bid_type[idx]["quantity"].asUInt64() << endl;
                                    }
                                    if (j_order_bid_type[idx].isMember("price")) {
                                      cout << "price:" << j_order_bid_type[idx]["price"].asDouble() << endl;
                                    }
                                }
                            }
                        }
                        if(ParseMandatory(triage_json, "ask", j_order_ask_type) == 0) {
                            if(j_order_ask_type.isArray()) {
                                cout << "--------- [ASK] ---------\n";
                                for(int idx = 0; idx < j_order_bid_type.size(); ++idx) {
                                    cout << "Element: " << idx << endl;
                                    if (j_order_ask_type[idx].isMember("count")) {
                                      cout << "count:" <<j_order_ask_type[idx]["count"].asUInt64() << endl;
                                    }
                                    if (j_order_ask_type[idx].isMember("quantity")) {
                                      cout << "quantity:" << j_order_ask_type[idx]["quantity"].asUInt64() << endl;
                                    }
                                    if (j_order_ask_type[idx].isMember("price")) {
                                      cout << "price:" << j_order_ask_type[idx]["price"].asDouble() << endl;
                                    }
                                }
                            }

                        }

                    }
                }
                else {
                    cout << "found a trade event at line: " << idx << endl;
                    if(ParseMandatory(triage_json, "symbol", j_symbol) == 0) {
                        cout << "symbol: " << j_symbol.asString() << endl;
                    }
                    if(ParseMandatory(triage_json, "quantity", j_order_quantity) == 0) {
                        cout << "quantity: " << j_order_quantity.asUInt64() << endl;
                    }
                    if(ParseMandatory(triage_json, "price", j_order_price) == 0) {
                        cout << "price: " << j_order_price.asDouble() << endl;
                    }
                }

            }
            cout << "-------------\n";
        }
        file.close();
    }
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
