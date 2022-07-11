/*!**************************************************************************
 *  \No COPYRIGHT
 *  \file   process_object_pool.h
 *  \brief  header file for process object pool class
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
