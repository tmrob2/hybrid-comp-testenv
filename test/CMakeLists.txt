cmake_minimum_required(VERSION 3.22)

set(This ThreadTest)

set(Sources ThreadTest.cpp)

add_executable(${This} ${Sources})

target_link_libraries(${This} PUBLIC
    gtest_main
    hybrid_computing
)

add_test(
    NAME ${This}
    COMMAND ${This}
)