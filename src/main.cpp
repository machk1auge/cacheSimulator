//
// Created by julian on 12.07.24.
//
#include <systemc.h>
#include "cpu.hpp"
#include "memory.h"
#include "cache.h"
#include "directMappedCache.h"
#include "fourwayMappedCache.h"

// Global variables
int directMapped = 1;
unsigned cacheLines = 16;
unsigned cacheLineSize = 4;
unsigned cacheLatency = 5;
unsigned memoryLatency = 10;
unsigned numRequests = 10;
unsigned cycles = 1000;
Request* requests = nullptr;

// Initialize the requests
void initializeRequests() {
    requests = new Request[numRequests];
    requests[0] = Request(0, 0, 1);
    requests[1] = Request(1, 1, 1);
    requests[2] = Request(2, 2, 1);
    requests[3] = Request(3, 3, 1);
    requests[4] = Request(4, 4, 1);
    requests[5] = Request(5, 5, 1);
    requests[6] = Request(6, 6, 1);
    requests[7] = Request(7, 7, 1);
    requests[8] = Request(8, 8, 1);
    requests[9] = Request(9, 9, 1);
}

// Free the requests
void freeRequests() {
    delete[] requests;
    requests = nullptr;
}

int sc_main(int argc, char *argv[])
{
    initializeRequests();

    // ----- Clock ----- //
    sc_clock clk("clk", 1, SC_NS);

    // ----- Cache ----- //
    // Create a cache object depending on the directMapped flag
    Cache *cache;
    if (directMapped)
    {
        cache = new DirectMappedCache("cache_inst", cacheLines, cacheLineSize, cacheLatency);
    }
    else
    {
        cache = new FourwayMappedCache("cache_inst", cacheLines, cacheLineSize, cacheLatency);
    }
    cache->clkCACHEIn(clk);

    // ----- CPU ----- //
    Cpu cpu("cpu_inst", requests, numRequests);
    cpu.clkCPUIn(clk);

    // ----- Memory ----- //
    Memory memory("memory_inst", memoryLatency, cacheLineSize);
    memory.clkMEMORYIn(clk);

    // ----- Signals Cache ----- //

    // - Cache done signal
    sc_signal<bool> cacheDoneSignal;
    cpu.cacheDoneCPUIn(cacheDoneSignal);
    cache->cacheDoneCACHEOut(cacheDoneSignal);
    // - Address of the next request
    sc_signal<sc_uint<32>> cacheAdressSignal;
    cpu.addressCPUOut(cacheAdressSignal);
    cache->cacheAddressCACHEIn(cacheAdressSignal);
    // - Value of the next request
    sc_signal<sc_uint<32>> cacheWriteDataSignal;
    cpu.writeDataCPUOut(cacheWriteDataSignal);
    cache->cacheWriteDataCACHEIn(cacheWriteDataSignal);
    // - Write or read request
    sc_signal<bool> cacheWriteEnableSignal;
    cpu.writeEnableCPUOut(cacheWriteEnableSignal);
    cache->cacheWriteEnableCACHEIn(cacheWriteEnableSignal);
    // - Cache read data
    sc_signal<sc_uint<8>> cacheReadDataSignal;
    cpu.cacheReadDataCPUIn(cacheReadDataSignal);
    cache->cacheReadDataCACHEOut(cacheReadDataSignal);

    // ----- Signals Memory ----- //

    // - Memory done signal
    sc_signal<bool> memoryDoneSignal;
    cache->memoryDoneCACHEIn(memoryDoneSignal);
    memory.doneMEMORYOut(memoryDoneSignal);
    // - Address of the memory request
    sc_signal<sc_uint<32>> memoryAddressSignal;
    cache->memoryAddressCACHEOut(memoryAddressSignal);
    memory.addressMEMORYIn(memoryAddressSignal);
    // - Value of the memory write request
    sc_signal<sc_uint<32>> memoryWriteDataSignal;
    cache->memoryWriteDataCACHEOut(memoryWriteDataSignal);
    memory.writeDataMEMORYIn(memoryWriteDataSignal);
    // - Write memory signal
    sc_signal<bool> memoryWriteEnableSignal;
    cache->memoryWriteEnableCACHEOut(memoryWriteEnableSignal);
    memory.writeEnableMEMORYIn(memoryWriteEnableSignal);
    // - Memory enable signal
    sc_signal<bool> memoryEnableSignal;
    cache->memoryEnableCACHEOut(memoryEnableSignal);
    memory.enableMEMORYIn(memoryEnableSignal);

    // - Memory read data
    std::vector<sc_signal<sc_uint<8>>> memoryReadDataSignals(cacheLineSize);
    for (size_t i = 0; i < cacheLineSize; i++) {
        cache.memoryReadDataCACHEIn[i](memoryReadDataSignals[i]);
        memory.readDataMEMORYOut[i](memoryReadDataSignals[i]);
    }

    // Start the simulation with the given number of cycles
    sc_start(cycles, SC_NS);

    // Clean up
    delete cache;
    freeRequests();

    return 0;
}