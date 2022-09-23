#pragma once

#include <map>
#include <list>
#include <vector>
#include <iterator>
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
    using rbnode_it = typename std::list<rbnode_t>::iterator;

    using hshlist_t = typename std::list<std::size_t>;

    std::unordered_map<key_t, hshlist_t> hashmap;
    std::map<std::size_t, rbnode_t> rbtree;

public:
    belady_t (std::size_t capacity_, std::vector<key_t> & input_reqs) 
    {
        capacity = capacity_;
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
private:
    void memory_insert(key_t ireq, std::size_t position) {
        auto hash_it = hashmap.find(ireq);

        if (hash_it == hashmap.end())
        {
            hshlist_t list_i {position}; //  create new list with the first element of near fp current key
            hashmap[ireq] = list_i;
            list_i.pop_front();  //  initially delete the first future position in cache line
        }
        else 
        {
            hash_it->second.push_back(position);
        }
    }

    bool cache_find (const key_t key)
    {
        auto list_i = hashmap.find(key)->second;
        
        if (list_i.empty()) //  this kind if elements must not be in the cache, because it need to be deleted
        {
            hashmap.erase(key);
            return false;
        }

        auto fp_i = list_i.front();
        if (rbtree.find(fp_i) == rbtree.end())
            return false;

        return true;    
    }

    template<typename func_t> void push_cache (const key_t key, const func_t slow_get_page)
    {
        hshlist_t fplist_i = hashmap.find(key)->second;

        if (fplist_i.empty())
        {
            hashmap.erase(key);
            return;
        }

       rbnode_t push_node {key, slow_get_page(key)};

        auto fp_i = fplist_i.front();
        if (rbtree.empty())
        {
            rbtree.insert({fp_i, push_node});
            size++;
            return;
        }

        auto farest = std::prev(rbtree.end());
        if (fp_i >= farest->first)
        {
            fplist_i.pop_front();
            return;
        }
        if (is_full())
        {
            rbtree.erase(farest);
            size--;
        }

        rbtree.insert({fp_i, push_node});
        size++;
    }

    void cache_update (const key_t key)
    {
        auto fplist_i = hashmap.find(key)->second;
        auto fp_i = fplist_i.front();
        auto node_i = rbtree.find(fp_i)->second;

        fplist_i.pop_front();

        if (fplist_i.empty()) //  element won't be requested in the future
        {
            rbtree.erase(fp_i);
            hashmap.erase(key);
            size--;
        }

        //  else the element need be replaced in the cache
        auto nextfp_i = fplist_i.front();
        rbtree.insert({nextfp_i, node_i});
        rbtree.erase(fp_i);
    }
};
}   //  namespace of belady cache
