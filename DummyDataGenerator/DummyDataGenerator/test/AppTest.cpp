#include <gtest/gtest.h>
#include "App.h"
#include "Generator.h"

// --- App::parseSeed negative / boundary cases ---

TEST(AppParseSeedTest, NoArgumentsReturnsDefaultSeed) {
    int argc = 1;
    const char* argv[] = {"DummyDataGenerator"};
    EXPECT_EQ(App::parseSeed(argc, const_cast<char**>(argv)), Generator::DEFAULT_SEED);
}

TEST(AppParseSeedTest, ValidSeedArgumentIsParsed) {
    int argc = 3;
    const char* argv[] = {"DummyDataGenerator", "--seed", "42"};
    EXPECT_EQ(App::parseSeed(argc, const_cast<char**>(argv)), 42u);
}

TEST(AppParseSeedTest, NonIntegerSeedFallsBackToDefault) {
    int argc = 3;
    const char* argv[] = {"DummyDataGenerator", "--seed", "abc"};
    EXPECT_EQ(App::parseSeed(argc, const_cast<char**>(argv)), Generator::DEFAULT_SEED);
}

TEST(AppParseSeedTest, SeedFlagWithNoValueFallsBackToDefault) {
    // "--seed" is the last token — no following value
    int argc = 2;
    const char* argv[] = {"DummyDataGenerator", "--seed"};
    EXPECT_EQ(App::parseSeed(argc, const_cast<char**>(argv)), Generator::DEFAULT_SEED);
}

TEST(AppParseSeedTest, UnknownFlagIgnored) {
    int argc = 3;
    const char* argv[] = {"DummyDataGenerator", "--unknown", "42"};
    EXPECT_EQ(App::parseSeed(argc, const_cast<char**>(argv)), Generator::DEFAULT_SEED);
}

TEST(AppParseSeedTest, ZeroSeedIsAccepted) {
    // seed 0 is DEFAULT_SEED — parsing still succeeds
    int argc = 3;
    const char* argv[] = {"DummyDataGenerator", "--seed", "0"};
    EXPECT_EQ(App::parseSeed(argc, const_cast<char**>(argv)), Generator::DEFAULT_SEED);
}
