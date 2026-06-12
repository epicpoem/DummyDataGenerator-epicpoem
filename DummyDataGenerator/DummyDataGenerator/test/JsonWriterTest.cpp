#include <gtest/gtest.h>
#include <nlohmann/json.hpp>
#include <fstream>
#include <filesystem>
#include "JsonWriter.h"
#include "Generator.h"

namespace fs = std::filesystem;
using json   = nlohmann::json;

class JsonWriterTest : public ::testing::Test {
protected:
    const std::string testDir = "test_tmp_data";

    void SetUp()    override { fs::remove_all(testDir); }
    void TearDown() override { fs::remove_all(testDir); }

    std::vector<Sample> defaultSamples() {
        return Generator().generateSamples();
    }

    json readJson(const std::string& path) {
        std::ifstream f(path);
        return json::parse(f);
    }
};

// --- writeSamples ---

TEST_F(JsonWriterTest, WriteSamplesCreatesFile) {
    JsonWriter w(testDir);
    EXPECT_TRUE(w.writeSamples(defaultSamples(), WriteMode::OVERWRITE));
    EXPECT_TRUE(fs::exists(w.getSamplesPath()));
}

TEST_F(JsonWriterTest, WriteSamplesCreatesDirectoryIfMissing) {
    JsonWriter w(testDir + "/nested/path");
    EXPECT_TRUE(w.writeSamples(defaultSamples(), WriteMode::OVERWRITE));
    EXPECT_TRUE(fs::exists(w.getSamplesPath()));
}

TEST_F(JsonWriterTest, WriteSamplesOverwriteStoresCorrectCount) {
    JsonWriter w(testDir);
    w.writeSamples(defaultSamples(), WriteMode::OVERWRITE);
    w.writeSamples(defaultSamples(), WriteMode::OVERWRITE);

    auto arr = readJson(w.getSamplesPath());
    EXPECT_EQ(arr.size(), 5u);
}

TEST_F(JsonWriterTest, WriteSamplesOverwriteStoresCorrectFields) {
    JsonWriter w(testDir);
    w.writeSamples(defaultSamples(), WriteMode::OVERWRITE);

    auto arr = readJson(w.getSamplesPath());
    EXPECT_EQ(arr[0]["id"],    "S-001");
    EXPECT_EQ(arr[0]["stock"], 480);
}

TEST_F(JsonWriterTest, WriteSamplesAppendDoesNotDuplicateIds) {
    JsonWriter w(testDir);
    w.writeSamples(defaultSamples(), WriteMode::OVERWRITE);
    w.writeSamples(defaultSamples(), WriteMode::APPEND);

    auto arr = readJson(w.getSamplesPath());
    EXPECT_EQ(arr.size(), 5u);
}

TEST_F(JsonWriterTest, WriteSamplesAppendAddsNewEntries) {
    JsonWriter w(testDir);
    w.writeSamples(defaultSamples(), WriteMode::OVERWRITE);

    // Add a new sample with a different ID
    std::vector<Sample> extra = {{"S-099", "테스트 시료", 1.0, 0.9, 100}};  // avgProdTime=1.0
    w.writeSamples(extra, WriteMode::APPEND);

    auto arr = readJson(w.getSamplesPath());
    EXPECT_EQ(arr.size(), 6u);
}

// --- writeOrders ---

TEST_F(JsonWriterTest, WriteOrdersCreatesFile) {
    JsonWriter w(testDir);
    Generator gen(42);
    auto samples = gen.generateSamples();
    auto orders  = gen.generateOrders(samples, "20260612");

    EXPECT_TRUE(w.writeOrders(orders, WriteMode::OVERWRITE));
    EXPECT_TRUE(fs::exists(w.getOrdersPath()));
}

TEST_F(JsonWriterTest, WriteOrdersStoresCorrectStatus) {
    JsonWriter w(testDir);
    Generator gen(42);
    auto samples = gen.generateSamples();
    auto orders  = gen.generateOrders(samples, "20260612");
    w.writeOrders(orders, WriteMode::OVERWRITE);

    auto arr = readJson(w.getOrdersPath());
    EXPECT_FALSE(arr.empty());
    for (const auto& o : arr) {
        EXPECT_EQ(o["status"], "RESERVED");
    }
}

TEST_F(JsonWriterTest, WriteOrdersAppendAccumulatesEntries) {
    JsonWriter w(testDir);
    Generator gen(42);
    auto samples = gen.generateSamples();
    auto orders  = gen.generateOrders(samples, "20260612");

    w.writeOrders(orders, WriteMode::OVERWRITE);
    w.writeOrders(orders, WriteMode::APPEND);

    auto arr = readJson(w.getOrdersPath());
    EXPECT_EQ(arr.size(), orders.size() * 2);
}
