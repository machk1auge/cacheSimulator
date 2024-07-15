//
// Created by julian on 15.07.24.
//

#include "cpu.h"

Cpu::Cpu(sc_module_name name, Request* requests, unsigned requestCount)
: sc_module(name), requests(requests), requestCount(requestCount)
{
    SC_CTHREAD(cpuThread, clkCPUIn.pos());
    dont_initialize();
}

void Cpu::cpuThread()
{
    for (int i = 0; i < requestCount; ++i)
    {
        addressBusCPUOut.write(requests[i].address);
        readCPUOut.write(requests[i].type == 1);
        writeCPUOut.write(requests[i].type == 0);
        dataBusCPUOut.write(requests[i].data);

        // Wait for delta cycle 2
        wait(SC_ZERO_TIME);
        // Wait for delta cycle 3
        wait(SC_ZERO_TIME);
        // Wait for delta cycle 4
        wait(SC_ZERO_TIME);

        // Wait until the cache has finished the operation
        while (stallCPUIn.read())
        {
            wait();
            // Wait for delta cycle 2
            wait(SC_ZERO_TIME);
            // Wait for delta cycle 3
            wait(SC_ZERO_TIME);
            // Wait for delta cycle 4
            wait(SC_ZERO_TIME);
        }

        requests[i].data = dataCacheCPUIn.read();
    }

    // Stop the simulation
    sc_stop();
}