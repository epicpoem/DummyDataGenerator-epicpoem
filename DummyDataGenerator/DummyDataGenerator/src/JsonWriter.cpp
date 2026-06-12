#include "JsonWriter.h"
#include <nlohmann/json.hpp>
#include <fstream>
#include <filesystem>

using json = nlohmann::json;
namespace fs = std::filesystem;

JsonWriter::JsonWriter(const std::string& dataDir)
    : samplesPath_(dataDir + "/samples.json")
    , ordersPath_(dataDir + "/orders.json")
{}

bool JsonWriter::ensureDirectory() const {
    fs::path dir = fs::path(samplesPath_).parent_path();
    if (!fs::exists(dir)) {
        return fs::create_directories(dir);
    }
    return true;
}

static json toJson(const Sample& s) {
    return {
        {"id",          s.id},
        {"name",        s.name},
        {"avgProdTime", s.avgProdTime},
        {"yield",       s.yield},
        {"stock",       s.stock}
    };
}

static json toJson(const Order& o) {
    return {
        {"id",                       o.id},
        {"sampleId",                 o.sampleId},
        {"customerName",             o.customerName},
        {"quantity",                 o.quantity},
        {"status",                   o.status},
        {"actualProductionQuantity", o.actualProductionQuantity},
        {"productionStartTime",      o.productionStartTime},
        {"totalProductionTime",      o.totalProductionTime}
    };
}

bool JsonWriter::writeSamples(const std::vector<Sample>& samples, WriteMode mode) {
    if (!ensureDirectory()) return false;

    json arr = json::array();

    if (mode == WriteMode::APPEND && fs::exists(samplesPath_)) {
        std::ifstream in(samplesPath_);
        if (in.is_open()) {
            try { arr = json::parse(in); }
            catch (...) { arr = json::array(); }
        }
    }

    for (const auto& s : samples) {
        // Skip duplicates by ID in append mode
        bool dup = false;
        for (const auto& existing : arr) {
            if (existing["id"] == s.id) { dup = true; break; }
        }
        if (!dup) arr.push_back(toJson(s));
    }

    std::ofstream out(samplesPath_);
    if (!out.is_open()) return false;
    out << arr.dump(2);
    return true;
}

bool JsonWriter::writeOrders(const std::vector<Order>& orders, WriteMode mode) {
    if (!ensureDirectory()) return false;

    json arr = json::array();

    if (mode == WriteMode::APPEND && fs::exists(ordersPath_)) {
        std::ifstream in(ordersPath_);
        if (in.is_open()) {
            try { arr = json::parse(in); }
            catch (...) { arr = json::array(); }
        }
    }

    for (const auto& o : orders) {
        arr.push_back(toJson(o));
    }

    std::ofstream out(ordersPath_);
    if (!out.is_open()) return false;
    out << arr.dump(2);
    return true;
}
