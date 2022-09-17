#pragma once

#include <cassert>
#include <map>
#include <list>
#include <vector>
#include <iterator>
#include <unordered_map>
#include <iostream>

namespace chc
{
    //  cache_t class: cache_cap, hashmap, list
enum {MAX_AGE = 100000};

template <typename page_t, typename key_t = int> class lfuda_t 
{
    std::size_t capacity;        //  capacity of cache
    std::size_t size = 0;        //  number of elements in cache
    std::size_t age = 0;         //  characteristic for realisation of LFUDA

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

    using llnode_it = typename std::list<llnode_t>::iterator;

    struct wlnode_t
    {
        std::size_t weight;
        std::list<llnode_t> local_list;                              //  nodes of local list with page
    };

    std::list<wlnode_t> wllist;                                      //   nodes of weight list with local list as node  
    std::unordered_map<key_t, llnode_it> hashmap;                       //   unordered_map
    std::map<std::size_t, wlnode_it> RBtree;             //   rbtree

    //  do I really nead iterator to hashmap and rbtree???
    //  using hashmap_it = typename std::unordered_map<page_t, key_t>::iterator;

public:
    lfuda_t (std::size_t capacity_) : capacity{capacity_} {};

    bool is_full () const { return capacity == size; }

    template <typename func> bool lookup_update (const key_t& key, func slow_get_page)
    {
        auto hit_it = hashmap.find(key);
        if (hit_it == hashmap.end())
        {
            //  insert new element to cache
            push_cache (key, slow_get_page);
            return false;
        }
        else 
        {
            //  any work with cache_element after hit to it
            cache_update (key);
            return true;
        };
    }

    void cache_dump (const key_t& key) const
    {
        std::cout << "Next key: " << key << '\n';
        std::cout << "Age of cache: " << age << '\n';

        auto wlnode_i = wllist.begin();
        while (wlnode_i != wllist.end())
        {
            std::cout << "WEIGHT: " << wlnode_i->weight << '\n';
            std::cout << '\t';

            auto llnode_i = wlnode_i->local_list.begin();
            while (llnode_i != wlnode_i->local_list.end())
            {
                std::cout << llnode_i->key << " -> ";
                llnode_i = std::next(llnode_i);
            }
            std::cout << "NULL\n";

            wlnode_i = std::next(wlnode_i);
        }
        std::cout << "\n\n\n";
    }

private:
    void is_age_reset () 
    {
        if (age > MAX_AGE)
            age = 0;
    }
    template <typename func> void push_cache (const key_t& key, const func slow_get_page) 
    {
        if (is_full())      //  this full about size of cache - not about size of hashmap
        {
            auto evict_node = std::prev(wllist.begin()->local_list.end());      //  access to iterator to the last node in the local list
            hashmap.erase(evict_node->key);  

            age = wllist.begin()->weight;

            wllist.begin()->local_list.erase(evict_node);
            if (wllist.begin()->local_list.empty())
            {
                RBtree.erase (wllist.begin()->weight);
                wllist.pop_front ();
            }

            //  What about arese in the RBtree, you're monster?!?!??!

            size--;
        }

        auto need_wlnode = find_need_wlnode (age + 1);
        need_wlnode->local_list.push_front({key, slow_get_page(key), need_wlnode});
        hashmap[key] = need_wlnode->local_list.begin();
        size++;
    }

    void cache_update (const key_t& key)
    {
        //  increase weight found llnode and move it into cache
        auto need_llnode = hashmap.find(key)->second;
        auto old_wlnode = need_llnode->parent;

        auto need_wlnode = find_need_wlnode (old_wlnode->weight + age + 1);
        need_wlnode->local_list.push_front({key, need_llnode->page, need_wlnode});
        hashmap[key] = need_wlnode->local_list.begin();

        //  changing a state of RBtree 
        RBtree.erase (old_wlnode->weight);
        RBtree [old_wlnode->weight + age + 1] = need_wlnode;

        old_wlnode->local_list.erase(need_llnode);
        if (old_wlnode->local_list.empty())
            wllist.erase(old_wlnode);

    }

    wlnode_it find_need_wlnode (const std::size_t weight)
    {
        auto is_need_wlnode = RBtree.find(weight);
        if (is_need_wlnode == RBtree.end())
        {
            //  create new wlnode with nececcery weight and add new element into rbtree
            wlnode_it need_wlnode;
            auto more_need_wlnode = RBtree.upper_bound(weight);

            if (more_need_wlnode == RBtree.end())
            {
                wllist.push_back({weight, {}});
                need_wlnode = std::prev(wllist.end());
            }
            else
            {
                //  insert need_wlnode before wlnode with upper_weight 
                need_wlnode = wllist.emplace (more_need_wlnode->second);
                need_wlnode->weight = weight;
            }

            RBtree.insert({weight, need_wlnode});

            return need_wlnode;
        }
        return is_need_wlnode->second;
    }
};

}   // namespace chc