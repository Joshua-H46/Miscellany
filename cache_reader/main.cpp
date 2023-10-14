#include "CacheReader.h"
#include <iostream>
#include <cstdlib>

const char* str = "123456789123456789123456789123456789123456789123456789123456789123456789123456789";

int main()
{
    auto n = strlen(str);
    cache_reader::CacheReader creader(str, n);
    cache_reader::BlockReader<8> breader(str, n);
    
    // int k1 = 0;
    // while (k1 < n)
    // {
    //     char buffer[16];
    //     int sz = breader.read(buffer);
    //     buffer[sz] = '\0';
    //     std::cout << 8 << '\t' << sz << '\t' << buffer << std::endl;
    //     k1 += 8;
    // }
    // std::cout << std::endl;

    int k = 0;
    while (k < n)
    {
        auto size = std::rand() % 15;
        char buffer[16];
        int sz = creader.read(buffer, size);
        buffer[sz] = '\0';
        std::cout << size << '\t' << sz << '\t' << buffer << std::endl;
        k += size;
    }
}