#pragma once

#include <map>
#include <list>
#include <vector>
#include <iterator>
#include <unordered_map>
#include <iostream>

namespace belady_chc
{
template<typename page_t, typename key_t = int> class belady_cache_t
{
    std::size_t capacity;
    std::size_t size = 0;

    //  structure of node in the red-black tree
    struct rbnode_t
    {
        key_t key;
        page_t page;
    };

    using hshlist_t = typename std::list<std::size_t>;
    using rbnode_it = typename std::list<rbnode_t>::iterator;
    //  using hshnode_it  = typename std::list<std::size_t>::iterator;
    //  using input_it  = typename std::vector<std::size_t>::iterator;

    std::unordered_map<key_t, hshlist_t> hashmap;
    std::map<std::size_t, rbnode_it> rbtree;

public:
    void memory_insert(std::size_t ireq, std::size_t position)      //  where is from my request ?
    {
        auto hash_it = hashmap.find(ireq);
        if (hash_it == hashmap.end())
        {
            hshlist_t list_i {position}; //  create new list with the first element of near fp current key
            hashmap[ireq] = list_i;
        }
        else 
        {
            hash_it->second.push_back(position);
        }
    }

    belady_cache_t (std::size_t capacity_, std::size_t reqs, std::vector<std::size_t>& input_reqs) 
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
        if (cache_find(key))
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
    bool cache_find (const key_t key)
    {
        auto list_i = hashmap.find(key)->second;
        
        if (list_i.empty()) //  this kind if elements must not be in the cache, because it need to be deleted
        {
            hashmap.erase(key);
            return false;
        }

        auto fp_i = list_i.begin();
        if (rbtree.find(fp_i) == rbtree.end())
            return false;

        return true;    
    }

    template<typename func_t> void push_cache (const key_t key, const func_t slow_get_page)
    {
        auto fplist_i = hashmap.find(key)->second;

        if (fplist_i.empty())
        {
            hashmap.erase(key);
            return;
        }

        auto fp_i = fplist_i.begin();
        if (rbtree.empty())
        {
            rbtree.insert({fp_i, {key, slow_get_page(key)}});
            size++;
            return;
        }

        auto farest = std::prev(rbtree.end());
        if (fp_i >= farest_fp->first)
        {
            fplist_i.pop_front();
            return;
        }
        if (is_full())
        {
            rbtree.erase(farest);
            size--;
        }

        rbtree.insert({fp_i, {key, slow_get_page(key), fp_i}});
        size++;
    }

    void cache_update (const key_t key)
    {
        auto fplist_i = hashmap.find(key)->second;
        auto fp_i = fplist_i.begin();
        auto node_i = rbtree.find(fp_i)->second;

        fplist_i.pop_front();
        auto nextfp_i = fplist_i.begin();

        if (nextfp_i == fplist_i.end()) //  element won't be requested in the future
        {
            rbtree.erase(fp_i);
            hashmap.erase(key);
            size--;
        }

        //  else the element need be replaced in the cache
        rbtree.insert(nextfp_i, {key, node_i->page});
        rbtree.erase(fp_i);
    }
};
}   //  namespace of belady cache
