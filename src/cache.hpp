#pragma once

#include <map>
#include <list>
#include <vector>
#include <iterator>
#include <unordered_map>

namespace chc
{
    //  cache_t class: cache_cap, hashmap, list
template <typename page_t, typename key_t = int> class lfuda_t 
{
    size_t capacity;        //  capacity of cache
    size_t size;            //  number of elements in cache
    size_t age;             //  characteristic for realisation of LFUDA

    //  wlist_t --- list of node with weight
    //  wlnode_t --- weight node contains pointer to appropriate local list 
    //  llist_t --- contains nodes with the same weight (parent for nodes of the local list)
    //  llnode_t --- contain data and pointer to parent

    struct wlnode_t;
    using wlnode_it = typename std::list<wlnode_t>::iterator;

    struct llnode_t
    {
        key_t key;
        page_t page;
        wlnode_it parent;
    };

    struct wlnode_t
    {
        int weight;
        std::list <llnode_t> local_list;           //  nodes of local list with page
    };

    std::list<wlnode_t> wllist;                   //  nodes of weight list with local list as node  
    std::unordered_map<key_t, page_t> hashmap;    // unordered_map

    using hashmap_it = typename std::unordered_map<page_t, key_t>::iterator;

public:
    lfuda_t (size_t capacity_) : capacity (capacity_);

    bool is_full () const { return capacity == size;}

    template <typename func> bool lookup_update (const key_t key, func slow_get_page)
    {
        int hit = hashmap.find (key);

        if (hit == hashmap.end())
        {
            cache_update (key, slow_get_page);  //  just moving to a new state of cache
                                                //  why void?
            return false;
        }
        else 
        {
            //  any working with cache_element after hit to it
            return true;
        };
    }

private:
    template <typename func> void cache_update (const key_t key, const func slow_get_page) 
    {
        auto low_wlist = wllist.begin();                        //  returns iterator of wlnode on begin of wllist

        if (is_full ())
        {
            auto evict_node = low_wlist->local_list.begin();    //  get iterator to node with the lowest weight
            low_wlist->local_list.erase (evict_node);           //  delete node in local_list
            hashmap.erase (evict_node->key);                    //  delete element in hashmap by its key
            size--;
        }
        
        //  get new page and insert it by its Wi = 1 + Age
        //  for this we need use rb tree
        if (low_list->local_list.empty())
            wllist.erase (low_list);

        //  insert for rbtree
    }

    
};

}   // namespace chc