#include "SimpleThreadPool.h"
#include <iostream>

int main()
{
    thread_pool::SimpleThreadPool tp(4);
    for (int i=0; i<1024; i++)
    {
        tp.commit([num = i]() {
            std::cout << num << '\n';
        });
    }
    std::this_thread::sleep_for(std::chrono::seconds(3));
}