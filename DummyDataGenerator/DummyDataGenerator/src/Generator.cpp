#include "Generator.h"
#include <random>
#include <cstdio>

namespace {

struct SampleTemplate {
    const char* id;
    const char* name;
    double productionTime;
    double yield;
    int defaultStock;
};

// Fixed per spec; defaultStock matches spec example values (used for DEFAULT_SEED)
static const SampleTemplate SAMPLE_TEMPLATES[] = {
    {"S-001", "실리콘 웨이퍼-8인치", 0.5, 0.92, 480},
    {"S-002", "GaN 에피택셀-4인치",  0.3, 0.78, 220},
    {"S-003", "SiC 파워기판-6인치",  0.8, 0.92,  30},
    {"S-004", "포토레지스트-PR7",    0.2, 0.95, 910},
    {"S-005", "산화막 웨이퍼-SiO2", 0.6, 0.88,   0},
};

static const char* CUSTOMERS[] = {
    "삼성전자 파운드리",
    "SK하이닉스",
    "LG이노텍",
    "DB하이텍",
    "한미반도체",
    "네패스",
};

constexpr int SAMPLE_COUNT   = 5;
constexpr int CUSTOMER_COUNT = 6;
constexpr int ORDER_COUNT    = 3;

} // namespace

Generator::Generator(unsigned int seed) : seed_(seed) {}

std::vector<Sample> Generator::generateSamples() const {
    std::vector<Sample> samples;
    samples.reserve(SAMPLE_COUNT);

    if (seed_ == DEFAULT_SEED) {
        // DEFAULT_SEED always produces the spec example data exactly
        for (const auto& t : SAMPLE_TEMPLATES) {
            samples.push_back({t.id, t.name, t.productionTime, t.yield, t.defaultStock});
        }
        return samples;
    }

    std::mt19937 rng(seed_);
    std::uniform_int_distribution<int> stockDist(0, 999);

    for (const auto& t : SAMPLE_TEMPLATES) {
        samples.push_back({t.id, t.name, t.productionTime, t.yield, stockDist(rng)});
    }
    return samples;
}

std::vector<Order> Generator::generateOrders(const std::vector<Sample>& samples,
                                             const std::string& date) const {
    if (samples.empty()) return {};

    std::mt19937 rng(seed_);
    std::uniform_int_distribution<int> qtyDist(50, 500);
    std::uniform_int_distribution<int> sampleIdx(0, static_cast<int>(samples.size()) - 1);
    std::uniform_int_distribution<int> customerIdx(0, CUSTOMER_COUNT - 1);

    std::vector<Order> orders;
    orders.reserve(ORDER_COUNT);

    for (int i = 0; i < ORDER_COUNT; ++i) {
        char orderId[32];
        std::snprintf(orderId, sizeof(orderId), "ORD-%s-%04d", date.c_str(), i + 1);
        orders.push_back({
            orderId,
            samples[sampleIdx(rng)].id,
            CUSTOMERS[customerIdx(rng)],
            qtyDist(rng),
            "RESERVED"
        });
    }
    return orders;
}
