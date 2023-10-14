#include "SPSCQueue.h"
#include "MPSCQueue.h"
#include <thread>
#include <iostream>

int main()
{
    /*
    ring_buffer::SPSCQueue<uint64_t> que(1024);
    std::thread t1([&]() {
        uint64_t val;
        while (true) {
            if (que.pop(val)) {
                //std::cout << "Popping " << val << std::endl;
                if (val == 19999) {
                    return;
                }
            }
        }
    });

    int i = 0;
    while (true) {
        if (i == 20000) {
            break;
        }
        if (que.push(i)) {
            i++;
        }
    }
    t1.join();

    */

    ring_buffer::MPSCQueue<uint64_t> que(1024);
    std::thread t1([&]() {
        uint64_t cnt1 = 0;
        while (cnt1 < 100000) {
            if (que.push(cnt1)) {
                cnt1++;
            }
        }
    });
    std::thread t2([&]() {
        uint64_t cnt2 = 100000;
        while (cnt2 < 200000) {
            if (que.push(cnt2)) {
                cnt2++;
            }
        }
    });
    std::thread t3([&]() {
        uint64_t cnt3 = 200000;
        while (cnt3 < 300000) {
            if (que.push(cnt3)) {
                cnt3++;
            }
        }
    });
    std::thread t4([&]() {
        uint64_t cnt4 = 300000;
        while (cnt4 < 400000) {
            if (que.push(cnt4)) {
                cnt4++;
            }
        }
    });
    int cnt = 0;
    uint64_t val;
    while (cnt < 400000) {
        if (que.pop(val)) {
            std::cout << "Popping " << val << ' ' << cnt << std::endl;
            cnt++;
        }
    }
    t1.join();
    t2.join();
    t3.join();
    t4.join();
}