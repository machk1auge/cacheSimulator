//
// Created by julian on 15.07.24.
//

#ifndef CPU_H
#define CPU_H
#include <systemc.h>

#include "request.h"


SC_MODULE(Cpu)
{
public:

    // --- INPUT PORTS --- //

    // Clock signal
    sc_in<bool> clkCPUIn;
    // Signla indicating that the cpu has to stall
    sc_in<bool> stallCPUIn;
    // Port receiving the data from the cache
    sc_in<sc_uint<32>> dataCacheCPUIn;

    // --- OUTPUT PORTS --- //

    // Bus containing the address of the memory to be accessed
    sc_out<sc_uint<32>> addressBusCPUOut;
    // Signal indicating whether the instruction is a read
    sc_out<bool> readCPUOut;
    // Signal indicating whether the instruction is a write
    sc_out<bool> writeCPUOut;
    // Signal flushing the cache
    sc_out<bool> flushCPUOut;
    // DataBus containing the data to be written
    sc_out<sc_uint<32>> dataBusCPUOut;

    // --- METHODS --- //

    void cpuThread();

    // --- CONSTRUCTOR --- //

    SC_CTOR(Cpu);

    Cpu(sc_module_name name, Request* requests, unsigned requestCount);

    private:

    // --- INTERNAL VARIABLES --- //
    Request* requests;
    unsigned requestCount;

};



#endif //CPU_H
