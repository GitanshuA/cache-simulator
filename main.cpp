#include "headers.hpp"
#include "cache.hpp"

int main()
{
    ifstream config("cache.config");
    int cacheSize, blockSize, associativity;
    string replacementPolicy, writePolicy;
    config >> cacheSize >> blockSize >> associativity >> replacementPolicy >> writePolicy;
    Cache *cache = Cache::createCache(cacheSize, blockSize, associativity, replacementPolicy, writePolicy);
    config.close();
    ifstream input("cache.access");
    ofstream output("output.txt");
    long long address;
    char colon;
    char mode;
    output<<hex;
    input>>hex;
    while (input >> mode >> colon >> address)
    {
        cache->access(address, mode, output);
    }
    return 0;
}
