/*!**************************************************************************
 *  \No COPYRIGHT
 *  \file   input_handle_class.h
 *  \brief  header file for input handle class
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

        /**< thread created from here */
        void InitInputHandlerThread(std::string path_to_json);

    private:
        /**< main logic handling */
        void handleInputWorker(std::string path_to_json);
        /**< convert string to json format */
        bool convertString2Json(std::string inp_string, Json::Value& out_json);
        /**< send a Order queue class to process thread */
        void sendOrderToProcess(OrderQueue2& pack_order);

    private:
        std::thread *m_input_handler_pth;
};

#endif // INPUTHANDLER_H
