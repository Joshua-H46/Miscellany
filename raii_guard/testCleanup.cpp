#include "RaiiGuard.h"
#include <iostream>

void foo1()
{
    auto cleanup = []() {
        std::cout << "Clean up done\n";
    };

    raii_guard::RaiiGuard rg(cleanup);
    std::cout << "foo1 running...\n";
}

void foo2()
{
    auto cleanup = []() {
        std::cout << "Clean up done\n";
    };

    raii_guard::RaiiGuard rg(cleanup);
    rg.cancel();
    std::cout << "foo2 running...\n";
}

void foo3()
{
    auto cleanup = []() {
        std::cout << "Clean up done\n";
    };

    raii_guard::RaiiGuard rg1(cleanup);
    raii_guard::RaiiGuard rg2(std::move(rg1));
    std::cout << "foo3 running...\n";
}

int main()
{
    foo1();
    foo2();
    foo3();
}