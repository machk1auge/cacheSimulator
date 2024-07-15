//
// Created by julian on 15.07.24.
//

#ifndef CONTROLLER_H
#define CONTROLLER_H
#include <systemc.h>

#include "cacheLine.h"


class Controller : public sc_module
{
public:

    // --- INPUT PORTS --- //

    // Clock
    sc_in<bool> clkCONTROLLERIn;
    // AddressBus from CPU
    sc_in<sc_uint<32>> addressCONTROLLERIn;
    // Signal inidcating whether the CPU is reading
    sc_in<bool> readCONTROLLERIn;
    // Signal indicating whether the CPU is writing
    sc_in<bool> writeCONTROLLERIn;
    // Signal to flush the cache
    sc_in<bool> flushCONTROLLERIn;
    // Signal from the memory to indicate that the memory is done
    sc_in<bool> memoryReadyCONTROLLERIn;

    // --- OUTPUT PORTS --- //

    // AddressBus to the cache
    sc_out<sc_uint<32>> addressBusToCacheCONTROLLEROut;
    // Continuation of the addressBus to the cache, used, if the write operation spans two cache lines
    sc_out<sc_uint<32>> addressBusContinuationToCacheCONTROLLEROut;
    // Signal indicating to update the cache (used for the write trough policy)
    sc_out<bool> updateCacheCONTROLLEROut;
    // Signal indicating to refill the cache (used for writing new cache line)
    sc_out<bool> refillCacheCONTROLLEROut;
    // Signal indicating that the cpu has to stall until the cache is ready
    sc_out<bool> stallCONTROLLEROut;
    // AddressBus to the memory (contains the address of the block to be read)
    sc_out<sc_uint<32>> addressBusToMemoryCONTROLLEROut;
    // Signal indicating that the memory has to write data
    sc_out<bool> writeMemoryCONTROLLEROut;
    // Signal indicating that the memory has to read data
    sc_out<bool> readMemoryCONTROLLEROut;


    // --- METHODS --- //

    // Method ideling inside the controller (thread)
    virtual void controllerThread() = 0;
    // Method to extract the tag, index and offset from the address
    virtual void extractBits() = 0;
    unsigned getOffsetBits() { return offsetBits; }
    unsigned getIndexBits() { return indexBits; }
    unsigned getTagBits() { return tagBits; }
    unsigned getCacheLatency() { return cacheLatency; }
    unsigned getOffsetBitsOfAddress() { return offsetBitsOfAddress; }
    unsigned getIndexBitsOfAddress() { return indexBitsOfAddress; }
    unsigned getTagBitsOfAddress() { return tagBitsOfAddress; }
    unsigned getCachelineSize() { return cacheLineSize; }
    void setOffsetBitsOfAddress(unsigned offsetBitsOfAddress) { this->offsetBitsOfAddress = offsetBitsOfAddress; }
    void setIndexBitsOfAddress(unsigned indexBitsOfAddress) { this->indexBitsOfAddress = indexBitsOfAddress; }
    void setTagBitsOfAddress(unsigned tagBitsOfAddress) { this->tagBitsOfAddress = tagBitsOfAddress; }

    // --- CONSTRUCTOR --- //

    SC_HAS_PROCESS(Controller);
    Controller(sc_module_name name, unsigned const offsetBits, unsigned const indexBits, unsigned const tagBits, unsigned const latency, unsigned const cacheLineSize)
    : sc_module(name), offsetBits(offsetBits), indexBits(indexBits), tagBits(tagBits), cacheLatency(latency), cacheLineSize(cacheLineSize) {}

private:

    // --- INTERNAL VARIABLES --- //

    const unsigned offsetBits;
    const unsigned indexBits;
    const unsigned tagBits;
    unsigned offsetBitsOfAddress;
    unsigned indexBitsOfAddress;
    unsigned tagBitsOfAddress;
    const unsigned cacheLatency;
    const unsigned cacheLineSize;
};



#endif //CONTROLLER_H
