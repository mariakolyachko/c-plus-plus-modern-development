//
// Created by mariakolyachko on 19.12.19.
//

#include "test_runner.h"

TestRunner::~TestRunner() {
    if (fail_count > 0) {
        std::cerr << fail_count << " unit tests failed. Terminate" << std::endl;
        exit(1);
    }
}

