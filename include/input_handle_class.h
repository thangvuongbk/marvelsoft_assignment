#ifndef INPUTHANDLER_H
#define INPUTHANDLER_H

#include <thread>

#include <string>
#include <json/json.h>

#include "../order_property.h"

class InputHandler
{
    public:
        InputHandler();
        virtual ~InputHandler();

        void InitInputHandlerThread(std::string path_to_json);

    private:
        void handleInputWorker(std::string path_to_json);
        bool convertString2Json(std::string inp_string, Json::Value& out_json);
        void sendOrderToProcess(OrderQueue2& pack_order);

    private:
        std::thread *m_input_handler_pth;
};

#endif // INPUTHANDLER_H
