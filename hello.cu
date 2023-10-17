//
// Created by thomas on 25/09/23.
//

#include <cstdio>
#include "hello.h"

__global__ void sayHello() {
    printf("Hello world from the GPU!\n");
}

int helloTest() {
    printf("Hello world from the CPU!\n");

    sayHello<<<1,1>>>();
    cudaDeviceSynchronize();

    return 0;
}
