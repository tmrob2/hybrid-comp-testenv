cmake_minimum_required(VERSION 3.22)
set(This hybrid_computing)

project(${This} CXX C CUDA)

set(CMAKE_C_STANDARD 99)
set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CUDA_STANDARD 14)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_POSITION_INDEPENDENT_CODE ON)
set(CMAKE_CUDA_SEPARABLE_COMPILATION ON)

include(FetchContent)
FetchContent_Declare(
    googletest
    URL https://github.com/google/googletest/archive/03597a01ee50ed33e9dfd640b249b4be3799d395.zip
)
FetchContent_MakeAvailable(googletest)

find_package(CUDAToolkit REQUIRED)
find_package(XercesC REQUIRED)

enable_testing()

set(Headers library.h hello.h CuSparse.h)

set(Sources library.cpp hello.cu CuSparse.cpp)

add_subdirectory(test)
add_executable(hcMain main.cpp)

add_library(${This} STATIC ${Sources} ${Headers})
target_link_libraries(${This} PRIVATE CUDA::cudart CUDA::cusparse)

target_link_directories(hcMain PUBLIC ${Sources} ${Headers})
target_link_libraries(hcMain PRIVATE ${This} CUDA::cudart CUDA::cusparse)

