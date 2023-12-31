#include "headers.hpp"

class CacheBlock
{
public:
    long long tag = 0;
    bool valid = 0;
    bool dirty = 0;
};

class Cache
{
public:
    int lines, associativity, offset_bits, index_bits, tag_bits, accesses;
    string writePolicy;
    vector<vector<CacheBlock *>> cache;
    static Cache *createCache(int cacheSize, int blockSize, int associativity, string replacementPolicy, string writePolicy);
    virtual bool find(long long tag, int index) = 0;
    virtual void insert(long long tag, int index) = 0;
    void access(long long address, char mode, ofstream &output);
    Cache(int lines, int associativity, int blockSize, string writePolicy);
};

class LRUCache : public Cache
{
public:
    LRUCache(int lines, int associativity, int blockSize, string writePolicy);
    void insert(long long tag, int index);
    bool find(long long tag, int index);
};

class FIFOCache : public Cache
{
public:
    FIFOCache(int lines, int associativity, int blockSize, string writePolicy);
    void insert(long long tag, int index);
    bool find(long long tag, int index);
};

class RandomCache : public Cache
{
public:
    RandomCache(int lines, int associativity, int blockSize, string writePolicy);
    void insert(long long tag, int index);
    bool find(long long tag, int index);
};

class LRUCacheBlock : public CacheBlock
{
public:
    int lru = 0;
};

class FIFOCacheBlock : public CacheBlock
{
public:
    int fifo = 0;
};

class RandomCacheBlock : public CacheBlock
{
};
