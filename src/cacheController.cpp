//
// Created by julian on 12.07.24.
//

#include "cacheController.h"
#include <systemc.h>
#include <iostream>

// Parameterized constructor
CacheController::CacheController(sc_module_name name, unsigned cacheLines, unsigned cacheLineSize, unsigned cacheLatency)
    : sc_module(name), statistics({0,0,0,0,0}), bits({0,0,0}), cacheLineSize(cacheLineSize), memoryReadDataCACHEIn(cacheLineSize), cacheLatency(cacheLatency)
{
    // Create cache lines
    for (unsigned i = 0; i < cacheLines; ++i)
    {
        // Add a new cache line to the cache, with the given cache line size
        cacheLinesArray.emplace_back(cacheLineSize);
    }
}

// Destructor
CacheController::~CacheController()
{
}

// Print the number of bits for the cache
void CacheController::printBits()
{
    std::cout << "Bits of cache:" << std::endl;
    std::cout << "----------------------------------------" << std::endl;
    std::cout << "Offset: " << bits.offset << std::endl;
    std::cout << "Index: " << bits.index << std::endl;
    std::cout << "Tag: " << bits.tag << std::endl;
    std::cout << "----------------------------------------" << std::endl;
}

