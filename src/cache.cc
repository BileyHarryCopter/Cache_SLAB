//                                      //
//  Fake stranger's code for my own)    //
//                                      //
#include <iostream>
#include <cassert>

#include "cache.hpp"

int slow_get_page (int key) { return key; }

int main (void)
{
    unsigned hits         = 0;
    unsigned numb_request = 0;
    size_t   cache_size   = 0;  

    std::cout << "Input size (positive integer number) of cache:\n";
    std::cin >> cache_size;
    std::cout << "Input number (positive integer number) of requests:\n";
    std::cin >> numb_request; 

    // There is a place for crazy cpplang code

    std::cout << numb_request << ' ' << cache_size << '\n';

    //  Short aneckdot
    caches::bebra Bebra = {21};
    std::cout << "Bebra value:\n" << Bebra.length << '\n';

    return 0;    
}