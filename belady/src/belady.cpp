#include <vector>
#include <cassert>
#include <iostream>
#include "belady.hpp"

// slow get page imitation
int slow_get_page(int key) { return key; }

std::size_t contest_processing (size_t cache_cap, int requests)
{
    key_t ireq = 0;
    std::vector<key_t> memory;
    //  input from std::cin and move it in vector
    for (int i = 0; i < requests; i++)
    {
        std::cin >> ireq;
        assert (std::cin.good());
        memory.push_back(ireq);
    }

    std::size_t hits = 0;
    auto & memory_ref = memory;
    belady_chc::belady_t<int> cache (cache_cap, memory_ref);
    //  int i = 0;
    for (key_t ireq : memory)
    {
        //  cache.cache_dump(ireq, i);
        if (cache.lookup_update (ireq, slow_get_page))
            hits++;
        //  i++;
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