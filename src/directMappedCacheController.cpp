//
// Created by julian on 15.07.24.
//

#include "directMappedCacheController.h"

// Constructor implementation
DirectMappedCacheController::DirectMappedCacheController(sc_module_name name, unsigned cacheLines, unsigned cacheLineSize, unsigned cacheLatency)
    : Controller(
          name,
          static_cast<unsigned>(std::log2(cacheLineSize)),  // offsetBits
          static_cast<unsigned>(std::log2(cacheLines)),     // indexBits
          32 - static_cast<unsigned>(std::log2(cacheLineSize)) - static_cast<unsigned>(std::log2(cacheLines)), // tagBits
          cacheLatency,
          cacheLineSize),
      cacheLinesMetadata(cacheLines)
{}

// Extracts the bits from the address
void DirectMappedCacheController::extractBits()
{
    sc_uint<32> address = this->addressCONTROLLERIn.read();

    // Calculate masks based on offset and index bits
    unsigned offsetMask = (1 << getOffsetBits()) - 1;
    unsigned indexMask = (1 << getIndexBits()) - 1;

    // Extract offset bits
    setOffsetBitsOfAddress(address & offsetMask);

    // Extract index bits
    setIndexBitsOfAddress((address >> getOffsetBits()) & indexMask);

    // Extract tag bits
    setTagBitsOfAddress(address >> (getOffsetBits() + getIndexBits()));
}

// Thread implementation
void DirectMappedCacheController::controllerThread()
{
    while (true)
    {
        // Wait for delta cycle 2
        wait(SC_ZERO_TIME);

        // This if block handles the read and write fetching from memory part
        if (readCONTROLLERIn.read() || writeCONTROLLERIn.read())
        {
            stallCONTROLLEROut.write(true);

            // Await the cache latency
            for (unsigned i = 0; i < getCacheLatency() - 1; i++)
            {
                wait();
                // Wait for delta cycle 2
                wait(SC_ZERO_TIME);
            }

            // This method extracts the bits (offset, index, tag) from the address
            extractBits();

            unsigned index = getIndexBitsOfAddress();
            unsigned tag = getTagBitsOfAddress();
            unsigned offset = getOffsetBitsOfAddress();

            // Check if the data lies in one or two cache lines
            bool hit = cacheLinesMetadata[index].getValid() && (cacheLinesMetadata[index].getTag() == tag);

            if (!hit)
            {
                // Send the address to the memory
                addressBusToMemoryCONTROLLEROut.write(addressCONTROLLERIn.read());
                // Read the data from the memory
                readMemoryCONTROLLEROut.write(true);

                // Wait for the memory to finish reading the data block
                while (!memoryReadyCONTROLLERIn.read())
                {
                    wait();
                    // Wait for delta cycle 2
                    wait(SC_ZERO_TIME);
                }

                // Stop reading from memory
                readMemoryCONTROLLEROut.write(false);

                // Update the cache line metadata
                cacheLinesMetadata[index].setTag(tag);
                cacheLinesMetadata[index].setValid(true);

                // Refill the cache with the data from the memory
                addressBusToCacheCONTROLLEROut.write(index * getCachelineSize());
                refillCacheCONTROLLEROut.write(true);

                // Wait for delta cycle 3
                wait(SC_ZERO_TIME);

                // Stop refilling the cache
                refillCacheCONTROLLEROut.write(false);
            }

            // Handle the case when the data lies in two cache lines
            if (offset + 4 > getCachelineSize())
            {
                // Compute the index of the next cache line
                unsigned nextIndex = (index + 1) % cacheLinesMetadata.size();
                // Compute the tag of the next cache line, if the next cache line is the last one, the tag will be incremented
                unsigned nextTag = (index + 1 == cacheLinesMetadata.size()) ? (tag + 1) : tag;

                if (!(cacheLinesMetadata[nextIndex].getValid() && (cacheLinesMetadata[nextIndex].getTag() == nextTag)))
                {
                    // Calculate the address of the next cache line
                    sc_uint<32> nextAddress = addressCONTROLLERIn.read() + (getCachelineSize() - offset);

                    // Send the address to the memory
                    addressBusToMemoryCONTROLLEROut.write(nextAddress);
                    // Read the data from the memory
                    readMemoryCONTROLLEROut.write(true);

                    // Wait for the memory to finish reading the data block
                    while (!memoryReadyCONTROLLERIn.read())
                    {
                        wait();
                        // Wait for delta cycle 2
                        wait(SC_ZERO_TIME);
                    }

                    // Stop reading from memory
                    readMemoryCONTROLLEROut.write(false);

                    // Update the cache line metadata for the next cache line
                    cacheLinesMetadata[nextIndex].setTag(nextTag);
                    cacheLinesMetadata[nextIndex].setValid(true);

                    // Refill the cache with the data from the memory
                    addressBusToCacheCONTROLLEROut.write(nextIndex * getCachelineSize());
                    refillCacheCONTROLLEROut.write(true);

                    // Wait for delta cycle 3
                    wait(SC_ZERO_TIME);

                    // Stop refilling the cache
                    refillCacheCONTROLLEROut.write(false);
                }
            }
        }

        if (writeCONTROLLERIn.read())
        {
            // Write the data to the cache
            addressBusToCacheCONTROLLEROut.write(getIndexBitsOfAddress() * getCachelineSize() + getOffsetBitsOfAddress());
            updateCacheCONTROLLEROut.write(true);
            // Write the dato to the memory
            writeMemoryCONTROLLEROut.write(true);

            // TODO: Wait for the memory to finish writing the data block
            while (memoryReadyCONTROLLERIn.read() == false)
            {
                wait();
                // Wait for delta cycle 2
                wait(SC_ZERO_TIME);
                // Wait for delta cycle 3
                wait(SC_ZERO_TIME);
            }

            // Stop writing to the cache
            writeMemoryCONTROLLEROut.write(false);
            // Stop writing to the memory
            updateCacheCONTROLLEROut.write(false);

        }

        stallCONTROLLEROut.write(false);

        wait();
    }
}

