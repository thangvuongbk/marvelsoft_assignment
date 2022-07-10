#include "process_object_pool_class.h"

using namespace std;

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
    cout << "Entering func:" << __func__ << endl;

    auto process_handler = m_process_object.find(symbol);
    if (process_handler == m_process_object.end())
    {
        std::cout << "Creating new." << std::endl;
        ProcessHandler* newObject = new ProcessHandler();
        m_process_object.insert(std::pair<std::string, ProcessHandler*> (symbol, newObject));

        cout << "Leaving func:" << __func__ << endl;
        return newObject;
    }
    else
    {
        std::cout << "Reusing existing." << std::endl;
        cout << "Entering func:" << __func__ << endl;
        return process_handler->second;
    }

}
