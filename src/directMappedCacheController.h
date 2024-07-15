//
// Created by julian on 15.07.24.
//

#ifndef DIRECTMAPPEDCACHECONTROLLER_H
#define DIRECTMAPPEDCACHECONTROLLER_H
#include "controller.h"


class DirectMappedCacheController: public Controller{
public:

    // --- METHODS --- //

    void extractBits() override;
    void controllerThread() override;

    // --- CONSTRUCTOR --- //

    SC_CTOR(DirectMappedCacheController);
    DirectMappedCacheController(sc_module_name name, unsigned cacheLines, unsigned cacheLineSize, unsigned cacheLatency);

private:

    // --- INTERNAL VARIABLES --- //

    // Vector containing the cache lines metadata (LRU, valid, tag)
    std::vector<CacheLine> cacheLinesMetadata;
};



#endif //DIRECTMAPPEDCACHECONTROLLER_H
