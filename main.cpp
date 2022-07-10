#include <iostream>
#include <string>


#include "input_handle_class.h"

using namespace std;


void init() {
    //std::string path_to_json = "/home/thangvuong/work/CodeBlock_WS/Assignment/input.json";
    //std::string path_to_json = "/home/thangvuong/work/CodeBlock_WS/Assignment/input_2.json";
    //std::string path_to_json = "/home/thangvuong/work/CodeBlock_WS/Assignment/CIMB.json";
    std::string path_to_json = "/home/thangvuong/work/CodeBlock_WS/Assignment/ABBN.json";

    // init the input handler thread
    InputHandler *inp_handler_thread = new InputHandler();
    inp_handler_thread->InitInputHandlerThread(path_to_json);


}

int main()
{
    init();
    while(1) {
        //cout << "main thread waiting ...\n";
    }

    return 0;
}


















