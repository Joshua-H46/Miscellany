#include "CallOnce.h"
#include <atomic>
#include <thread>
#include <iostream>

void foo(int i)
{
    std::this_thread::sleep_for(std::chrono::seconds(3));
    std::cout << "Call foo " << i << "\n";
}

int main() {
    call_once::OnceFlag flag;
    std::vector<std::thread> vec;
    vec.reserve(32);
    for (int i=0; i<32; i++)
    {
        vec.emplace_back(std::thread([&](int n) {
            call_once::callOnce(flag, foo, n);
            std::cout << "thread " << n << " finished\n";
        }, i));
    }
    for (auto& t : vec)
    {
        t.join();
    }
}