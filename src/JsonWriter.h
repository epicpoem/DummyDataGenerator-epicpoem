#pragma once
#include <string>
#include <vector>
#include "models/Sample.h"
#include "models/Order.h"

enum class WriteMode { OVERWRITE, APPEND };

class JsonWriter {
public:
    explicit JsonWriter(const std::string& dataDir = "data");

    bool writeSamples(const std::vector<Sample>& samples, WriteMode mode);
    bool writeOrders(const std::vector<Order>& orders,   WriteMode mode);

    const std::string& getSamplesPath() const { return samplesPath_; }
    const std::string& getOrdersPath()  const { return ordersPath_; }

private:
    std::string samplesPath_;
    std::string ordersPath_;

    bool ensureDirectory() const;
};
