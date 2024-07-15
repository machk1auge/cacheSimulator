//
// Created by julian on 14.07.24.
//

#ifndef MEMORY_H
#define MEMORY_H
#include <systemc.h>
#include <unordered_map>


class Memory: public sc_module
{
public:

    // Method to set the value at a specific memory location, or add the location with value 0 if not present
    void addOrSetMemoryAt(sc_uint<32> address, sc_uint<8> value) {
        auto it = memory.find(address);
        if (it != memory.end()) {
            memory[address] = value;
        } else {
            addMemoryAt(address);
        }
    }

    // Method to get the value at a specific memory location, or add the location with value 0 if not present
    sc_uint<8> getOrAddMemoryAt(sc_uint<32> address) {
        auto it = memory.find(address);
        if (it != memory.end()) {
            return it->second;
        } else {
            addMemoryAt(address);
            return 0;
        }
    }

    // Method to add a memory location with value 0
    void addMemoryAt(sc_uint<32> address) {
        memory[address] = 0;
    }

    unsigned getBlockSize() const {
        return blockSize;
    }

    virtual void memoryAccessThread() = 0;

    SC_HAS_PROCESS(Memory);

    Memory(const sc_module_name& name, unsigned blockSize) : sc_module(name), blockSize(blockSize) {};


private:

    // --- INTERNAL VARIABLES --- //

    // Actual cache memory implemented as a unorded map of tuples containing addresses and their corresponding bytes because memory is addressed bytewise
    std::unordered_map<sc_uint<32>, sc_uint<8>> memory;
    // Block size (number of bytes in a memory block) is eqivalent to the cacheline size
    const unsigned blockSize;

};



#endif //MEMORY_H
