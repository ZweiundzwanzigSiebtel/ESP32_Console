#ifndef COMMUNICATION_BUS_H
#define COMMUNICATION_BUS_H

#include <cstdint>
#include <cstddef>
#include <vector>

class Bus {
    public:
    virtual ~Bus() {};
    
    virtual void write(uint8_t data) = 0;
    virtual void write_bytes(std::vector<uint8_t> data);
};

#endif