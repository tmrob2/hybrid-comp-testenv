//
// Created by thomas on 23/09/23.
//
#include "library.h"
#include "hello.h"
#include "CuSparse.h"
#include <iostream>

// todo the next stage is to implement multiple threads and construct a dispatcher to those
//  threads.

void test1() {
    std::vector<double> x = {1., 2., 3., 4., 5.};
    std::vector<double> y = {2., 3., 4., 5., 6.};

    auto looper = std::make_unique<mythread::CLooper>(0);

    looper->run();
    // The dispatchers will need to be looped over in the multi-threaded case.
    while (!looper->running()) {
        // do nothing
    }

    std::cout << "Thread started!\n";

    auto dispatcher = looper->getDispatcher();


    uint countDown = x.size();
    std::cout << "Count down size: " << countDown << "\n";
    // The dispatcher has to wait to accept the task

    while (countDown > 0) {
        uint k = countDown - 1;

        mythread::Problem task = mythread::Problem(k, x[k], y[k]);

        if(dispatcher->post(std::move(task))) {
            --countDown;
        } else {
            std::cout << "Thread looper busy or not running\n";
        }
    }
    std::vector<std::pair<int, double>> finalSolution;
    while(!looper->solutionsReady()) {
        // do nothing
    }
    finalSolution = looper->getSolution();

    std::cout << "final solution: ";
    for (auto val: finalSolution) {
        std::cout << "(" << val.first << ", " << val.second << "), ";
    }
    std::cout << "\n";

    std::cout << "Got to the sorting stage\n";
    std::sort(finalSolution.begin(), finalSolution.end(), [](const auto& left, const auto& right){
        return left.first < right.first;
    });

    std::cout << "final solution: ";
    for (auto val: finalSolution) {
        std::cout << "(" << val.first << ", " << val.second << "), ";
    }
    std::cout << "\n";

    dispatcher = nullptr;
    looper->stop();
    looper = nullptr;
}

void test2() {

    std::vector<double> x = {1., 2., 3., 4., 5.};
    std::vector<double> y = {2., 3., 4., 5., 6.};
    std::vector<mythread::Problem> problems;
    for (uint k = 0; k < x.size(); ++k) {
        problems.emplace_back(k, x[k], y[k]);
    }

    std::vector<std::unique_ptr<mythread::CLooper>> threads;
    for (uint k = 0; k < 2; ++ k) {
        auto looper = std::make_unique<mythread::CLooper>(k);
        threads.push_back(std::move(looper));
    }

    auto threadPool = mythread::CLooperPool(std::move(threads));

    std::cout << "Starting the thread pool: \n";

    threadPool.run();

    while(!threadPool.running()) {
        // do nothing
    }

    std::cout << "Thread pool started!\n";

    threadPool.solve(problems);

    auto solutions = threadPool.getSolutions();

    std::cout << "Thread solution: ";
    for (auto sol: solutions) {
        std::cout << "(" << sol.first << "," << sol.second << "), ";
    }
    std::cout << "\n";

    threadPool.stop();

    std::cout << "Thread pool stopped!\n";

};

void test3() {
    mythread::Problem problem(1, 2., 3.);
    helloTest();
    cusparsetest::cuspHello();
}

void test4() {
    cusparsetest::createMatrix();
}

int main() {
    test3();
}


