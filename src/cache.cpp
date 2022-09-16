#include <iostream>
#include "cache.hpp"
#include <cassert>

// slow get page imitation
int slow_get_page(int key) { return key; }

int contest_processing (size_t cache_cap, int requests)
{
    chc::lfuda_t<int> cache {cache_cap};

    int ireq = 0;
    int hits = 0;

    for (int i = 0; i < requests; i++)
    {
        std::cin >> ireq;
        assert (std::cin.good());
        
        cache.cache_dump(ireq);

        if (cache.lookup_update (ireq, slow_get_page))
            hits++;
    }
    cache.cache_dump(ireq);

    return hits;
}

int main (void)
{
    int reqs = 0;
    size_t init_cap   = 0;

    std::cin >> init_cap >> reqs;
    assert (std::cin.good());

    int hits = contest_processing (init_cap, reqs);
    std::cout << "Hits: " << hits << std::endl;

    return 0;
}