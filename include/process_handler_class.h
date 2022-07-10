#ifndef PROCESSHANDLER_H
#define PROCESSHANDLER_H

#include <iostream>
#include <mutex>
#include <thread>
#include <string>
#include <chrono>


#include "../template.h"
#include "../order_property.h"

class ProcessHandler
{
    public:
        WaitSignalQueue<OrderQueue*> m_order_queue;

    public:
        ProcessHandler();
        virtual ~ProcessHandler();

        /* init function to create a new thread for it */
        bool InitProcessThread();

        /* Received command from client*/
        bool ProcessRequestArrived(OrderQueue& order_queue);

        /* Write to file */
        void WriteToFile(std::string file_path);

    protected:
         /* Act as a process worker*/
        void handleRequestWorker();

    private:
        std::thread *processApp_pth;

};

#endif // PROCESSHANDLER_H
