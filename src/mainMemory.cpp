//
// Created by julian on 14.07.24.
//

#include "mainMemory.h"

MainMemory::MainMemory(const sc_module_name& name, unsigned blockSize, unsigned memoryLatency)
: Memory(name, blockSize), memoryLatency(memoryLatency)
{
    SC_CTHREAD(memoryAccessThread, clkMAINMEMORYIn.pos());
    dont_initialize();
}

// This method reads the data block from the memory and presents it to the cache
// It requires the block address to start reading from
void MainMemory::readDataBlock()
{
    //Address to start reading
    sc_uint<32> address = addressBusReadMAINMEMORYIn.read();

    // Align the address to the block size
    address = address & ~(getBlockSize() - 1);

    for (int i = 0; i < getBlockSize(); ++i)
    {
        dataBusReadMAINMEMORYOut[i].write(getOrAddMemoryAt(address + i));
    }
}

// This method writes the data word to the memory (directly from the CPU)
// It requires the address to start writing and the data word to write
void MainMemory::writeDataWord()
{
    // Address to start writing
    sc_uint<32> address = addressBusWriteMAINMEMORYIn.read();

    for (int i = 0; i < 4; i++)
    {
        addOrSetMemoryAt(address + i, (dataBusWriteMAINMEMORYIn.read() >> (8 * i) & 0xFF));
    }
}

void MainMemory::memoryAccessThread()
{
    while (true)
    {
        memoryReadyMAINMEMORYOut.write(false);
        if (readMAINMEMORYIn.read())
        {
            // This for loop is used to simulate the memory latency
            for (unsigned i = 0; i < getMemoryLatency() - 1; i++)
            {
                wait();
            }
            // In the last cycle the data block is read and presented to the cache
            readDataBlock();
        }
        else if (writeMAINMEMORYIn.read())
        {
            // This for loop is used to simulate the memory latency
            for (unsigned i = 0; i < getMemoryLatency() - 1; i++)
            {
                wait();
            }
            // In the last cycle the data word is written to the memory
            writeDataWord();
        }
        memoryReadyMAINMEMORYOut.write(true);
        wait();
    }
}

void MainMemory::printMainMemory()
{
}


