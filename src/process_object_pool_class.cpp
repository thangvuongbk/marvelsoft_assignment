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


ProcessOBjectPool* ProcessOBjectPool::getIns()
{
    static ProcessOBjectPool ins;
    return &ins;
}



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
