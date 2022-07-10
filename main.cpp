#include <iostream>
#include <string>

#include <fstream>
#include <json/json.h>
#include <memory>

using namespace std;


class OrderQueue2 {
public:
	typedef struct order_attr_ {
	    uint32_t order_count;
		uint32_t order_quanity;
	} order_attr_t;

	std::string event_type; // book or trade
    std::string symbol;
	std::map<double, order_attr_t> bid; // BUY. first is price value
	std::map<double, order_attr_t> ask; // SELL. first is price value
};

void init() {


}

int main()
{
    //std::string path_to_json = "/home/thangvuong/work/CodeBlock_WS/Assignment/input.json";
    std::string path_to_json = "/home/thangvuong/work/CodeBlock_WS/Assignment/input_2.json";

    return 0;
}


















