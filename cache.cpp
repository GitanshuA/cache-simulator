#include "headers.hpp"
#include "cache.hpp"

Cache *Cache::createCache(int cacheSize, int blockSize, int associativity, string replacementPolicy, string writePolicy)
{
    if (associativity == 0)
    {
        associativity = cacheSize / blockSize;
    }
    int lines = cacheSize / (blockSize * associativity);
    if (replacementPolicy == "LRU")
    {
        return new LRUCache(lines, associativity, blockSize, writePolicy);
    }
    else if (replacementPolicy == "FIFO")
    {
        return new FIFOCache(lines, associativity, blockSize, writePolicy);
    }
    else if (replacementPolicy == "RANDOM")
    {
        return new RandomCache(lines, associativity, blockSize, writePolicy);
    }
    else
    {
        cout << "Invalid replacement policy" << endl;
        exit(0);
    }
}

Cache::Cache(int lines, int associativity, int blockSize, string writePolicy)
{
    this->lines = lines;
    this->associativity = associativity;
    this->writePolicy = writePolicy;
    this->accesses = 0;
    this->offset_bits = log2(blockSize);
    this->index_bits = log2(lines);
    this->tag_bits = 32 - offset_bits - index_bits;
}

LRUCache::LRUCache(int lines, int associativity, int blockSize, string writePolicy) : Cache(lines, associativity, blockSize, writePolicy)
{
    cache.resize(lines);
    for (int i = 0; i < lines; i++)
    {
        cache[i].resize(associativity);
        for (int j = 0; j < associativity; j++)
        {
            cache[i][j] = new LRUCacheBlock();
        }
    }
}

FIFOCache::FIFOCache(int lines, int associativity, int blockSize, string writePolicy) : Cache(lines, associativity, blockSize, writePolicy)
{
    cache.resize(lines);
    for (int i = 0; i < lines; i++)
    {
        cache[i].resize(associativity);
        for (int j = 0; j < associativity; j++)
        {
            cache[i][j] = new FIFOCacheBlock();
        }
    }
}

RandomCache::RandomCache(int lines, int associativity, int blockSize, string writePolicy) : Cache(lines, associativity, blockSize, writePolicy)
{
    srand(time(0));
    cache.resize(lines);
    for (int i = 0; i < lines; i++)
    {
        cache[i].resize(associativity);
        for (int j = 0; j < associativity; j++)
        {
            cache[i][j] = new RandomCacheBlock();
        }
    }
}

void Cache::access(long long address, char mode, ofstream &output)
{
    accesses++;
    int tag = address >> (index_bits + offset_bits);
    int index = (address >> offset_bits) & ((1 << index_bits) - 1);
    bool hit = 0;
    if (find(tag, index))
    {
        hit = 1;
    }
    else
    {
        if (mode == 'R')
            insert(tag, index);
        else if (mode == 'W' && writePolicy == "WB")
            insert(tag, index);
    }
    output << "Address: 0x" << address << ", Set: 0x" << index << ", " << (hit ? "Hit" : "Miss") << ", Tag: 0x" << tag << endl;
}

bool LRUCache::find(long long tag, int index)
{
    for (int i = 0; i < associativity; i++)
    {
        if (cache[index][i]->valid && cache[index][i]->tag == tag)
        {
            ((LRUCacheBlock *)(cache[index][i]))->lru = accesses;
            if (writePolicy == "WB")
            {
                cache[index][i]->dirty = 1;
            }
            return 1;
        }
    }
    return 0;
}

bool FIFOCache::find(long long tag, int index)
{
    for (int i = 0; i < associativity; i++)
    {
        if (cache[index][i]->valid && cache[index][i]->tag == tag)
        {
            if (writePolicy == "WB")
            {
                cache[index][i]->dirty = 1;
            }
            return 1;
        }
    }
    return 0;
}

bool RandomCache::find(long long tag, int index)
{
    for (int i = 0; i < associativity; i++)
    {
        if (cache[index][i]->valid && cache[index][i]->tag == tag)
        {
            if (writePolicy == "WB")
            {
                cache[index][i]->dirty = 1;
            }
            return 1;
        }
    }
    return 0;
}

void LRUCache::insert(long long tag, int index)
{
    int rep_ind = -1, mini = INT_MAX;
    for (int i = 0; i < associativity; i++)
    {
        if (!cache[index][i]->valid)
        {
            cache[index][i]->valid = 1;
            cache[index][i]->tag = tag;
            cache[index][i]->dirty = 0;
            ((LRUCacheBlock *)(cache[index][i]))->lru = accesses;
            return;
        }
        if (((LRUCacheBlock *)(cache[index][i]))->lru < mini)
        {
            mini = ((LRUCacheBlock *)(cache[index][i]))->lru;
            rep_ind = i;
        }
    }
    cache[index][rep_ind]->dirty = 0;
    cache[index][rep_ind]->tag = tag;
    ((LRUCacheBlock *)(cache[index][rep_ind]))->lru = accesses;
}

void FIFOCache::insert(long long tag, int index)
{
    int rep_ind = -1, mini = INT_MAX;
    for (int i = 0; i < associativity; i++)
    {
        if (!cache[index][i]->valid)
        {
            cache[index][i]->valid = 1;
            cache[index][i]->tag = tag;
            cache[index][i]->dirty = 0;
            ((FIFOCacheBlock *)(cache[index][i]))->fifo = accesses;
            return;
        }
        if (((FIFOCacheBlock *)(cache[index][i]))->fifo < mini)
        {
            mini = ((FIFOCacheBlock *)(cache[index][i]))->fifo;
            rep_ind = i;
        }
    }
    cache[index][rep_ind]->dirty = 0;
    cache[index][rep_ind]->tag = tag;
    ((FIFOCacheBlock *)(cache[index][rep_ind]))->fifo = accesses;
}

void RandomCache::insert(long long tag, int index)
{
    for (int i = 0; i < associativity; i++)
    {
        if (!cache[index][i]->valid)
        {
            cache[index][i]->valid = 1;
            cache[index][i]->tag = tag;
            cache[index][i]->dirty = 0;
            return;
        }
    }
    int rep_ind = rand() % associativity;
    cache[index][rep_ind]->dirty = 0;
    cache[index][rep_ind]->tag = tag;
}