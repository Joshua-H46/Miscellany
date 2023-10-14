#include "SimpleThreadPool.h"
#include "ArbitraryTypeThreadPool.h"
#include <iostream>

int getNum(int i)
{
    return i;
}

int main()
{
    // thread_pool::SimpleThreadPool tp(4);
    thread_pool::ArbitraryThreadPool tp(4);
    for (int i=0; i<1024; i++)
    {
        tp.commit([num = i]() {
            std::cout << num << '\n';
        });
    }
    std::this_thread::sleep_for(std::chrono::seconds(3));

using ReturnType = std::invoke_result_t<decltype(getNum), int>;
    auto r1 = tp.commit(&getNum, 1);
    auto r2 = tp.commit(getNum, 6);
    auto r3 = tp.commit(getNum, 8);
    auto r4 = tp.commit(getNum, 5);

    std::cout << r1.get() << std::endl;
    std::cout << r2.get() << std::endl;
    std::cout << r3.get() << std::endl;
    std::cout << r4.get() << std::endl;
}