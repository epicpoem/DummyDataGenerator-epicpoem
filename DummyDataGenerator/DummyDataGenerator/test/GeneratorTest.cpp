#include <gtest/gtest.h>
#include <limits>
#include "Generator.h"

// --- generateSamples ---

TEST(GeneratorTest, DefaultSeedProducesSpecExampleData) {
    Generator gen;
    auto samples = gen.generateSamples();

    ASSERT_EQ(samples.size(), 5u);
    EXPECT_EQ(samples[0].id,    "S-001");
    EXPECT_EQ(samples[0].stock, 480);
    EXPECT_EQ(samples[1].stock, 220);
    EXPECT_EQ(samples[2].stock,  30);
    EXPECT_EQ(samples[3].stock, 910);
    EXPECT_EQ(samples[4].stock,   0);
}

TEST(GeneratorTest, AlwaysProducesFiveSamples) {
    EXPECT_EQ(Generator().generateSamples().size(),       5u);
    EXPECT_EQ(Generator(42).generateSamples().size(),     5u);
    EXPECT_EQ(Generator(99999).generateSamples().size(),  5u);
}

TEST(GeneratorTest, SameSeedProducesSameStockValues) {
    auto s1 = Generator(12345).generateSamples();
    auto s2 = Generator(12345).generateSamples();

    ASSERT_EQ(s1.size(), s2.size());
    for (size_t i = 0; i < s1.size(); ++i) {
        EXPECT_EQ(s1[i].stock, s2[i].stock);
    }
}

TEST(GeneratorTest, DifferentSeedsProduceDifferentStockValues) {
    auto s1 = Generator(1).generateSamples();
    auto s2 = Generator(2).generateSamples();

    bool anyDiff = false;
    for (size_t i = 0; i < s1.size(); ++i) {
        if (s1[i].stock != s2[i].stock) { anyDiff = true; break; }
    }
    EXPECT_TRUE(anyDiff);
}

TEST(GeneratorTest, FixedFieldsAreIdenticalAcrossAllSeeds) {
    auto s1 = Generator().generateSamples();
    auto s2 = Generator(99999).generateSamples();

    ASSERT_EQ(s1.size(), s2.size());
    for (size_t i = 0; i < s1.size(); ++i) {
        EXPECT_EQ(s1[i].id,                  s2[i].id);
        EXPECT_EQ(s1[i].name,                s2[i].name);
        EXPECT_DOUBLE_EQ(s1[i].avgProdTime, s2[i].avgProdTime);
        EXPECT_DOUBLE_EQ(s1[i].yield,        s2[i].yield);
    }
}

TEST(GeneratorTest, StockValuesAreInValidRange) {
    auto samples = Generator(42).generateSamples();
    for (const auto& s : samples) {
        EXPECT_GE(s.stock, 0);
        EXPECT_LE(s.stock, 999);
    }
}

// --- generateOrders ---

TEST(GeneratorTest, AllGeneratedOrdersHaveReservedStatus) {
    Generator gen(42);
    auto samples = gen.generateSamples();
    auto orders  = gen.generateOrders(samples, "20260612");

    EXPECT_FALSE(orders.empty());
    for (const auto& o : orders) {
        EXPECT_EQ(o.status, "RESERVED");
    }
}

TEST(GeneratorTest, SameSeedProducesSameOrders) {
    auto samples = Generator(42).generateSamples();
    auto o1 = Generator(42).generateOrders(samples, "20260612");
    auto o2 = Generator(42).generateOrders(samples, "20260612");

    ASSERT_EQ(o1.size(), o2.size());
    for (size_t i = 0; i < o1.size(); ++i) {
        EXPECT_EQ(o1[i].sampleId,     o2[i].sampleId);
        EXPECT_EQ(o1[i].customerName, o2[i].customerName);
        EXPECT_EQ(o1[i].quantity,     o2[i].quantity);
    }
}

TEST(GeneratorTest, OrderIdContainsSuppliedDate) {
    auto samples = Generator().generateSamples();
    auto orders  = Generator().generateOrders(samples, "20260612");

    for (const auto& o : orders) {
        EXPECT_NE(o.id.find("20260612"), std::string::npos);
    }
}

TEST(GeneratorTest, OrderSampleIdBelongsToSampleList) {
    Generator gen(42);
    auto samples = gen.generateSamples();
    auto orders  = gen.generateOrders(samples, "20260612");

    for (const auto& o : orders) {
        bool found = false;
        for (const auto& s : samples) {
            if (s.id == o.sampleId) { found = true; break; }
        }
        EXPECT_TRUE(found) << "Unknown sampleId: " << o.sampleId;
    }
}

TEST(GeneratorTest, EmptySamplesProducesNoOrders) {
    auto orders = Generator(42).generateOrders({}, "20260612");
    EXPECT_TRUE(orders.empty());
}

// --- Negative / boundary cases ---

TEST(GeneratorTest, MaxUintSeedProducesValidSamples) {
    auto samples = Generator(std::numeric_limits<unsigned int>::max()).generateSamples();
    ASSERT_EQ(samples.size(), 5u);
    for (const auto& s : samples) {
        EXPECT_GE(s.stock, 0);
        EXPECT_LE(s.stock, 999);
    }
}

TEST(GeneratorTest, AllOrderQuantitiesInValidRange) {
    Generator gen(42);
    auto samples = gen.generateSamples();
    auto orders  = gen.generateOrders(samples, "20260612");

    EXPECT_FALSE(orders.empty());
    for (const auto& o : orders) {
        EXPECT_GE(o.quantity, 50);
        EXPECT_LE(o.quantity, 500);
    }
}

TEST(GeneratorTest, NewOrderFieldsHaveDefaultValues) {
    Generator gen(42);
    auto samples = gen.generateSamples();
    auto orders  = gen.generateOrders(samples, "20260612");

    for (const auto& o : orders) {
        EXPECT_EQ(o.actualProductionQuantity, 0);
        EXPECT_TRUE(o.productionStartTime.empty());
        EXPECT_DOUBLE_EQ(o.totalProductionTime, 0.0);
    }
}
