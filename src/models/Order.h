#pragma once
#include <string>

struct Order {
    std::string orderId;
    std::string sampleId;
    std::string customer;
    int quantity;
    std::string status;
};
