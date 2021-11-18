project(opt_pair_tree)
cmake_minimum_required(VERSION 3.20)

add_library(nets STATIC
        src/static_optimal_net.cpp
        src/uniform_net.cpp
        src/requests.cpp
        src/net.cpp)

target_include_directories(nets PUBLIC $<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}/include>)

add_executable(${PROJECT_NAME} main.cpp)

target_include_directories(${PROJECT_NAME} PRIVATE include)
target_link_libraries(${PROJECT_NAME} nets)

enable_testing()
find_package(GTest REQUIRED)


add_executable(${PROJECT_NAME}-tests
        tests/uniform_net.cpp
        tests/optimal_net.cpp
        tests/net.cpp)

target_include_directories(${PROJECT_NAME}-tests PRIVATE include)
target_link_libraries(${PROJECT_NAME}-tests GTest::GTest GTest::Main nets)

add_test(NAME net-tests COMMAND ${PROJECT_NAME}-tests)

