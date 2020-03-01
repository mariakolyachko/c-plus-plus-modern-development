//
// Created by mariakolyachko on 19.12.19.
//

#ifndef YELLOW_TEST_RUNNER_H
#define YELLOW_TEST_RUNNER_H

#include <exception>
#include <iostream>
#include <string>

template <typename T>
std::ostream &operator<<(std::ostream &out, const std::vector<T> &vector) {
  for (const auto &item : vector) {
    out << item << ' ';
  }
  return out;
}


template<class T, class U>
void AssertEqual(const T& t, const U& u,
                 const std::string& hint);


template<class T, class U>
void AssertEqual(const T& t, const U& u,
                 const std::string& hint)
{
  if (t != u) {
    std::ostringstream os;
    os << "Assertion failed: " << t << " != " << u
       << " hint: " << hint;
    throw std::runtime_error(os.str());
  }
}

void Assert(bool b, const std::string& hint) {
  AssertEqual(b, true, hint);
}

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

    ~TestRunner() {}

private:
    int fail_count = 0;
};


#endif //YELLOW_TEST_RUNNER_H
