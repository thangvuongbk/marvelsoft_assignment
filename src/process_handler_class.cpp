#include "process_handler_class.h"

using namespace std;

ProcessHandler::ProcessHandler()
{
    //ctor
}

ProcessHandler::~ProcessHandler()
{
    //dtor
}

bool ProcessHandler::InitProcessThread()
{
    cout << "Entering func: " << __func__ << endl;
    processApp_pth = new thread([&]() noexcept {this->handleRequestWorker();});

    if(processApp_pth->joinable()) {
        processApp_pth->join();
    }

    std::this_thread::sleep_for(std::chrono::seconds(1));
    cout << "Leaving func: " << __func__ << endl;
    return 0;

}

bool ProcessHandler::ProcessRequestArrived(OrderQueue& order_queue)
{
    cout << "Entering func: " << __func__ << endl;

    cout << "order_queue.symbol: " << order_queue.symbol << endl;
    cout << "order_queue.event_type: " << order_queue.event_type << endl;
    cout << "order_queue.order_type: " << order_queue.order_type << endl;
    cout << "order_queue.order_count: " << order_queue.order_count << endl;
    cout << "order_queue.order_quanity: " << order_queue.order_quanity << endl;
    cout << "order_queue.order_price: " << order_queue.order_price << endl;

    cout << "Queue this order now" << endl;
    m_order_queue.enqueue(&order_queue);

    cout << "[Enqueue] Current Size: " << m_order_queue.queue_size() << endl;

    cout << "Leaving func: " << __func__ << endl;
    return 0;
}

void ProcessHandler::handleRequestWorker()
{
    cout << "Entering func: " << __func__ << endl;
    while(true) {
        if(m_order_queue.queue_size() > 0) {
           cout << "[Dequeue] Current Size: " << m_order_queue.queue_size() << endl;
           OrderQueue* get_order = m_order_queue.dequeue();
            cout << "get_order->symbol: " << get_order->symbol << endl;
            cout << "get_order->event_type: " << get_order->event_type << endl;
            cout << "get_order->order_type: " << get_order->order_type << endl;
            cout << "get_order->order_count: " << get_order->order_count << endl;
            cout << "get_order->order_quanity: " << get_order->order_quanity << endl;
            cout << "get_order->order_price: " << get_order->order_price << endl;

        }
        else {
            std::this_thread::sleep_for(std::chrono::seconds(2));
        }

    }

    cout << "Leaving func: " << __func__ << endl;

}

void ProcessHandler::WriteToFile(std::string file_path)
{
    cout << "Entering func: " << __func__ << endl;

    cout << "Leaving func: " << __func__ << endl;
}
