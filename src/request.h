//
// Created by julian on 13.07.24.
//

#ifndef REQUEST_H
#define REQUEST_H

class Request
{
public:
    Request();

    Request(unsigned address, unsigned data, unsigned type)
        : address(address), data(data), type(type) {}

    unsigned address;
    unsigned data;
    unsigned type;
};

#endif //REQUEST_H
