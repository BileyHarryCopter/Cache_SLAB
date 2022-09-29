#include <iostream>
#include "lfuda.hpp"
#include <cassert>
#include <chrono>

// slow get page imitation
int slow_get_page(int key) { return key; }

std::size_t contest_processing (size_t cache_cap, int requests)
{
    lfuda_chc::lfuda_t<int> cache {cache_cap};

    int ireq = 0;
    std::size_t hits = 0;

    for (int i = 0; i < requests; i++)
    {
        std::cin >> ireq;
        assert (std::cin.good());

        if (cache.lookup_update (ireq, slow_get_page))
            hits++;
    }

    return hits;
}

int main (void)
{
    int reqs = 0;
    size_t init_cap   = 0;

    std::cin >> init_cap >> reqs;
    assert (std::cin.good());

    std::size_t hits = contest_processing (init_cap, reqs);
    std::cout << hits << '\n';

    return 0;
}