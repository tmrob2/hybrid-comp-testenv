//
// Created by thomas on 25/09/23.
//

#ifndef HYBRID_COMPUTING_CUSPARSE_H
#define HYBRID_COMPUTING_CUSPARSE_H

#include <cuda_runtime_api.h>// cudaMalloc, cudaMemcpy, etc.
#include <cusparse.h>         // cusparseSpMV
#include <cstdio>            // printf
#include <cstdlib>           // EXIT_FAILURE
#include <thrust/version.h>
#include <cusp/version.h>
#include <iostream>

namespace cusparsetest{
    int createMatrix();
    int cuspHello();
}

#endif //HYBRID_COMPUTING_CUSPARSE_H
