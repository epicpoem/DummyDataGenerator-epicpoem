#pragma once
#include <string>

class App {
public:
    App(int argc, char* argv[]);
    int run();

    static unsigned int parseSeed(int argc, char* argv[]);

private:
    unsigned int seed_;
    bool usingDefaultSeed_;

    void showMenu();
    void runGeneration(bool includeOrders);
    static std::string currentDate();
};
