//
// Created by julian on 12.07.24.
//

#ifndef CACHECONTROLLER_H
#define CACHECONTRELLER_H

#include <systemc.h>
#include <vector>
#include "cacheLine.h"

struct statistics
{
    unsigned hits;
    unsigned misses;
    unsigned accesses;
    unsigned writes;
    unsigned reads;
};

struct bits
{
    unsigned offset;
    unsigned index;
    unsigned tag;
};

class CacheController : public sc_module
{
public:
    // --- INPUT PORTS --- //

    // Clock
    sc_in<bool> clkCACHEIn;
    // address to read/write
    sc_in<sc_uint<32>> cacheAddressCACHEIn;
    // data to write
    sc_in<sc_uint<32>> cacheWriteDataCACHEIn;
    // enable write
    sc_in<bool> cacheWriteEnableCACHEIn;
    // data that was read from memory
    std::vector<sc_in<sc_uint<8>>> memoryReadDataCACHEIn;
    // memory done signal that indicates that the memory has finished the request
    sc_in<bool> memoryDoneCACHEIn;

    // --- OUTPUT PORTS --- //

    // data that was read from cache retuning it to the CPU
    sc_out<sc_uint<8>> cacheReadDataCACHEOut;
    // cache done signal that indicates that the cache has finished the request
    sc_out<bool> cacheDoneCACHEOut;
    // address to read/write in memory
    sc_out<sc_uint<32>> memoryAddressCACHEOut;
    // data to write in memory
    sc_out<sc_uint<32>> memoryWriteDataCACHEOut;
    // enable write in memory
    sc_out<bool> memoryWriteEnableCACHEOut;
    // enable memory
    sc_out<bool> memoryEnableCACHEOut;

    // --- CONSTRUCTOR --- //
    
    SC_HAS_PROCESS(CacheController);

    // Paramerized constructor
    // name: name of the module, cacheLines: number of cache lines, cacheLineSize: size of the cache line, cacheLatency: latency of the cache
    CacheController(sc_module_name name, unsigned cacheLines, unsigned cacheLineSize, unsigned cacheLatency);
    virtual ~CacheController();

    // --- FUNCTIONS --- //

    // Pure virtual function that handles the cache access
    virtual void cacheAccess() = 0;
    // Pure virtual function that prints the cache
    virtual void printCache() = 0;
    // Pure virtual function that calculates the bits of the address
    virtual void calculateBits(unsigned cacheLines, unsigned cacheLineSize) = 0;

    // Function that prints the bits of the address
    void printBits();

    // --- GETTERS --- //
    struct statistics getStatistics() { return statistics; }
    struct bits getBits() { return bits; }

private:
    // --- INTERNAL VARIABLES --- //

    // Vector that contains the cache lines
    std::vector<CacheLine> cacheLinesArray;
    // size of the cache line (in bytes)
    unsigned cacheLineSize;
    // latency of the cache (in cycles)
    unsigned cacheLatency;
    // statistics of the cache
    struct statistics statistics;
    // struct that contains the bits of the address
    struct bits bits;
};

#endif