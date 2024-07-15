//
// Created by julian on 12.07.24.
//

#ifndef CACHELINE_H
#define CACHELINE_H
#include <systemc.h>

class CacheLine {

public:

    // --- CONSTRUCTOR --- //

    CacheLine() {};

    // --- FUNCTIONS --- //

    sc_uint<32> getTag() { return this->tag; }
    void setTag(sc_uint<32> newTag) { this->tag = newTag; }
    int getLru() { return this->lru; }
    void setLru(int newLru) { this->lru = newLru; }
    bool getValid() { return this->valid; }
    void setValid(bool newValid) { this->valid = newValid; }

private:
    // --- INTERNAL VARIABLES --- //

    // Tag of the cache line
    sc_uint<32> tag = 0;
    // Least Recently Used value of the cache line (used for replacement)
    int lru = 0;
    // Valid bit of the cache line (true if the cache line contains valid data)
    bool valid = false;

};

#endif

