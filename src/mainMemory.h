//
// Created by julian on 14.07.24.
//

#ifndef MAINMEMORY_H
#define MAINMEMORY_H
#include "memory.h"


class MainMemory: Memory {

    // --- INPUT PORTS --- //

    // Clock
    sc_in<bool> clkMAINMEMORYIn;
    // Port, reading the write address for write trough access from the cpu
    sc_in<sc_uint<32>> addressBusWriteMAINMEMORYIn;
    // Port, reading the read address for block reagin access from cache controller this. this address gets aligned to the block size
    sc_in<sc_uint<32>> addressBusReadMAINMEMORYIn;
    // Signal indicating to write the data word comming from the cpu
    sc_in<bool> writeMAINMEMORYIn;
    // Signal indicating to read block from the memory for the cache
    sc_in<bool> readMAINMEMORYIn;
    // Port containing the data word to write to the memory
    sc_in<sc_uint<32>> dataBusWriteMAINMEMORYIn;

    // --- OUTPUT PORTS --- //

    // Signal containing the data block read from the memory
    std::vector<sc_out<sc_uint<8>>> dataBusReadMAINMEMORYOut;
    // Signal indicating that the memory access is done
    sc_out<bool> memoryReadyMAINMEMORYOut;

    // --- METHODS --- //

    // Method reading data block and presenting it to the cache
    void readDataBlock();
    // Method writing data word to the memory
    void writeDataWord();
    // Method ideling and applying the main memory access
    void memoryAccessThread() override;
    // Method to print the main memory
    void printMainMemory();
    // Method to set the memory latency
    void setMemoryLatency(unsigned memoryLatency) { this->memoryLatency = memoryLatency; }
    // Method to get the memory latency
    unsigned getMemoryLatency() { return this->memoryLatency; }

    // --- CONSTRUCTOR --- //

    SC_HAS_PROCESS(MainMemory);

    MainMemory(const sc_module_name& name, unsigned blockSize, unsigned memoryLatency);

private:

    // --- INTERNAL VARIABLES --- //

    unsigned memoryLatency;
};



#endif //MAINMEMORY_H
