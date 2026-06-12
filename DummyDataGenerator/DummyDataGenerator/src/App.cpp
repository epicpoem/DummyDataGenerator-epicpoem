#include "App.h"
#include "Generator.h"
#include "JsonWriter.h"
#include <iostream>
#include <sstream>
#include <iomanip>
#include <ctime>
#include <limits>

#ifdef _WIN32
#include <windows.h>
#endif

App::App(int argc, char* argv[])
    : seed_(parseSeed(argc, argv))
    , usingDefaultSeed_(seed_ == Generator::DEFAULT_SEED)
{}

unsigned int App::parseSeed(int argc, char* argv[]) {
    for (int i = 1; i + 1 < argc; ++i) {
        if (std::string(argv[i]) == "--seed") {
            try {
                long val = std::stol(argv[i + 1]);
                return static_cast<unsigned int>(val);
            } catch (...) {
                std::cerr << "[경고] --seed 값이 정수가 아닙니다. 기본 시드를 사용합니다.\n";
            }
        }
    }
    return Generator::DEFAULT_SEED;
}

std::string App::currentDate() {
    std::time_t t = std::time(nullptr);
    std::tm tm{};
#ifdef _WIN32
    localtime_s(&tm, &t);
#else
    localtime_r(&t, &tm);
#endif
    std::ostringstream oss;
    oss << std::put_time(&tm, "%Y%m%d");
    return oss.str();
}

int App::run() {
#ifdef _WIN32
    SetConsoleOutputCP(CP_UTF8);
    SetConsoleCP(CP_UTF8);
#endif

    std::cout << "=== DummyDataGenerator ===\n";
    std::cout << "사용 시드: " << seed_;
    if (usingDefaultSeed_) std::cout << " (기본 시드)";
    std::cout << "\n\n";

    showMenu();
    return 0;
}

void App::showMenu() {
    while (true) {
        std::cout << "[1] 시료 데이터만 생성\n"
                  << "[2] 시료 + 주문 데이터 생성\n"
                  << "[0] 종료\n"
                  << "선택: ";

        int choice = -1;
        if (!(std::cin >> choice)) {
            std::cin.clear();
            std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
            continue;
        }
        std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');

        if (choice == 0) break;
        if (choice == 1 || choice == 2) {
            runGeneration(choice == 2);
        } else {
            std::cout << "올바른 번호를 입력하세요.\n\n";
        }
    }
    std::cout << "종료합니다.\n";
}

void App::runGeneration(bool includeOrders) {
    std::cout << "\n저장 방식을 선택하세요:\n"
              << "[1] 덮어쓰기 (기존 데이터 삭제 후 저장)\n"
              << "[2] 추가    (기존 데이터 유지하고 추가)\n"
              << "[0] 취소\n"
              << "선택: ";

    int choice = -1;
    if (!(std::cin >> choice)) {
        std::cin.clear();
        std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
    }
    std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');

    if (choice == 0) {
        std::cout << "취소되었습니다.\n\n";
        return;
    }

    WriteMode mode = (choice == 2) ? WriteMode::APPEND : WriteMode::OVERWRITE;

    Generator gen(seed_);
    JsonWriter writer;

    auto samples = gen.generateSamples();
    if (!writer.writeSamples(samples, mode)) {
        std::cerr << "[오류] 시료 데이터 저장 실패.\n\n";
        return;
    }

    int orderCount = 0;
    if (includeOrders) {
        auto orders = gen.generateOrders(samples, currentDate());
        if (!writer.writeOrders(orders, mode)) {
            std::cerr << "[오류] 주문 데이터 저장 실패.\n\n";
            return;
        }
        orderCount = static_cast<int>(orders.size());
    }

    std::cout << "\n[완료]\n"
              << "시료 " << samples.size() << "건 저장: " << writer.getSamplesPath() << "\n";
    if (includeOrders) {
        std::cout << "주문 " << orderCount << "건 저장: " << writer.getOrdersPath() << "\n";
    }
    std::cout << "\n";
}
