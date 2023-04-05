#ifndef COMMUNICATION_BUS_H
#define COMMUNICATION_BUS_H

#include <gsl/span>
#include <cstdint>
#include <cstddef>
#include <vector>

class Bus {
    public:
    virtual ~Bus() {};
    
//    virtual void write(gsl::span<const uint8_t> data) = 0;
//    virtual void write_bytes(gsl::span<const uint8_t> data) = 0;
};

#endif