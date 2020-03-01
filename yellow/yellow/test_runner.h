//
// Created by mariakolyachko on 19.12.19.
//

#ifndef YELLOW_TEST_RUNNER_H
#define YELLOW_TEST_RUNNER_H

#include <exception>
#include <iostream>
#include <string>

class TestRunner {
public:
    template <class TestFunc>
    void RunTest(TestFunc func, const std::string& test_name) {
        try {
            func();
            std::cerr << test_name << " OK" << std::endl;
        } catch (std::runtime_error& e) {
            ++fail_count;
            std::cerr << test_name << " fail: " << e.what() << std::endl;
        }
    }

    ~TestRunner();

private:
    int fail_count = 0;
};


#endif //YELLOW_TEST_RUNNER_H
