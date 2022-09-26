#pragma once

#include <map>
#include <list>
#include <vector>
#include <unordered_map>
#include <iostream>

namespace belady_chc
{
template<typename page_t, typename key_t = int> class belady_t
{
    std::size_t capacity;
    std::size_t size = 0;
    //  structure of node in the red-black tree
    struct rbnode_t
    {
        key_t key;
        page_t page;
    };
    std::unordered_map<key_t, std::list<std::size_t>> hashmap;
    std::map<std::size_t, rbnode_t> rbtree;

public:
    belady_t (std::size_t capacity_, std::vector<key_t> & input_reqs) : capacity {capacity_} 
    {
        //  creating a hashmap from input
        std::size_t position = 0;
        for (auto ireq : input_reqs)
        {
            memory_insert(ireq, position);
            position++;
        }
    }

    bool is_full () { return size == capacity; };

    template<typename func_t> bool lookup_update (const key_t key, const func_t slow_get_page)
    {
        if (!cache_find(key))
        {
            push_cache (key, slow_get_page);
            return false;
        }
        else 
        {
            cache_update (key);
            return true;
        }
    }

    void cache_dump (key_t ireq, size_t position)
    {
        std::cout << "KEY: " << ireq << " POS: " << position << '\n';
        auto list_i = hashmap.find(ireq)->second;
        std::cout << "HSHLIST OF KEY:\n\t";
        for (auto pos_i = list_i.begin(); pos_i != list_i.end(); pos_i = std::next(pos_i))
        {
            std::cout << *pos_i << " -> ";
        }
        std::cout << "NULL\n";

        std::cout << "TREE STAGE: \n\t";
        for (auto node_i : rbtree)
        {
            std::cout << node_i.second.key << '(' << node_i.first << ')' << " -> ";
        }
        std::cout << "NULL\n\n\n\n";
    }

private:
    void memory_insert(key_t ireq, std::size_t position) 
    {
        auto hash_it = hashmap.find(ireq);

        if (hash_it == hashmap.end())
            hashmap[ireq] = {};
        else 
            hash_it->second.push_back(position);
    }

    bool cache_find (const key_t key)
    {
        auto list_i = hashmap.find(key)->second;
        auto fp_i = list_i.front();
        if (rbtree.find(fp_i) == rbtree.end())
            return false;
        return true;    
    }

    template<typename func_t> void push_cache (const key_t key, const func_t slow_get_page)
    {
        auto& fplist_i = hashmap.find(key)->second;

        if (fplist_i.empty())
        {
            hashmap.erase(key);
            return;
        }

        auto fp_i = fplist_i.front();
        if (rbtree.empty())
        {
            rbtree.insert({fp_i, {key, slow_get_page(key)}});
            size++;
            return;
        }

        auto farest = std::prev(rbtree.end());
        if (is_full())
        {
            if (fp_i >= farest->first)
            {
                fplist_i.pop_front();
                return;
            }
            hashmap[farest->second.key].pop_front();
            rbtree.erase(farest->first);
            size--;
        }

        rbtree.insert({fp_i, {key, slow_get_page(key)}});
        size++;
    }

    void cache_update (const key_t key)
    {
        auto& fplist_i = hashmap.find(key)->second;
        auto fp_i = fplist_i.front();
        auto node_i = rbtree.find(fp_i)->second;

        fplist_i.pop_front();

        if (fplist_i.empty()) //  element won't be requested in the future
        {
            rbtree.erase(fp_i);
            hashmap.erase(key);
            size--;
            return;
        }

        //  else the element need be replaced in the cache
        auto nextfp_i = fplist_i.front();
        rbtree.insert({nextfp_i, node_i});
        rbtree.erase(fp_i);
    }

};
}   //  namespace of belady cache
