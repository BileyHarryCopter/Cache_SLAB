#include <iostream>
#include "cache.hpp"

int contest_processing (size_t cache_cap, int requests)
{
    chc::lfuda_t<int> cache {cache_cap};
    int ireq = 0;
    int hits = 0;

    for (auto i = 0; i < requests; i++)
    {
        std::cin >> ireq;
        assert (std::cin.good());

        if (cache.lookup (ireq, slow_get_page))
            hits++;
    }

    return hits;
}

int main (void)
{
    int reqs = 0;
    size_t init_cap   = 0;

    std::cin >> cap >> reqs;
    assert (std::cin.good());

    int hits = test_process (cap, reqs);

    std::cout << "Hits: " << hits << std::endl;

    return 0;
}