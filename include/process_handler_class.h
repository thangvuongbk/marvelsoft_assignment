/*!**************************************************************************
 *  \No COPYRIGHT
 *  \file   process_handler_class.h
 *  \brief  header file for process handler class
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

#ifndef PROCESSHANDLER_H
#define PROCESSHANDLER_H

#include <iostream>
#include <mutex>
#include <thread>
#include <string>
#include <chrono>
#include <map>
#include <fstream>


#include "../template.h"
#include "../order_property.h"

class ProcessHandler
{
    public:
        ProcessHandler();
        virtual ~ProcessHandler();

        /* init function to create a new thread for it */
        void InitProcessThread();

        /* Received command from client*/
        //bool ProcessRequestArrived(OrderQueue& order_queue);
        bool ProcessRequestArrived(OrderQueue2 order_queue);

        /* Write to file */
        void WriteToFile(std::string file_path, const std::string& content);

    public:
        std::thread *processApp_pth;
        std::string  thread_id_str;

    private:
         /* Act as a process worker*/
        void handleRequestWorker();

        /* Construct a message output*/
        std::string constructMesg(const std::string& intention, const std::string& side, uint32_t quantity, double price);

    private:
        WaitSignalQueue<OrderQueue2*> m_order_queue;
        OrderQueue2 m_prev_order;
        std::map<double, uint32_t> m_trade_map; // price and quantity from trade event
        mutable std::mutex m_mutex;
        std::string m_out_event;
};

#endif // PROCESSHANDLER_H
