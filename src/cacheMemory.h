//
// Created by julian on 13.07.24.
//

#ifndef CACHEMEMORY_H
#define CACHEMEMORY_H
#include <systemc.h>
#include "memory.h"

class CacheMemory: Memory
{
public:

    // --- INPUT PORTS --- //

    // Clock
    sc_in<bool> clkCACHEMEMORYIn;
    // Port, reading the starting address from wich a read or write is starting
    sc_in<sc_uint<32>> addressBusCACHEMEMORYIn;
    // Port, reading the address to wich the cpu wants to jump in case that the data to write spans over multiple cachelines
    sc_in<sc_uint<32>> continuationAddressBusCACHEMEMORYIn;
    // Signal indicating to refill cache line, this means writing block of data read from the memory
    sc_in<bool> refillCACHEMEMORYIn;
    // Signal indicating to update chache line, this means writing the 32 bit word comming from the cpu
    sc_in<bool> updateCACHEMEMORYIn;
    // Port containing the data block read from the memory
    std::vector<sc_in<sc_uint<8>>> writeDataBusFromMemoryCACHEMEMORYIn;
    // Port containing the data of of writeDataBus wich is the write trough data from the cpu
    sc_in<sc_uint<32>> writeDataBusFromCpuCACHEMEMORYIn;

    // --- OUTPUT PORTS --- //

    // Signal containing the data word read from the cache wich is the send to the cpu
    sc_out<sc_uint<32>> outputDataBusCACHEMEMORYOut;

    // --- METHODS --- //

    // Refill method is responsible for refilling the cache line with the data block read from the memory
    void refillCacheLine();
    // Update method is responsible for updating the cache line with the data read from the data bus
    void updateCacheLine();
    // Method ideling and applying the cache memory access
    void memoryAccessThread() override;
    // Method to print the cache memory
    void printCacheMemory();

    // --- CONSTRUCTOR --- //

    SC_HAS_PROCESS(CacheMemory);

    CacheMemory(const sc_module_name& name, unsigned cacheSize, unsigned cacheLineSize);

private:

    // --- INTERNAL VARIABLES --- //

    // Number of cachelines of the cache
    unsigned cacheSize;
};



#endif //CACHEMEMORY_H
