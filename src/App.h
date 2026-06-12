#pragma once
#include <string>

class App {
public:
    App(int argc, char* argv[]);
    int run();

private:
    unsigned int seed_;
    bool usingDefaultSeed_;

    unsigned int parseSeed(int argc, char* argv[]);
    void showMenu();
    void runGeneration(bool includeOrders);
    static std::string currentDate();
};
