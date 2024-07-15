//
// Created by julian on 13.07.24.
//

#include "cacheMemory.h"

// Constructor
CacheMemory::CacheMemory(const sc_module_name& name, unsigned cacheSize, unsigned cacheLineSize)
    : Memory(name, cacheLineSize), cacheSize(cacheSize)
{
    // Initialize the cache memory with the given size, by filling the data vektor with Bytes of 0x00
    for (unsigned i = 0; i < cacheSize; i++)
    {
        // The adresses of the cache are incrementet by 1 starting at zero
        addMemoryAt(i);
    }

    SC_CTHREAD(memoryAccessThread, clkCACHEMEMORYIn.pos());
    dont_initialize();
}

// This method is responsible for refilling the cache line with the data block read from the memory.
// Since the cache memorys data bus from the memory delivers only one block of data,
// the cache line is refilled by writing the data block into the corresponding bytes in the memory.
// No continuation is needed, since the data block is always the same size as the cache line.
void CacheMemory::refillCacheLine()
{
    // Reading the address, that indicates, where to start writing the new block
    sc_uint<32> startAddress = addressBusCACHEMEMORYIn.read();

    // Writing the data read from the memory into the corresponding bytes in the memory
    for (unsigned i = 0; i < getBlockSize(); i++)
    {
        addOrSetMemoryAt(startAddress + i, writeDataBusFromMemoryCACHEMEMORYIn[i].read());
    }
}

// This method is responsible for updating the cache line with the data read from the data bus
// Since the cache memorys data bus from the cpu delivers only one word of data,
// wich can span over multiple cache lines, the cache line is updated by writing the word into the corresponding bytes in the memory.
// Data is written bytewise, starting at the address given by the address bus, until the end of the cache line is reached.
// If the data spans over multiple cache lines, the cpu has to send a continuation address, where to write the rest of the data.
void CacheMemory::updateCacheLine()
{
    // Reading the address, that indicates, where to start writing the new block
    sc_uint<32> startAddress = addressBusCACHEMEMORYIn.read();
    sc_uint<32> continuationAddress = continuationAddressBusCACHEMEMORYIn.read();

    // Extract block size and offset bits information
    unsigned blockSize = getBlockSize();

    // Calculate the number of offset bits (log2 of block size)
    unsigned offsetMask = blockSize - 1;
    unsigned offsetBits = startAddress & offsetMask;

    // Calculate how many bytes can be written in the current cache line
    unsigned bytesToWriteInCurrentLine = blockSize - offsetBits;

    // Ensure we do not write more than 4 bytes in the first cache line
    if (bytesToWriteInCurrentLine > 4) {
        bytesToWriteInCurrentLine = 4;
    }

    // Write bytes to the current cache line
    for (unsigned i = 0; i < bytesToWriteInCurrentLine; i++)
    {
        addOrSetMemoryAt(startAddress + i, (writeDataBusFromCpuCACHEMEMORYIn.read() >> (8 * i)) & 0xFF);
    }

    // Write the remaining bytes to the continuation address
    unsigned remainingBytes = 4 - bytesToWriteInCurrentLine;
    for (unsigned i = 0; i < remainingBytes; i++)
    {
        addOrSetMemoryAt(continuationAddress + i, (writeDataBusFromCpuCACHEMEMORYIn.read() >> (8 * (i + bytesToWriteInCurrentLine))) & 0xFF);
    }
}

void CacheMemory::memoryAccessThread()
{
    while(true)
    {
        // Wait for delta cycle 2
        wait(SC_ZERO_TIME);
        // Wait for delta cycle 3
        wait(SC_ZERO_TIME);

        if(refillCACHEMEMORYIn.read())
        {
            refillCacheLine();
        }
        else if(updateCACHEMEMORYIn.read())
        {
            updateCacheLine();
        }
    }
}

void CacheMemory::printCacheMemory()
{

}



