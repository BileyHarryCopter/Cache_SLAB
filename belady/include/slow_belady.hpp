#pragma once

#include <list>
#include <vector>


namespace slow_belady_chc 
{

constexpr int NO_NEED = -1;

template<typename page_t, typename key_t = int> class slow_belady_t
{
    std::size_t size = 0;
    std::size_t capacity;
    std::size_t current_pos = 0;

    struct node_t
    {
        int fppos;
        key_t key;
        page_t page;
    };

    std::list<node_t> cache_list;
    std::vector<int> memory;

    using list_it = typename std::list<node_t>::iterator;

public:
    slow_belady_t (std::size_t capacity_, std::vector<key_t> & input_reqs) : capacity {capacity_}
    {
        //  creation the memory of cache for future prediction
        for (auto x : input_reqs)
            memory.push_back(x);
    }

    bool is_full () const { return size == capacity; };

    template<typename func_t> bool lookup_update (const key_t key, const func_t slow_get_page)
    {
        if (find_by_key(key) == cache_list.end())
        {
            push_cache(key, slow_get_page);
            current_pos++;
            return false;
        }
        else 
        {
            cache_update(key);
            current_pos++;
            return true;
        }
    }

    void cache_dump (const key_t key) const
    {
        std::cout << "KEY: " << key << "\n\t";
        for (auto x : cache_list)
        {
            std::cout << x.key << '(' << x.fppos << ')' << " -> ";
        }
        std::cout << "NULL\n\n\n";
    }

private:
    int find_fp (const key_t key) //    fp = future fpposition
    {
        for (auto fp = current_pos + 1; fp < memory.size(); fp++)
        {
            if (memory[fp] == key)
                return fp;
        }
        return NO_NEED;
    }

    list_it find_next_node (const int fp)
    {
        for (auto node_i = cache_list.begin(); node_i != cache_list.end(); node_i = std::next(node_i))
        {
            if (node_i->fppos > fp)
                return node_i;
        }
        return cache_list.end();  //  return cache_list.end()
    }

    list_it find_by_key (const key_t key)
    {
        for (auto node_i = cache_list.begin(); node_i != cache_list.end(); node_i = std::next(node_i))
        {
            if (node_i->key == key)
                return node_i;
        }
        return cache_list.end();
    }

    template<typename func_t> void push_cache (const key_t key, const func_t slow_get_page)
    {
        auto fp = find_fp (key);

        if (fp == NO_NEED)
            return;  

        if (is_full())
        {
            if (fp >= cache_list.back().fppos)    
                return; 
            cache_list.pop_back();
            size--;
        }

        auto next_node = find_next_node (fp);
        if (next_node == cache_list.end())
            cache_list.push_back({fp, key, slow_get_page(key)});
        else 
            cache_list.insert(next_node, {fp, key, slow_get_page(key)});
        size++;
    }

    void cache_update (const key_t key)
    {
        auto fp = find_fp(key);
        auto node_i = find_by_key(key);
        if (fp == NO_NEED)
        {
            cache_list.erase(node_i);
            size--;
            return;
        }

        auto next_node = find_next_node (fp);
        if (next_node == cache_list.end())
            cache_list.push_back({fp, key, node_i->page});
        else
            cache_list.insert(next_node, {fp, key, node_i->page});

        cache_list.erase(node_i);
    }
};

}   //  namespace of slow version of belady cache