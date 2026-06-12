cmake_minimum_required(VERSION 3.14)
project(DummyDataGenerator VERSION 1.0.0)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

if(MSVC)
    add_compile_options(/utf-8)
endif()

# nlohmann/json - local single header (include/)
add_library(nlohmann_json INTERFACE)
target_include_directories(nlohmann_json INTERFACE ${CMAKE_SOURCE_DIR}/include)

# Google Test / Mock from local NuGet package (no internet needed)
set(GMOCK_NATIVE ${CMAKE_SOURCE_DIR}/packages/gmock.1.11.0/lib/native)

add_library(gtest STATIC
    ${GMOCK_NATIVE}/src/gtest/gtest-all.cc
)
target_include_directories(gtest PUBLIC ${GMOCK_NATIVE}/include)
target_compile_definitions(gtest PUBLIC GTEST_HAS_PTHREAD=0)
if(MSVC)
    target_compile_options(gtest PRIVATE /wd4996 /wd4244 /wd4267)
endif()

add_library(gmock STATIC
    ${GMOCK_NATIVE}/src/gmock/gmock-all.cc
)
target_include_directories(gmock PUBLIC ${GMOCK_NATIVE}/include)
target_link_libraries(gmock PUBLIC gtest)

add_library(gmock_main STATIC
    ${GMOCK_NATIVE}/src/gmock/src/gmock_main.cc
)
target_link_libraries(gmock_main PUBLIC gmock)

# Core library (shared between main exe and tests)
add_library(DummyDataGeneratorLib STATIC
    src/Generator.cpp
    src/JsonWriter.cpp
    src/App.cpp
)
target_include_directories(DummyDataGeneratorLib PUBLIC src)
target_link_libraries(DummyDataGeneratorLib PUBLIC nlohmann_json)

# Main executable
add_executable(DummyDataGenerator src/main.cpp)
target_link_libraries(DummyDataGenerator PRIVATE DummyDataGeneratorLib)

# Tests
enable_testing()
add_executable(DummyDataGeneratorTests
    test/GeneratorTest.cpp
    test/JsonWriterTest.cpp
    test/AppTest.cpp
)
target_link_libraries(DummyDataGeneratorTests PRIVATE DummyDataGeneratorLib gmock_main)
add_test(NAME DummyDataGeneratorTests COMMAND DummyDataGeneratorTests)
