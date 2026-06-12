#pragma once
#include <vector>
#include <string>
#include "models/Sample.h"
#include "models/Order.h"

class Generator {
public:
    static constexpr unsigned int DEFAULT_SEED = 0;

    explicit Generator(unsigned int seed = DEFAULT_SEED);

    std::vector<Sample> generateSamples() const;
    std::vector<Order>  generateOrders(const std::vector<Sample>& samples,
                                       const std::string& date) const;

    unsigned int getSeed() const { return seed_; }
    bool isDefaultSeed() const   { return seed_ == DEFAULT_SEED; }

private:
    unsigned int seed_;
};
