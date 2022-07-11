/*!**************************************************************************
 *  \No COPYRIGHT
 *  \file   main.cpp
 *  \brief  entry for the program
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

#include <iostream>
#include <string>
#include <chrono>

#include "input_handle_class.h"

using namespace std;


void init() {
    //std::string path_to_json = "/home/thangvuong/work/CodeBlock_WS/Assignment/input.json";
    // init the input handler thread
    InputHandler *inp_handler_thread = new InputHandler();
    inp_handler_thread->InitInputHandlerThread("in/input.json"); // This is under "in" default folder and exact input name is "input.json"
}

int main(int agrc, char** argv)
{
    init();
    while(1) {
        //cout << "main thread waiting ...\n";
        //std::this_thread::sleep_for(std::chrono::seconds(1));
   }

    return 0;
}


















