# LFUDA and BELADY cache implementations

## For what?

Firstly, it's just interesting. Secondly, this is the first practice task on [C++ based course](https://sourceforge.net/projects/cpp-lects-rus/files/cpp-graduated) by [K.I.Vladimirov](https://github.com/tilir).

## LFUDA cache implementation with complexity O(logM)*

Main structure of cache represents realisation of structure from this [source](http://dhruvbird.com/lfu.pdf). But LFUDA decision suppose include a technology of dynamic aging, which needed of sorted list of weights (for more information check this [article](https://bparli.medium.com/enhancing-least-frequently-used-caches-with-dynamic-aging-64dc973d5857)). For this sorting it's be suggested to use red-black-tree from std::map.

## BELADY cache implementation with complexity O(logM)*

1) Filling the hashmap from input by hash pair: key and iterator to the list with number of position element with the key in requests (FP = position in the future).
2) Cache space is a map (rbtree) which contains number of nodes less than cache's capacity. The nodes are placed by first future position (from left to right = from the nearest to the farest).
3) PUSH: 
   1) Cache is full. We need to evict the farest nodes in the future - access to the last element in the map, erase it from the hashmap by its key, erase it from the map and decrease the size of cache
   2) For requested page we access to it's node by key in hashmap and find the FP. After that there are 2 cases: if FP is more than other FP in the cache (we do not need in eviction - *in this cace we do not even need to evict a node if cache is full)*, and if FP belong to diapazon of cache FPs - insert in rbtree execute for O(log N)

That is all about algorithm in general ideas

## Hot to install

1) First of all, clone repo:
   
```   
   git clone https://github.com/BileyHarryCopter/LFUDA_BELADY_CACHE.git
   cd LFUDA_BELADY_CACHE/
```

2) After that you can make:
```
   cmake -B build
   cd build/[cache]
   cmake --build .
```
Where [cache] can be **lfuda** or **belady**.

3) Congratulations! Now you have executable file which call [./cache]
   
## How to launch tests

1) From root directory move to test and write down:
```
   ./test.sh [cache] [N]
```
Where [cache] can be **lfuda** or **belady** and [N] - number of tests

## Comparison LFUDA with ideal cache BELADY

It is still not ready, because I want to do it special 

# NOTE
\* M - capacity of cache 