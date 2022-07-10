#ifndef PROCESSOBJECTPOOL_H
#define PROCESSOBJECTPOOL_H

#include <map>
#include <string>

#include "process_handler_class.h"

class ProcessOBjectPool
{
    public:
        ProcessOBjectPool();
        virtual ~ProcessOBjectPool();

        /**
         * Static method for accessing class instance.
         * Part of Singleton design pattern.
         *
         * @return ProcessOBjectPool instance.
         */
        static ProcessOBjectPool* getIns();

        /**
         * Returns instance of ProcessHandler.
         *
         * New ProcessHandler will be created if its not available based on the symbol
         *
         * @return ProcessHandler instance.
         */
        ProcessHandler* getProcessObj(std::string symbol);

    private:
        std::map<std::string, ProcessHandler*> m_process_object;
        static int m_thread_count;

};

#endif // PROCESSOBJECTPOOL_H
