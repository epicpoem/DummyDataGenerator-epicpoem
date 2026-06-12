#pragma once
#include <string>

struct Order {
    std::string id;
    std::string sampleId;
    std::string customerName;
    int quantity;
    std::string status;
    int actualProductionQuantity;
    std::string productionStartTime;
    double totalProductionTime;
};
