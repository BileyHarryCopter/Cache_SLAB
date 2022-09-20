# Cache_SLAB

## About Belady's cache implementation

1) Filling the hashmap from input by hash pair: key and iterator to the list with number of position element with the key in requests (FP = position in the future).
2) Cache space is a map (rbtree) which contains number of nodes less than cache's capacity. The nodes are placed by first future position (from left to right = from the nearest to the farest).
3) PUSH: 
   1) Cache is full. We need to evict the farest nodes in the future - access to the last element in the map, erase it from the hashmap by its key, erase it from the map and decrease the size of cache
   2) For requested page we access to it's node by key in hashmap and find the FP. After that there are 2 cases: if FP is more than other FP in the cache (we do not need in eviction - *in this cace we do not even need to evict a node if cache is full)*, and if FP belong to diapazon of cache FPs - insert in rbtree execute for O(ln N)

That is all about algorithm in general ideas