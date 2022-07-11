/*!**************************************************************************
 *  \No COPYRIGHT
 *  \file   process_object_pool_class.cpp
 *  \brief  This class is to manage the process class as process object pool.
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

#include <thread>

#include "process_object_pool_class.h"

using namespace std;

int ProcessOBjectPool::m_thread_count = 0;


ProcessOBjectPool::ProcessOBjectPool()
{
    //ctor
}

ProcessOBjectPool::~ProcessOBjectPool()
{
    //dtor
}

/** \brief This class should have only one instance
 *
 * \param
 * \param
 * \return ProcessOBjectPool
 *
 */

ProcessOBjectPool* ProcessOBjectPool::getIns()
{
    static ProcessOBjectPool ins;
    return &ins;
}


/** \brief if a ProcessHandler is associated with a unique symbol is created, then it returns, else create a new object and save to pool
 *
 * \param symbol: which shall associate with an unique object
 * \param
 * \return ProcessHandler
 *
 */

ProcessHandler* ProcessOBjectPool::getProcessObj(std::string symbol)
{
    auto process_handler = m_process_object.find(symbol);
    if (process_handler == m_process_object.end())
    {
        m_thread_count++;
        ProcessHandler* newObject = new ProcessHandler();
        m_process_object.insert(std::pair<std::string, ProcessHandler*> (symbol, newObject));
        //newObject->InitProcessThread();
        std::thread t(&ProcessHandler::InitProcessThread, newObject);
        if(t.joinable()) {
           t.detach();
        }
        //std::cout << "Creating new[thread_id: " << newObject->thread_id_str << "; TotalThread: " << m_thread_count << "]" << std::endl;
        //std::cout << "Creating new. Total thread: " << m_thread_count  << std::endl;
        return newObject;
    }
    else
    {
        //std::cout << "Reusing existing. thread_id = " << process_handler->second->thread_id_str << std::endl;
        return process_handler->second;
    }

}
