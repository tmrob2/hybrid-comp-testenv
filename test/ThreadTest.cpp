//
// Created by thomas on 23/09/23.
//
#include <gtest/gtest.h>
#include "../library.h"
#include <algorithm>

TEST(ThreadTest, BasicThreadSetup) {
    // set up and pull down a thread
    auto looper = std::make_unique<mythread::CLooper>(0);

    ASSERT_EQ(looper->run(), true);

    std::this_thread::sleep_for(std::chrono::seconds(1));

    ASSERT_EQ(looper->stop(), true);
    looper = nullptr;
    ASSERT_EQ(looper, nullptr);
}

TEST(ThreadTest, FuncComputation) {
    std::vector<double> x = {1., 2., 3., 4., 5.};
    std::vector<double> y = {2., 3., 4., 5., 6.};
    std::vector<double> sol = {2., 6., 12., 20., 30.};

    // The dispatchers will need to be looped over in the multi-threaded case.
    auto looper = std::make_unique<mythread::CLooper>(0);
    // Make sure that the worker thread is ready before we give it any work to do

    looper->run();
    while (!looper->running()) {
        // do nothing
    }

    // The dispatchers will need to be looped over in the multi-threaded case.
    auto dispatcher = looper->getDispatcher();

    uint countDown = x.size();
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

    std::sort(finalSolution.begin(), finalSolution.end(), [](const auto& left, const auto& right){
        return left.first < right.first;
    });

    ASSERT_EQ(sol.size(), finalSolution.size());

    for(uint_fast64_t i = 0; i < sol.size(); ++i) {
        ASSERT_EQ(sol[i], finalSolution[i].second);
    }

    dispatcher = nullptr;
    looper->stop();
    looper = nullptr;
}

TEST(ThreadTest, ThreadPool) {
    std::vector<std::unique_ptr<mythread::CLooper>> threads;
    for (uint k = 0; k < 2; ++ k) {
        auto looper = std::make_unique<mythread::CLooper>(k);
        threads.push_back(std::move(looper));
    }

    auto threadPool = mythread::CLooperPool(std::move(threads));

    ASSERT_EQ(threadPool.run(), true);

    while(!threadPool.running()) {
        // do nothing
    }
    // now stop the thread pool
    threadPool.stop();
}

TEST(ThreadTest, ThreadPoolComputation) {
    std::vector<double> x = {1., 2., 3., 4., 5.};
    std::vector<double> y = {2., 3., 4., 5., 6.};
    std::vector<double> sol = {2., 6., 12., 20., 30.};
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

    threadPool.run();

    while(!threadPool.running()) {
        // do nothing
    }

    threadPool.solve(problems);

    auto solutions = threadPool.getSolutions();

    ASSERT_EQ(solutions.size(), sol.size());

    for(uint_fast64_t i = 0; i < solutions.size(); ++i) {
        ASSERT_EQ(sol[i], solutions[i].second);
    }

    threadPool.stop();
}